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

#include <Window.hpp>

#include <Helper/Handles.hpp>
#include <Helper/Descriptors.hpp>
#include <Helper/ParametersStructs.hpp>

#include <Resources/Geometry.hpp>

#include <Logger.hpp>
#include <Math/Math.hpp>
#include <Math/Matrix4x4.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

Logger logger {};

auto DeviceLostCallback = [](wgpu::Device const* device, wgpu::DeviceLostReason reason, wgpu::StringView message, void* userData1) {
	std::cerr << "[Lost device] (" << reason << "): " << message << std::endl;
};
auto UncapturedErrorCallback = [](wgpu::Device const* device, wgpu::ErrorType type, wgpu::StringView message, void* userData1) {
	std::cerr << "[Uncaptured error] (" << type << "): " << message << std::endl;
};

bool running = false;

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
		Queue queue(device);

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

		logger.Info("Geometry loaded successfully.");

		TextureView textureView;
		Texture texture = LoadTexture("resources/de-geulasse.png", device.Handle(), &textureView.Handle());

		logger.Info("Texture loaded successfully.");
		
		SamplerDescriptor samplerDescriptor(0.0f, 8.0f);
		Sampler sampler(device, samplerDescriptor);

		BufferDescriptor uniformBufferDescriptor(sizeof(MyUniforms), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform, "uniform_buffer");
		Buffer uniformBuffer(device, uniformBufferDescriptor);

		std::cout << " • vertexData.size(): " << vertexData.size() << std::endl;
		std::cout << " • sizeof(VertexAttributes): " << sizeof(VertexAttributes) << std::endl;
		std::cout << " • vertexData.size() * sizeof(VertexAttributes): " << vertexData.size() * sizeof(VertexAttributes) << std::endl;

		BufferDescriptor vertexBufferDescriptor(vertexData.size() * sizeof(VertexAttributes), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex, "vertex_buffer");
		Buffer vertexBuffer(device, vertexBufferDescriptor);

		queue->writeBuffer(vertexBuffer.Handle(), 0, vertexData.data(), vertexBufferDescriptor.size);
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

		queue->writeBuffer(uniformBuffer.Handle(), 0, &uniforms, sizeof(MyUniforms));

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

			TextureView textureView = GetNextTexture(device.Handle(), surface.Handle());

			uniforms.time = -static_cast<float>(SDL_GetTicks()) / 1000;
			queue->writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, time), &uniforms.time, sizeof(MyUniforms::time));

			cameraPosition = Math::Vector3(-300.0f * std::cos(uniforms.time), -400.0f, -300.0f * std::sin(uniforms.time));
			uniforms.cameraPosition = cameraPosition;

			//std::cout << "cameraPosition: " << cameraPosition << std::endl;

			queue->writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, cameraPosition), &uniforms.cameraPosition, sizeof(MyUniforms::cameraPosition));

			L = Math::Matrix4x4::LookAt(cameraPosition, Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f));

			uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);
			queue->writeBuffer(uniformBuffer.Handle(), offsetof(MyUniforms, viewMatrix), &uniforms.viewMatrix, sizeof(MyUniforms::viewMatrix));

			CommandEncoderDescriptor commandEncoderDescriptor;
			CommandEncoder commandEncoder(device, commandEncoderDescriptor);

			std::vector<RenderPassColorAttachment> renderPassColorAttachments {};
			renderPassColorAttachments.push_back(RenderPassColorAttachment(textureView));

			RenderPassDepthStencilAttachment renderPassDepthStencilAttachment(depthTextureView);

			QuerySet occlusionQuerySet;

			RenderPassDescriptor renderPassDescriptor(renderPassColorAttachments, renderPassDepthStencilAttachment);
			RenderPassEncoder renderPassEncoder(commandEncoder, renderPassDescriptor);

			renderPassEncoder->setPipeline(renderPipeline.Handle());
			renderPassEncoder->setVertexBuffer(0, vertexBuffer.Handle(), 0, vertexData.size() * sizeof(VertexAttributes));
			renderPassEncoder->setBindGroup(0, bindGroups[0].Handle(), 0, nullptr);
			renderPassEncoder->draw(indexCount, 1, 0, 0);

			renderPassEncoder->end();

			CommandBufferDescriptor commandBufferDescriptor;
			CommandBuffer commandBuffer(commandEncoder);
		
			queue->submit(commandBuffer.Handle());

			renderPassEncoder->release();
			commandEncoder->release();
			commandBuffer->release();

			surface->present();
		}
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	logger.Info("Successfully exited.");
	return EXIT_SUCCESS;
}
