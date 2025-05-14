#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <array>
#include <vector>
#include <format>
#include <memory>
#include <cmath>

#define WEBGPU_CPP_IMPLEMENTATION
#include <wgpu-native/webgpu.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <sdl2webgpu.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Window.hpp>

#include <Helper/Instance.hpp>
#include <Helper/CompatibleSurface.hpp>
#include <Helper/Limits.hpp>
#include <Helper/Adapter.hpp>
#include <Helper/DeviceLostCallbackInfo.hpp>
#include <Helper/UncapturedErrorCallbackInfo.hpp>
#include <Helper/DeviceDescriptor.hpp>
#include <Helper/Device.hpp>

#include <Helper/StencilFaceState.hpp>
#include <Helper/DepthStencilState.hpp>
#include <Helper/ConstantEntry.hpp>
#include <Helper/BlendComponent.hpp>
#include <Helper/BlendState.hpp>
#include <Helper/ColorTargetState.hpp>
#include <Helper/ShaderModule.hpp>
#include <Helper/FragmentState.hpp>
#include <Helper/VertexState.hpp>
#include <Helper/MultisampleState.hpp>
#include <Helper/PrimitiveState.hpp>
#include <Helper/PipelineLayoutDescriptor.hpp>
#include <Helper/PipelineLayout.hpp>
#include <Helper/RenderPipelineDescriptor.hpp>
#include <Helper/RenderPipeline.hpp>

#include <Helper/BufferDescriptor.hpp>
#include <Helper/Buffer.hpp>
#include <Helper/TextureDescriptor.hpp>
#include <Helper/Texture.hpp>
#include <Helper/TextureViewDescriptor.hpp>
#include <Helper/TextureView.hpp>
#include <Helper/SamplerDescriptor.hpp>
#include <Helper/Sampler.hpp>

#include <Helper/VertexAttribute.hpp>
#include <Helper/VertexBufferLayout.hpp>

#include <Helper/BufferBindingLayout.hpp>
#include <Helper/BufferBinding.hpp>
#include <Helper/TextureBindingLayout.hpp>
#include <Helper/TextureBinding.hpp>
#include <Helper/SamplerBindingLayout.hpp>
#include <Helper/SamplerBinding.hpp>

#include <Helper/BindGroupLayoutDescriptor.hpp>
#include <Helper/BindGroupLayout.hpp>
#include <Helper/BindGroupDescriptor.hpp>
#include <Helper/BindGroup.hpp>

#include <Logger.hpp>
#include <Math/Math.hpp>
#include <Math/Matrix4x4.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

constexpr float const PI = 3.1415926535897932384626433832795f;

constexpr float windowWidth = 640.0f;
constexpr float windowHeight = 480.0f;

struct MyUniforms {
	Math::Matrix4x4 projectionMatrix {};
	Math::Matrix4x4 viewMatrix {};
	Math::Matrix4x4 modelMatrix {};
	Math::Vector4 color {};
	Math::Vector3 cameraPosition {};
	float time = 0.0f;
};

static_assert(sizeof(MyUniforms) % 16 == 0, "MyUniforms must be aligned to 16 bytes.");

struct VertexAttributes {
	Math::Vector3 position {};
	Math::Vector3 normal {};
	Math::Vector3 color {};
	Math::Vector2 uv {};
};

Logger logger {};

auto DeviceLostCallback = [](wgpu::Device const* device, wgpu::DeviceLostReason reason, wgpu::StringView message, void* userData1) {
	std::cerr << "[Lost device] (" << reason << "): " << message << std::endl;
};
auto UncapturedErrorCallback = [](wgpu::Device const* device, wgpu::ErrorType type, wgpu::StringView message, void* userData1) {
	std::cerr << "[Uncaptured error] (" << type << "): " << message << std::endl;
};

bool running = false;

