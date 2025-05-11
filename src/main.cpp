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
#include <Helper/DeviceDescriptor.hpp>
#include <Helper/Device.hpp>

#include <Helper/StencilFaceState.hpp>
#include <Helper/DepthStencilState.hpp>
#include <Helper/RenderPipeline.hpp>

#include <Helper/TextureDescriptor.hpp>
#include <Helper/Texture.hpp>
#include <Helper/TextureViewDescriptor.hpp>
#include <Helper/TextureView.hpp>

#include <Helper/BufferBindingLayout.hpp>
#include <Helper/TextureBindingLayout.hpp>
#include <Helper/SamplerBindingLayout.hpp>
#include <Helper/BufferBinding.hpp>
#include <Helper/TextureBinding.hpp>
#include <Helper/SamplerBinding.hpp>

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
	std::cerr << "Lost device (" << reason << "): " << message << std::endl;
	};
auto UncapturedErrorCallback = [](WGPUErrorType errorType, wgpu::StringView message, void* userData1, void* userData2) {
	std::cerr << "Uncaptured error (" << errorType << "): " << message << std::endl;
	};

//wgpu::Instance instance = nullptr;
//SDL_Window* window = nullptr;
//wgpu::Surface surface = nullptr;
//wgpu::Adapter adapter = nullptr;
//wgpu::Device device = nullptr;
//std::unique_ptr<wgpu::UncapturedErrorCallback> errorCallback = nullptr;
wgpu::RenderPipeline renderPipeline = nullptr;
wgpu::ShaderModule fragmentShaderModule = nullptr;
wgpu::ShaderModule vertexShaderModule = nullptr;
wgpu::PipelineLayout pipelineLayout = nullptr;
wgpu::Buffer vertexBuffer = nullptr;
wgpu::Buffer indexBuffer = nullptr;
wgpu::Buffer uniformBuffer = nullptr;
std::vector<wgpu::BindGroupLayout> bindGroupLayouts{};
std::vector<wgpu::BindGroup> bindGroups{};
uint32_t uniformStride = 0;
//wgpu::Texture depthTexture = nullptr;
//wgpu::TextureView depthTextureView = nullptr;
wgpu::Texture texture = nullptr;
wgpu::Sampler sampler = nullptr;
std::vector<VertexAttributes> vertexData;

auto CleanOnExit(int code, std::string message = "", bool error = false) -> int {
	if (error) {
		logger.Error(message);
	}

	else {
		logger.Info(message);
	}

	if (pipelineLayout != nullptr) {
		pipelineLayout.release();
		pipelineLayout = nullptr;
	}

	if (fragmentShaderModule != nullptr) {
		fragmentShaderModule.release();
		fragmentShaderModule = nullptr;
	}

	if (vertexShaderModule != nullptr) {
		vertexShaderModule.release();
		vertexShaderModule = nullptr;
	}

	if (renderPipeline != nullptr) {
		renderPipeline.release();
		renderPipeline = nullptr;
	}

	if (vertexBuffer != nullptr) {
		vertexBuffer.release();
		vertexBuffer = nullptr;
	}

	if (indexBuffer != nullptr) {
		indexBuffer.release();
		indexBuffer = nullptr;
	}

	if (uniformBuffer != nullptr) {
		uniformBuffer.release();
		uniformBuffer = nullptr;
	}

	for (auto& bindGroupLayout : bindGroupLayouts) {
		if (bindGroupLayout != nullptr) {
			bindGroupLayout.release();
			bindGroupLayout = nullptr;
		}
	}

	for (auto& bindGroup : bindGroups) {
		if (bindGroup != nullptr) {
			bindGroup.release();
			bindGroup = nullptr;
		}
	}

	if (texture != nullptr) {
		texture.destroy();
		texture.release();
		texture = nullptr;
	}

	return code;
	};
bool running = false;