static void WriteMipMaps(wgpu::Device device, wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount, unsigned char* const data) {
	wgpu::Queue queue = device.getQueue();

	wgpu::TexelCopyTextureInfo destination {};
	destination.texture = texture;
	destination.origin = { 0, 0, 0 };
	destination.aspect = wgpu::TextureAspect::All;

	wgpu::TexelCopyBufferLayout source {};
	source.offset = 0;

	wgpu::Extent3D mipLevelSize = textureSize;
	std::vector<uint8_t> previousLevelPixels;
	wgpu::Extent3D previousMipLevelSize;
	for (uint32_t level = 0; level < mipLevelCount; ++level) {
		std::vector<uint8_t> pixels(4 * mipLevelSize.width * mipLevelSize.height);

		if (level == 0) {
			std::memcpy(pixels.data(), data, pixels.size());
		}

		else {
			for (uint32_t i = 0; i < mipLevelSize.width; ++i) {
				for (uint32_t j = 0; j < mipLevelSize.height; ++j) {
					uint8_t* p = &pixels[4 * (j * mipLevelSize.width + i)];

					uint8_t* p00 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 0))];
					uint8_t* p01 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 1))];
					uint8_t* p10 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 0))];
					uint8_t* p11 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 1))];

					p[0] = (p00[0] + p01[0] + p10[0] + p11[0]) / 4;
					p[1] = (p00[1] + p01[1] + p10[1] + p11[1]) / 4;
					p[2] = (p00[2] + p01[2] + p10[2] + p11[2]) / 4;
					p[3] = (p00[3] + p01[3] + p10[3] + p11[3]) / 4;
				}
			}
		}

		destination.mipLevel = level;
		source.bytesPerRow = 4 * mipLevelSize.width;
		source.rowsPerImage = mipLevelSize.height;
		queue.writeTexture(destination, pixels.data(), pixels.size(), source, mipLevelSize);

		previousLevelPixels = std::move(pixels);
		previousMipLevelSize = mipLevelSize;
		mipLevelSize.width /= 2;
		mipLevelSize.height /= 2;
	}

	queue.release();
}

static wgpu::Texture LoadTexture(std::filesystem::path const& path, wgpu::Device device, wgpu::TextureView* pTextureView = nullptr) {
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
	if (data == nullptr) {
		std::cerr << "Failed to load texture: " << path << std::endl;
		return nullptr;
	}

	wgpu::TextureDescriptor textureDescriptor {};
	textureDescriptor.dimension = wgpu::TextureDimension::_2D;
	textureDescriptor.format = wgpu::TextureFormat::RGBA8Unorm;
	textureDescriptor.size = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };
	textureDescriptor.mipLevelCount = std::bit_width(std::max(textureDescriptor.size.width, textureDescriptor.size.height));
	textureDescriptor.sampleCount = 1;
	textureDescriptor.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	textureDescriptor.viewFormatCount = 0;
	textureDescriptor.nextInChain = nullptr;
	wgpu::Texture texture = device.createTexture(textureDescriptor);
	
	WriteMipMaps(device, texture, textureDescriptor.size, textureDescriptor.mipLevelCount, data);

	stbi_image_free(data);

	if (pTextureView != nullptr) {
		wgpu::TextureViewDescriptor textureViewDescriptor {};
		textureViewDescriptor.aspect = wgpu::TextureAspect::All;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.mipLevelCount = textureDescriptor.mipLevelCount;
		textureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
		textureViewDescriptor.format = textureDescriptor.format;
		textureViewDescriptor.nextInChain = nullptr;
		*pTextureView = texture.createView(textureViewDescriptor);
	}

	return texture;
}

static wgpu::TextureView GetNextTexture(wgpu::Device& device, wgpu::Surface& surface) {
	wgpu::SurfaceTexture surfaceTexture {};
	surface.getCurrentTexture(&surfaceTexture);

	//std::cout << "Surface texture: ";
	switch (surfaceTexture.status) {
		case wgpu::SurfaceGetCurrentTextureStatus::DeviceLost:
			std::cerr << "Device lost" << std::endl;
			return nullptr;

		case wgpu::SurfaceGetCurrentTextureStatus::Lost:
			std::cerr << "Lost" << std::endl;
			return nullptr;

		case wgpu::SurfaceGetCurrentTextureStatus::Outdated:
			std::cerr << "Outdated" << std::endl;
			return nullptr;

		case wgpu::SurfaceGetCurrentTextureStatus::OutOfMemory:
			std::cerr << "Out of memory" << std::endl;
			return nullptr;

		case wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal:
			//std::cout << "Success" << std::endl;
			break;

		case wgpu::SurfaceGetCurrentTextureStatus::Timeout:
			std::cerr << "Timeout" << std::endl;
			return nullptr;

		default:
			std::cerr << "Unknown" << std::endl;
			return nullptr;
	}

	wgpu::Texture texture = surfaceTexture.texture;

	wgpu::TextureViewDescriptor textureViewDescriptor {};
	textureViewDescriptor.label = wgpu::StringView("current_surface_texture_view");
	textureViewDescriptor.format = texture.getFormat();
	textureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
	textureViewDescriptor.baseMipLevel = 0;
	textureViewDescriptor.mipLevelCount = 1;
	textureViewDescriptor.baseArrayLayer = 0;
	textureViewDescriptor.arrayLayerCount = 1;
	textureViewDescriptor.aspect = wgpu::TextureAspect::All;
	textureViewDescriptor.nextInChain = nullptr;
	wgpu::TextureView textureView = texture.createView(textureViewDescriptor);

	return textureView;
}

static bool LoadGeometry(std::filesystem::path const& path, std::vector<float>& pointData, std::vector<uint16_t>& indexData, int dimensions) {
	std::ifstream file(path);
	if (!file.is_open()) {
		return false;
	}

	pointData.clear();
	indexData.clear();

	enum class Section {
		None,
		Points,
		Indices,
	};

	Section currentSection = Section::None;

	float value = 0.0;
	uint16_t index = 0;
	std::string line = "";
	while (!file.eof()) {
		std::getline(file, line);

		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}

		if (line == "[points]") {
			currentSection = Section::Points;
		}

		else if (line == "[indices]") {
			currentSection = Section::Indices;
		}

		else if (line[0] == '#' || line.empty()) {
			// Do nothing, this is a comment
		}

		else if (currentSection == Section::Points) {
			std::istringstream iss(line);
			for (int i = 0; i < dimensions + 3; ++i) {
				iss >> value;
				pointData.push_back(value);
			}
		}

		else if (currentSection == Section::Indices) {
			std::istringstream iss(line);
			for (int i = 0; i < 3; ++i) {
				iss >> index;
				indexData.push_back(index);
			}
		}
	}
	return true;
}

static bool LoadGeometryFromOBJ(std::filesystem::path const& path, std::vector<VertexAttributes>& vertexData) {
	tinyobj::attrib_t attrib {};
	std::vector<tinyobj::shape_t> shapes {};
	std::vector<tinyobj::material_t> materials {};

	std::string warn {};
	std::string err {};

	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str());

	if (!warn.empty()) {
		std::cerr << "Warning: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << "Error: " << err << std::endl;
	}

	if (!result) {
		return false;
	}

	for (auto const& shape : shapes) {
		size_t offset = vertexData.size();
		vertexData.resize(offset + shape.mesh.indices.size());
		for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
			tinyobj::index_t idx = shape.mesh.indices[i];

			vertexData[i].position = {
				 attrib.vertices[3 * idx.vertex_index + 0],
				-attrib.vertices[3 * idx.vertex_index + 2],
				 attrib.vertices[3 * idx.vertex_index + 1]
			};

			vertexData[i].normal = {
				 attrib.normals[3 * idx.normal_index + 0],
				-attrib.normals[3 * idx.normal_index + 2],
				 attrib.normals[3 * idx.normal_index + 1]
			};

			vertexData[i].color = {
				 attrib.colors[3 * idx.vertex_index + 0],
				 attrib.colors[3 * idx.vertex_index + 1],
				 attrib.colors[3 * idx.vertex_index + 2]
			};

			vertexData[i].uv = {
				 attrib.texcoords[2 * idx.texcoord_index + 0],
				 1 - attrib.texcoords[2 * idx.texcoord_index + 1]
			};
		}
	}

	return true;
}