static void WriteMipMaps(wgpu::Device device, wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount, unsigned char* const data) {
	wgpu::Queue queue = device.getQueue();

	wgpu::TexelCopyTextureInfo destination{};
	destination.texture = texture;
	destination.origin = { 0, 0, 0 };
	destination.aspect = wgpu::TextureAspect::All;

	wgpu::TexelCopyBufferLayout source{};
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

static wgpu::VertexAttribute CreateVertexAttribute(uint32_t location, wgpu::VertexFormat format, uint32_t offset) {
	wgpu::VertexAttribute vertexAttribute{};

	vertexAttribute.shaderLocation = location;
	vertexAttribute.format = format;
	vertexAttribute.offset = offset;

	return vertexAttribute;
}

static void EditVertexAttribute(wgpu::VertexAttribute& vertexAttribute, uint32_t location, wgpu::VertexFormat format, uint32_t offset) {
	vertexAttribute.shaderLocation = location;
	vertexAttribute.format = format;
	vertexAttribute.offset = offset;
}

static void GetDeviceSupportedLimits(wgpu::Device& device, wgpu::Limits* limits, bool log = false, Logger* logger = nullptr) {
	if (device == nullptr) {
		if (logger != nullptr) {
			logger->Warn("Device is invalid.");
		}
		return;
	}

	wgpu::Bool retrievedLimits = device.getLimits(limits);

	if (log) {
		if (retrievedLimits) {
			std::cout << "\t- " << "Limits: " << std::endl;

			std::cout << "\t\t- " << "Max bind groups: " << limits->maxBindGroups << std::endl;
			std::cout << "\t\t- " << "Max bind groups plus vertex buffers: " << limits->maxBindGroupsPlusVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Max bindings per bind group: " << limits->maxBindingsPerBindGroup << std::endl;
			std::cout << "\t\t- " << "Max buffer size: " << limits->maxBufferSize << std::endl;
			std::cout << "\t\t- " << "Max color attachment bytes per sample: " << limits->maxColorAttachmentBytesPerSample << std::endl;
			std::cout << "\t\t- " << "Max color attachments: " << limits->maxColorAttachments << std::endl;
			std::cout << "\t\t- " << "Max compute invocations per workgroup: " << limits->maxComputeInvocationsPerWorkgroup << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size x: " << limits->maxComputeWorkgroupSizeX << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size y: " << limits->maxComputeWorkgroupSizeY << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size z: " << limits->maxComputeWorkgroupSizeZ << std::endl;
			std::cout << "\t\t- " << "Max compute workgroups per dimension: " << limits->maxComputeWorkgroupsPerDimension << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup storage size: " << limits->maxComputeWorkgroupStorageSize << std::endl;
			std::cout << "\t\t- " << "Max dynamic storage buffers per pipeline layout: " << limits->maxDynamicStorageBuffersPerPipelineLayout << std::endl;
			std::cout << "\t\t- " << "Max dynamic uniform buffers per pipeline layout: " << limits->maxDynamicUniformBuffersPerPipelineLayout << std::endl;
			//std::cout << "\t\t- " << "Max inter stage shader components: " << limits->maxInterStageShaderComponents << std::endl;
			std::cout << "\t\t- " << "Max inter stage shader variables: " << limits->maxInterStageShaderVariables << std::endl;
			std::cout << "\t\t- " << "Max sampled textures per shader stage: " << limits->maxSampledTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max samplers per shader stage: " << limits->maxSamplersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage buffer binding size: " << limits->maxStorageBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max storage buffers per shader stage: " << limits->maxStorageBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage textures per shader stage: " << limits->maxStorageTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max texture array layers: " << limits->maxTextureArrayLayers << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 1D: " << limits->maxTextureDimension1D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 2D: " << limits->maxTextureDimension2D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 3D: " << limits->maxTextureDimension3D << std::endl;
			std::cout << "\t\t- " << "Max uniform buffer binding size: " << limits->maxUniformBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max uniform buffers per shader stage: " << limits->maxUniformBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max vertex attributes: " << limits->maxVertexAttributes << std::endl;
			std::cout << "\t\t- " << "Max vertex buffer array stride: " << limits->maxVertexBufferArrayStride << std::endl;
			std::cout << "\t\t- " << "Max vertex buffers: " << limits->maxVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Min storage buffer offset alignment: " << limits->minStorageBufferOffsetAlignment << std::endl;
			std::cout << "\t\t- " << "Min uniform buffer offset alignment: " << limits->minUniformBufferOffsetAlignment << std::endl;

			std::cout << std::endl;
		}
	}
}

static bool CreateRenderPipeline(wgpu::RenderPipeline& renderPipeline, wgpu::Device& device, wgpu::RenderPipelineDescriptor const& renderPipelineDescriptor) {
	renderPipeline = device.createRenderPipeline(renderPipelineDescriptor);

	return (renderPipeline != nullptr);
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

	wgpu::TextureViewDescriptor textureViewDescriptor{};
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

static wgpu::ShaderModule LoadShaderModule(std::filesystem::path const& path, wgpu::Device device) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Can't open " << path << std::endl;
		return nullptr;
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	std::string shaderSource(size, ' ');
	file.seekg(0);
	file.read(shaderSource.data(), size);

	wgpu::ShaderSourceWGSL shaderCodeDesc{};
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = wgpu::SType::ShaderSourceWGSL;
	shaderCodeDesc.code = wgpu::StringView(shaderSource.c_str());

	wgpu::ShaderModuleDescriptor shaderDesc{};
	
	shaderDesc.label = wgpu::StringView(path.stem().string().c_str());
	shaderDesc.nextInChain = &shaderCodeDesc.chain;

	return device.createShaderModule(shaderDesc);
}

static bool LoadGeometryFromOBJ(std::filesystem::path const& path, std::vector<VertexAttributes>& vertexData) {
	tinyobj::attrib_t attrib {};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};

	std::string warn{};
	std::string err{};

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

static int InitRenderPipeline(Device device, wgpu::Adapter adapter, wgpu::Surface surface, std::vector<wgpu::VertexBufferLayout> const& vertexBufferLayouts, std::vector<wgpu::BindGroupLayoutEntry> const& bindGroupLayoutEntries, std::vector<wgpu::BindGroupEntry> const& bindGroupEntries) {
	StencilFaceState stencilBackFaceState {};
	StencilFaceState stencilFrontFaceState {};

	wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;

	DepthStencilState depthStencilState(stencilFrontFaceState, stencilBackFaceState, depthTextureFormat);

	std::vector<wgpu::ConstantEntry> fragmentConstantEntries {};
	//std::vector<wgpu::ShaderModuleCompilationHint> fragmentShaderModuleCompilationHints {};

	wgpu::ChainedStruct fragmentShaderModuleChainedStruct {};
	fragmentShaderModuleChainedStruct.next = nullptr;
	fragmentShaderModuleChainedStruct.sType = wgpu::SType::ShaderSourceWGSL;

	fragmentShaderModule = LoadShaderModule("resources/shader.wgsl", device.Handle());
	if (fragmentShaderModule == nullptr) {
		logger.Error("Failed to create fragment shader module.");
		return EXIT_FAILURE;
	}

	wgpu::BlendComponent colorComponent {};
	colorComponent.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
	colorComponent.operation = wgpu::BlendOperation::Add;
	colorComponent.srcFactor = wgpu::BlendFactor::SrcAlpha;

	wgpu::BlendComponent alphaComponent {};
	alphaComponent.dstFactor = wgpu::BlendFactor::One;
	alphaComponent.operation = wgpu::BlendOperation::Add;
	alphaComponent.srcFactor = wgpu::BlendFactor::Zero;

	wgpu::BlendState blendState {};
	blendState.alpha = alphaComponent;
	blendState.color = colorComponent;

	std::vector<wgpu::ColorTargetState> colorTargetStates {};
	colorTargetStates.resize(1, wgpu::ColorTargetState{});
	colorTargetStates[0].blend = &blendState;

	wgpu::SurfaceCapabilities surfaceCapabilities;
	surface.getCapabilities(adapter, &surfaceCapabilities);

	colorTargetStates[0].format = surfaceCapabilities.formats[0];
	colorTargetStates[0].nextInChain = nullptr;
	colorTargetStates[0].writeMask = wgpu::ColorWriteMask::All;

	wgpu::FragmentState fragmentState {};
	fragmentState.constantCount = fragmentConstantEntries.size();
	fragmentState.constants = fragmentConstantEntries.data();
	fragmentState.entryPoint = wgpu::StringView("frag_main");
	fragmentState.module = fragmentShaderModule;
	fragmentState.nextInChain = nullptr;
	fragmentState.targetCount = colorTargetStates.size();
	fragmentState.targets = colorTargetStates.data();

	wgpu::BindGroupLayoutDescriptor bindGroupLayoutDescriptor {};
	bindGroupLayoutDescriptor.nextInChain = nullptr;
	bindGroupLayoutDescriptor.entryCount = bindGroupLayoutEntries.size();
	bindGroupLayoutDescriptor.entries = bindGroupLayoutEntries.data();
	bindGroupLayouts.push_back(device.Handle().createBindGroupLayout(bindGroupLayoutDescriptor));
	if (bindGroupLayouts[0] == nullptr) {
		logger.Error("Failed to create bind group layout.");
		return EXIT_FAILURE;
	}

	wgpu::BindGroupDescriptor bindGroupDescriptor {};
	bindGroupDescriptor.nextInChain = nullptr;
	bindGroupDescriptor.layout = bindGroupLayouts[0];
	bindGroupDescriptor.entryCount = (uint32_t)(bindGroupEntries.size());
	bindGroupDescriptor.entries = bindGroupEntries.data();
	wgpu::BindGroup bindGroup = device.Handle().createBindGroup(bindGroupDescriptor);
	if (bindGroup == nullptr) {
		logger.Error("Failed to create bind group.");
		return EXIT_FAILURE;
	}

	bindGroups.push_back(bindGroup);

	wgpu::PipelineLayoutDescriptor pipelineLayoutDescriptor {};
	pipelineLayoutDescriptor.bindGroupLayoutCount = bindGroupLayouts.size();
	pipelineLayoutDescriptor.bindGroupLayouts = (WGPUBindGroupLayout*)(bindGroupLayouts.data());
	pipelineLayoutDescriptor.label = wgpu::StringView("pipeline_layout");
	pipelineLayoutDescriptor.nextInChain = nullptr;
	pipelineLayout = device.Handle().createPipelineLayout(pipelineLayoutDescriptor);
	if (pipelineLayout == nullptr) {
		logger.Error("Failed to create pipeline layout.");
		return EXIT_FAILURE;
	}

	wgpu::MultisampleState multisampleState {};
	multisampleState.alphaToCoverageEnabled = false;
	multisampleState.count = 1;
	multisampleState.mask = 0xFFFFFFFF;
	multisampleState.nextInChain = nullptr;

	wgpu::PrimitiveState primitiveState {};
	primitiveState.cullMode = wgpu::CullMode::None;
	primitiveState.frontFace = wgpu::FrontFace::CCW;
	primitiveState.nextInChain = nullptr;
	primitiveState.stripIndexFormat = wgpu::IndexFormat::Undefined;
	primitiveState.topology = wgpu::PrimitiveTopology::TriangleList;

	std::vector<wgpu::ConstantEntry> vertexConstantEntries {};
	//std::vector<wgpu::ShaderModuleCompilationHint> vertexShaderModuleCompilationHints {};

	vertexShaderModule = LoadShaderModule("resources/shader.wgsl", device.Handle());
	if (vertexShaderModule == nullptr) {
		logger.Error("Failed to create vertex shader module.");
		return EXIT_FAILURE;
	}

	wgpu::VertexState vertexState {};
	vertexState.bufferCount = vertexBufferLayouts.size();
	vertexState.buffers = vertexBufferLayouts.data();
	vertexState.constantCount = vertexConstantEntries.size();
	vertexState.constants = vertexConstantEntries.data();
	vertexState.entryPoint = wgpu::StringView("vert_main");
	vertexState.module = vertexShaderModule;

	wgpu::RenderPipelineDescriptor renderPipelineDescriptor {};
	renderPipelineDescriptor.depthStencil = &depthStencilState;
	renderPipelineDescriptor.fragment = &fragmentState;
	renderPipelineDescriptor.label = wgpu::StringView("render_pipeline");
	renderPipelineDescriptor.layout = pipelineLayout;
	renderPipelineDescriptor.multisample = multisampleState;
	renderPipelineDescriptor.nextInChain = nullptr;
	renderPipelineDescriptor.primitive = primitiveState;
	renderPipelineDescriptor.vertex = vertexState;

	if (!CreateRenderPipeline(renderPipeline, device.Handle(), renderPipelineDescriptor)) {
		logger.Error("Failed to create render pipeline.");
		return EXIT_FAILURE;
	}

	logger.Info("Render pipeline created.");

	return EXIT_SUCCESS;
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
		DeviceDescriptor deviceDescriptor(adapter, limits);

		//uniformStride = Math::CeilToNextMultiple(static_cast<uint32_t>(sizeof(MyUniforms)),
		//	static_cast<uint32_t>(deviceSupportedLimits.minUniformBufferOffsetAlignment));
		//wgpu::DeviceLostCallback deviceLostCallback = (wgpu::DeviceLostCallback)(DeviceLostCallback);
		
		Device device(adapter, deviceDescriptor);
		wgpu::Queue queue = device.Handle().getQueue();
		surface.Configure(adapter, device, window);

		wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;

		std::vector<wgpu::TextureFormat> viewFormats { depthTextureFormat };
		TextureDescriptor depthTextureDescriptor(depthTextureFormat, wgpu::TextureUsage::RenderAttachment, { static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight), 1 }, viewFormats);
		Texture depthTexture(device, depthTextureDescriptor);
	
		TextureViewDescriptor depthTextureViewDescriptor(wgpu::TextureAspect::DepthOnly, depthTextureFormat);
		TextureView depthTextureView(depthTexture, depthTextureViewDescriptor);

		// initialiazion of the assets
		{
			std::vector<wgpu::VertexBufferLayout> vertexBufferLayouts{};

			std::vector<wgpu::VertexAttribute> vertexAttributes(4);
			EditVertexAttribute(vertexAttributes[0], 0, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, position));
			EditVertexAttribute(vertexAttributes[1], 1, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, normal));
			EditVertexAttribute(vertexAttributes[2], 2, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, color));
			EditVertexAttribute(vertexAttributes[3], 3, wgpu::VertexFormat::Float32x2, offsetof(VertexAttributes, uv));

			wgpu::VertexBufferLayout vertexBufferLayout{};
			vertexBufferLayout.attributeCount = static_cast<uint32_t>(vertexAttributes.size());
			vertexBufferLayout.attributes = vertexAttributes.data();
			vertexBufferLayout.arrayStride = sizeof(VertexAttributes);
			vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;
			vertexBufferLayouts.push_back(vertexBufferLayout);

			std::vector<wgpu::BindGroupLayoutEntry> bindGroupLayoutEntries(3, wgpu::Default);
			EditBufferBindingLayout(bindGroupLayoutEntries[0], 0, wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment, wgpu::BufferBindingType::Uniform, sizeof(MyUniforms), false);
			EditTextureBindingLayout(bindGroupLayoutEntries[1], 1, wgpu::ShaderStage::Fragment, wgpu::TextureSampleType::Float, wgpu::TextureViewDimension::_2D);
			EditSamplerBindingLayout(bindGroupLayoutEntries[2], 2, wgpu::ShaderStage::Fragment, wgpu::SamplerBindingType::Filtering);

			//DisplayBufferBindingLayout(bindGroupLayoutEntries[0]); std::cout << std::endl;
			//DisplayTextureBindingLayout(bindGroupLayoutEntries[1]); std::cout << std::endl;
			//DisplaySamplerBindingLayout(bindGroupLayoutEntries[2]); std::cout << std::endl;

			wgpu::BufferDescriptor bufferDescriptor{};
			bufferDescriptor.label = wgpu::StringView("uniform_buffer");
			bufferDescriptor.size = sizeof(MyUniforms);
			bufferDescriptor.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
			bufferDescriptor.mappedAtCreation = false;
			uniformBuffer = device.Handle().createBuffer(bufferDescriptor);
			if (uniformBuffer == nullptr) {
				return CleanOnExit(EXIT_FAILURE, "Failed to create uniform buffer.", true);
			}

			bool success = LoadGeometryFromOBJ("resources/cube.obj", vertexData);
			if (!success) {
				return CleanOnExit(EXIT_FAILURE, "Failed to load geometry.", true);
			}

			wgpu::TextureView textureView = nullptr;
			wgpu::Texture texture = LoadTexture("resources/de-geulasse.png", device.Handle(), &textureView);
			if (texture == nullptr) {
				return CleanOnExit(EXIT_FAILURE, "Failed to create texture.", true);
			}

			wgpu::SamplerDescriptor samplerDescriptor{};
			samplerDescriptor.addressModeU = wgpu::AddressMode::Repeat;
			samplerDescriptor.addressModeV = wgpu::AddressMode::Repeat;
			samplerDescriptor.addressModeW = wgpu::AddressMode::ClampToEdge;
			samplerDescriptor.magFilter = wgpu::FilterMode::Linear;
			samplerDescriptor.minFilter = wgpu::FilterMode::Linear;
			samplerDescriptor.mipmapFilter = wgpu::MipmapFilterMode::Linear;
			samplerDescriptor.lodMinClamp = 0.0f;
			samplerDescriptor.lodMaxClamp = 8.0f;
			samplerDescriptor.compare = wgpu::CompareFunction::Undefined;
			samplerDescriptor.maxAnisotropy = 1;
			sampler = device.Handle().createSampler(samplerDescriptor);

			std::vector<wgpu::BindGroupEntry> bindGroupEntries(3, wgpu::Default);
			EditBufferBinding(bindGroupEntries[0], 0, uniformBuffer, sizeof(MyUniforms), false);
			EditTextureBinding(bindGroupEntries[1], 1, textureView);
			EditSamplerBinding(bindGroupEntries[2], 2, sampler);
		
			//DisplayBufferBinding(bindGroupEntries[0]); std::cout << std::endl;
			//DisplayTextureBinding(bindGroupEntries[1]); std::cout << std::endl;
			//DisplaySamplerBinding(bindGroupEntries[2]); std::cout << std::endl;

			if (InitRenderPipeline(device, adapter.Handle(), surface.Handle(), vertexBufferLayouts, bindGroupLayoutEntries, bindGroupEntries) == EXIT_FAILURE)    return CleanOnExit(EXIT_FAILURE, "Failed to create render pipeline 2.", true);
		}

		wgpu::BufferDescriptor vertexBufferDescriptor {};
		vertexBufferDescriptor.label = wgpu::StringView("vertex_buffer");
		vertexBufferDescriptor.size = vertexData.size() * sizeof(VertexAttributes);
		vertexBufferDescriptor.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
		vertexBufferDescriptor.mappedAtCreation = false;
		vertexBuffer = device.Handle().createBuffer(vertexBufferDescriptor);
		if (vertexBuffer == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create vertex buffer.", true);
		}

		queue.writeBuffer(vertexBuffer, 0, vertexData.data(), vertexBufferDescriptor.size);

		int indexCount = static_cast<int>(vertexData.size());

		MyUniforms uniforms {};
	
		Math::Vector3 cameraPosition(-300.0, -400.0, -300.0);
		uniforms.cameraPosition = cameraPosition;

		Math::Matrix4x4 S = Math::Matrix4x4::Scale(100.0f);
		uniforms.modelMatrix = Math::Matrix4x4::Transpose(S);
	
		Math::Matrix4x4 L = Math::Matrix4x4::LookAt(
			cameraPosition, // eye
			Math::Vector3( 0.0f,  0.0f, 0.0f), // target
			Math::Vector3( 0.0f,  0.0f, 1.0f)  // up
		);
		uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);

		float ratio = windowWidth / windowHeight;
		float vfov = 45.0f * PI / 180.0f;
		float near = 0.01f;
		float far = 1000.0f;
		Math::Matrix4x4 P = Math::Matrix4x4::Perspective(vfov, ratio, near, far);
		uniforms.projectionMatrix = Math::Matrix4x4::Transpose(P);

		uniforms.color = { 0.0f, 1.0f, 0.4f, 1.0f };
		uniforms.time = 1.0f;

		queue.writeBuffer(uniformBuffer, 0, &uniforms, sizeof(MyUniforms));

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
				return CleanOnExit(EXIT_FAILURE, "Failed to get texture view.", true);
			}

			uniforms.time = -static_cast<float>(SDL_GetTicks()) / 1000;
			queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, time), &uniforms.time, sizeof(MyUniforms::time));

			cameraPosition = Math::Vector3(-300.0f * std::cos(uniforms.time), -400.0f, -300.0f * std::sin(uniforms.time));
			uniforms.cameraPosition = cameraPosition;

			//std::cout << "cameraPosition: " << cameraPosition << std::endl;

			queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, cameraPosition), &uniforms.cameraPosition, sizeof(MyUniforms::cameraPosition));

			L = Math::Matrix4x4::LookAt(cameraPosition, Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f));

			uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);
			queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, viewMatrix), &uniforms.viewMatrix, sizeof(MyUniforms::viewMatrix));

			wgpu::CommandEncoderDescriptor commandEncoderDescriptor {};
			commandEncoderDescriptor.label = wgpu::StringView("command_encoder");
			commandEncoderDescriptor.nextInChain = nullptr;
		
			wgpu::CommandEncoder commandEncoder = device.Handle().createCommandEncoder(commandEncoderDescriptor);
			if (commandEncoder == nullptr) {
				return CleanOnExit(EXIT_FAILURE, "Failed to create command encoder.", true);
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

				std::vector<wgpu::RenderPassDepthStencilAttachment> renderPassDepthStencilAttachments{};
				renderPassDepthStencilAttachments.resize(1, wgpu::RenderPassDepthStencilAttachment{});
				renderPassDepthStencilAttachments[0].view = depthTextureView.Handle();
				std::cout << "depthTextureView: " << depthTextureView << depthTextureView.Handle() << std::endl;
				renderPassDepthStencilAttachments[0].depthClearValue = 1.0f;
				renderPassDepthStencilAttachments[0].depthLoadOp = wgpu::LoadOp::Clear;
				renderPassDepthStencilAttachments[0].depthStoreOp = wgpu::StoreOp::Store;
				renderPassDepthStencilAttachments[0].depthReadOnly = false;
				renderPassDepthStencilAttachments[0].stencilClearValue = 0;
				renderPassDepthStencilAttachments[0].stencilLoadOp = wgpu::LoadOp::Clear;
				renderPassDepthStencilAttachments[0].stencilStoreOp = wgpu::StoreOp::Store;
				renderPassDepthStencilAttachments[0].stencilReadOnly = true;

				wgpu::QuerySet occlusionQuerySet{};

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
					return CleanOnExit(EXIT_FAILURE, "Failed to create render pass encoder.", true);
				}
			}

			renderPassEncoder.setPipeline(renderPipeline);
			renderPassEncoder.setVertexBuffer(0, vertexBuffer, 0, vertexData.size() * sizeof(VertexAttributes));
			renderPassEncoder.setBindGroup(0, bindGroups[0], 0, nullptr);
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

			surface.Handle().present();
		}
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return CleanOnExit(EXIT_SUCCESS, "Successfully exited.");
}