static RenderPipeline InitRenderPipeline(Device& device, Adapter& adapter, CompatibleSurface& surface, std::vector<BindGroupLayout>& bindGroupLayouts, std::vector<BindGroup>& bindGroups, std::vector<VertexBufferLayout> const& vertexBufferLayouts, std::vector<BindGroupLayoutEntry> const& bindGroupLayoutEntries, std::vector<BindGroupEntry> const& bindGroupEntries) {
	StencilFaceState stencilBackFaceState;
	StencilFaceState stencilFrontFaceState;
	wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;
	DepthStencilState depthStencilState(stencilFrontFaceState, stencilBackFaceState, depthTextureFormat);

	PrimitiveState primitiveState;
	primitiveState.cullMode = wgpu::CullMode::None; // for debug purposes

	MultisampleState multisampleState;

	std::vector<ConstantEntry> vertexConstantEntries {};
	ShaderModule vertexShaderModule(device, "resources/shader.wgsl");
	VertexState vertexState(wgpu::StringView("vert_main"), vertexShaderModule, vertexBufferLayouts, vertexConstantEntries);
	
	BlendComponent colorComponent(wgpu::BlendFactor::SrcAlpha, wgpu::BlendFactor::OneMinusSrcAlpha, wgpu::BlendOperation::Add);
	BlendComponent alphaComponent(wgpu::BlendFactor::Zero, wgpu::BlendFactor::One, wgpu::BlendOperation::Add);
	BlendState blendState(colorComponent, alphaComponent);
	std::vector<ColorTargetState> colorTargetStates {};
	ColorTargetState colorTargetState(adapter, surface, blendState);
	colorTargetStates.push_back(colorTargetState);
	std::vector<ConstantEntry> fragmentConstantEntries {};
	ShaderModule fragmentShaderModule(device, "resources/shader.wgsl");
	FragmentState fragmentState(wgpu::StringView("frag_main"), fragmentShaderModule, colorTargetStates, fragmentConstantEntries);
	
	BindGroupLayoutDescriptor bindGroupLayoutDescriptor(bindGroupLayoutEntries);
	bindGroupLayouts.emplace_back(device, bindGroupLayoutDescriptor);

	BindGroupDescriptor bindGroupDescriptor(bindGroupLayouts[0], bindGroupEntries);
	bindGroups.emplace_back(device, bindGroupDescriptor);

	PipelineLayoutDescriptor pipelineLayoutDescriptor(bindGroupLayouts);
	PipelineLayout pipelineLayout(device, pipelineLayoutDescriptor);

	RenderPipelineDescriptor renderPipelineDescriptor(depthStencilState, primitiveState, multisampleState, vertexState, fragmentState, pipelineLayout);
	RenderPipeline renderPipeline(device, renderPipelineDescriptor);

	return renderPipeline;
}

int main() {
	try {
		Instance instance;
		Window window;
		CompatibleSurface surface(instance, window);
		Adapter adapter(instance, window, surface);

		Limits limits(adapter);
		limits.maxBufferSize = 150000 * sizeof(VertexAttributes);
		limits.maxVertexBufferArrayStride = sizeof(VertexAttributes);

		DeviceLostCallbackInfo deviceLostCallbackInfo(DeviceLostCallback, nullptr, nullptr);
		UncapturedErrorCallbackInfo uncapturedErrorCallbackInfo(UncapturedErrorCallback, nullptr, nullptr);

		DeviceDescriptor deviceDescriptor(adapter, limits, deviceLostCallbackInfo, uncapturedErrorCallbackInfo);

		Device device(adapter, deviceDescriptor);
		wgpu::Queue queue = device->getQueue();
		surface.Configure(adapter, device, window);

		wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;

		std::vector<wgpu::TextureFormat> viewFormats { depthTextureFormat };
		TextureDescriptor depthTextureDescriptor(depthTextureFormat, wgpu::TextureUsage::RenderAttachment, { static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight), 1 }, viewFormats);
		Texture depthTexture(device, depthTextureDescriptor);
	
		TextureViewDescriptor depthTextureViewDescriptor(wgpu::TextureAspect::DepthOnly, depthTextureFormat);
		TextureView depthTextureView(depthTexture, depthTextureViewDescriptor);

		std::vector<BindGroupLayout> bindGroupLayouts {};
		std::vector<BindGroup> bindGroups {};
		std::vector<VertexAttributes> vertexData {};

		BufferDescriptor uniformBufferDescriptor(sizeof(MyUniforms), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform, "uniform_buffer");
		Buffer uniformBuffer(device, uniformBufferDescriptor);

		BufferDescriptor vertexBufferDescriptor(vertexData.size() * sizeof(VertexAttributes), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex, "vertex_buffer");
		Buffer vertexBuffer(device, vertexBufferDescriptor);

		std::vector<VertexAttribute> vertexAttributes;
		vertexAttributes.push_back(VertexAttribute(0, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, position)));
		vertexAttributes.push_back(VertexAttribute(1, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, normal)));
		vertexAttributes.push_back(VertexAttribute(2, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, color)));
		vertexAttributes.push_back(VertexAttribute(3, wgpu::VertexFormat::Float32x2, offsetof(VertexAttributes, uv)));

		std::vector<VertexBufferLayout> vertexBufferLayouts {};
		VertexBufferLayout vertexBufferLayout(sizeof(VertexAttributes), vertexAttributes);
		vertexBufferLayouts.push_back(vertexBufferLayout);

		std::vector<BindGroupLayoutEntry> bindGroupLayoutEntries {};
		bindGroupLayoutEntries.push_back(BufferBindingLayout(0, wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment, wgpu::BufferBindingType::Uniform, sizeof(MyUniforms)));
		bindGroupLayoutEntries.push_back(TextureBindingLayout(1, wgpu::ShaderStage::Fragment, wgpu::TextureSampleType::Float));
		bindGroupLayoutEntries.push_back(SamplerBindingLayout(2, wgpu::ShaderStage::Fragment, wgpu::SamplerBindingType::Filtering));

		bool success = LoadGeometryFromOBJ("resources/cube.obj", vertexData);
		if (!success) {
			logger.Error("Failed to load geometry.");
			return EXIT_FAILURE;
		}

		TextureView textureView;
		wgpu::Texture texture = LoadTexture("resources/de-geulasse.png", device.Handle(), &textureView);
		if (texture == nullptr) {
			logger.Error("Failed to load texture.");
			return EXIT_FAILURE;
		}

		SamplerDescriptor samplerDescriptor(0.0f, 8.0f);
		Sampler sampler(device, samplerDescriptor);

		queue.writeBuffer(vertexBuffer.Handle(), 0, vertexData.data(), vertexBufferDescriptor.size);
		int indexCount = static_cast<int>(vertexData.size());

		std::vector<BindGroupEntry> bindGroupEntries {};
		bindGroupEntries.push_back(BufferBinding(0, uniformBuffer, sizeof(MyUniforms), 0));
		bindGroupEntries.push_back(TextureBinding(1, textureView));
		bindGroupEntries.push_back(SamplerBinding(2, sampler));

		RenderPipeline renderPipeline = InitRenderPipeline(device, adapter, surface, bindGroupLayouts, bindGroups, vertexBufferLayouts, bindGroupLayoutEntries, bindGroupEntries);

		MyUniforms uniforms {};
	
		Math::Vector3 cameraPosition(-300.0, -400.0, -300.0);
		uniforms.cameraPosition = cameraPosition;

		Math::Matrix4x4 S = Math::Matrix4x4::Scale(100.0f);
		uniforms.modelMatrix = Math::Matrix4x4::Transpose(S);
	
		Math::Matrix4x4 L = Math::Matrix4x4::LookAt(cameraPosition, Math::Vector3( 0.0f,  0.0f, 0.0f), Math::Vector3( 0.0f,  0.0f, 1.0f));
		uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);

		float ratio = windowWidth / windowHeight;
		float vfov = 45.0f * PI / 180.0f;
		float near = 0.01f;
		float far = 1000.0f;
		Math::Matrix4x4 P = Math::Matrix4x4::Perspective(vfov, ratio, near, far);
		uniforms.projectionMatrix = Math::Matrix4x4::Transpose(P);

		uniforms.color = { 0.0f, 1.0f, 0.4f, 1.0f };
		uniforms.time = 1.0f;

		queue.writeBuffer(uniformBuffer.Handle(), 0, &uniforms, sizeof(MyUniforms));

		Uint8 const* keyboard = SDL_GetKeyboardState(nullptr);

		running = true;
		SDL_Event event {};
		while (running) {
			if (SDL_PollEvent(&event) > 0) {
				if (event.type == SDL_QUIT) {
					running = false;
				}

				if (keyboard[SDL_SCANCODE_Z]) {
					cameraPosition.y += 0.01f;
				}

				if (keyboard[SDL_SCANCODE_S]) {
					cameraPosition.y -= 0.01f;
				}
			}

			wgpu::TextureView textureView = GetNextTexture(device.Handle(), surface.Handle());
			if (textureView == nullptr) {
				logger.Error("Failed to get texture view.");
				return EXIT_FAILURE;
			}

			uniforms.time = -static_cast<float>(SDL_GetTicks()) / 1000;
			queue.writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, time), &uniforms.time, sizeof(MyUniforms::time));

			cameraPosition = Math::Vector3(-300.0f * std::cos(uniforms.time), -400.0f, -300.0f * std::sin(uniforms.time));
			uniforms.cameraPosition = cameraPosition;

			//std::cout << "cameraPosition: " << cameraPosition << std::endl;

			queue.writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, cameraPosition), &uniforms.cameraPosition, sizeof(MyUniforms::cameraPosition));

			L = Math::Matrix4x4::LookAt(cameraPosition, Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f));

			uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);
			queue.writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, viewMatrix), &uniforms.viewMatrix, sizeof(MyUniforms::viewMatrix));

			wgpu::CommandEncoderDescriptor commandEncoderDescriptor {};
			commandEncoderDescriptor.label = wgpu::StringView("command_encoder");
			commandEncoderDescriptor.nextInChain = nullptr;

			wgpu::CommandEncoder commandEncoder = device->createCommandEncoder(commandEncoderDescriptor);
			if (commandEncoder == nullptr) {
				logger.Error("Failed to create command encoder.");
				return EXIT_FAILURE;
			}

			wgpu::RenderPassEncoder renderPassEncoder = nullptr;
			{
				std::vector<wgpu::RenderPassColorAttachment> renderPassColorAttachments{};
				renderPassColorAttachments.resize(1, wgpu::RenderPassColorAttachment{});
				renderPassColorAttachments[0].loadOp = wgpu::LoadOp::Clear;
				renderPassColorAttachments[0].resolveTarget = nullptr;
				renderPassColorAttachments[0].storeOp = wgpu::StoreOp::Store;
				renderPassColorAttachments[0].view = textureView;
				renderPassColorAttachments[0].clearValue = wgpu::Color{ 0.05f, 0.05f, 0.05f, 1.0f };
				renderPassColorAttachments[0].depthSlice = 0;
				renderPassColorAttachments[0].nextInChain = nullptr;

				std::vector<wgpu::RenderPassDepthStencilAttachment> renderPassDepthStencilAttachments{};
				renderPassDepthStencilAttachments.resize(1, wgpu::RenderPassDepthStencilAttachment{});
				renderPassDepthStencilAttachments[0].view = depthTextureView.Handle();
				renderPassDepthStencilAttachments[0].depthClearValue = 1.0f;
				renderPassDepthStencilAttachments[0].depthLoadOp = wgpu::LoadOp::Clear;
				renderPassDepthStencilAttachments[0].depthStoreOp = wgpu::StoreOp::Store;
				renderPassDepthStencilAttachments[0].depthReadOnly = false;
				renderPassDepthStencilAttachments[0].stencilClearValue = 0;
				renderPassDepthStencilAttachments[0].stencilLoadOp = wgpu::LoadOp::Clear;
				renderPassDepthStencilAttachments[0].stencilStoreOp = wgpu::StoreOp::Store;
				renderPassDepthStencilAttachments[0].stencilReadOnly = true;

				wgpu::QuerySet occlusionQuerySet {};

				wgpu::RenderPassDescriptor renderPassDescriptor{};
				renderPassDescriptor.colorAttachmentCount = renderPassColorAttachments.size();
				renderPassDescriptor.colorAttachments = renderPassColorAttachments.data();
				renderPassDescriptor.depthStencilAttachment = &renderPassDepthStencilAttachments[0];
				renderPassDescriptor.label = wgpu::StringView("render_pass");
				renderPassDescriptor.nextInChain = nullptr;
				renderPassDescriptor.occlusionQuerySet = occlusionQuerySet;
				renderPassDescriptor.timestampWrites = nullptr;

				renderPassEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
				if (renderPassEncoder == nullptr) {
					logger.Error("Failed to create render pass encoder.");
					return EXIT_FAILURE;
				}
			}

			renderPassEncoder.setPipeline(renderPipeline.Handle());
			renderPassEncoder.setVertexBuffer(0, vertexBuffer.Handle(), 0, vertexData.size() * sizeof(VertexAttributes));
			renderPassEncoder.setBindGroup(0, bindGroups[0].Handle(), 0, nullptr);
			renderPassEncoder.draw(indexCount, 1, 0, 0);
		
			renderPassEncoder.end();

			wgpu::CommandBufferDescriptor commandBufferDescriptor {};
			commandBufferDescriptor.label = wgpu::StringView("command_buffer");
			commandBufferDescriptor.nextInChain = nullptr;
			wgpu::CommandBuffer commandBuffer = commandEncoder.finish();
		
			queue.submit(commandBuffer);

			renderPassEncoder.release();
			commandEncoder.release();
			commandBuffer.release();

			surface->present();
		}
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	logger.Error("Successfully exited.");
	return EXIT_SUCCESS;
}
