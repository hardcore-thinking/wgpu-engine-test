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
#include <iomanip>
#include <cstdint>

#define WEBGPU_CPP_IMPLEMENTATION
#include <wgpu-native/webgpu.hpp>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <sdl2webgpu.h>

#include <Window.hpp>

#include <Helper/Handles.hpp>
#include <Helper/Descriptors.hpp>
#include <Helper/ParametersStructs.hpp>

#include <Resources/Texture/Texture2D.hpp>
#include <Resources/Texture/Cubemap.hpp>
#include <Resources/Geometry/Geometry.hpp>

#include <Logger.hpp>
#include <Math/Math.hpp>
#include <Math/Matrix3x3.hpp>
#include <Math/Matrix4x4.hpp>
#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <Math/Vector4.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr float const PI = 3.1415926535897932384626433832795f;

constexpr float windowWidth = 1138.0f;
constexpr float windowHeight = 640.0f;

struct MyUniforms {
	Math::Matrix4x4 viewDirectionProjectionInverse {};
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

static wgpu::TextureView GetNextTexture(Window& window, Adapter& adapter, Device& device, CompatibleSurface& surface) {
	wgpu::SurfaceTexture surfaceTexture {};
	surface->getCurrentTexture(&surfaceTexture);

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

		case wgpu::SurfaceGetCurrentTextureStatus::SuccessSuboptimal:
			std::cerr << "Success suboptimal" << std::endl;
			// Note: This can happen if the surface is resized or the window is minimized.
			// You may want to handle this case by reconfiguring the surface.
			surface->unconfigure();
			surface.Configure(adapter, device, window);
			break;

		case wgpu::SurfaceGetCurrentTextureStatus::Timeout:
			std::cerr << "Timeout" << std::endl;
			return nullptr;

		default:
			std::cerr << "Unknown (" << surfaceTexture.status << ")" << std::endl;
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

int main() {
	try {
		// MARK: Main instances
		Instance instance;
		WindowCreationInfo windowCreationInfo {
			.title = "WebGPU Skybox",
			.x = SDL_WINDOWPOS_CENTERED,
			.y = SDL_WINDOWPOS_CENTERED,
			.w = static_cast<int>(windowWidth),
			.h = static_cast<int>(windowHeight),
			.flags = SDL_WINDOW_SHOWN
		};

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Can't initialize SDL2: " << SDL_GetError() << std::endl;
		}

		std::clog << "Current video driver: " << SDL_GetCurrentVideoDriver() << std::endl;
		std::clog << SDL_GetError() << std::endl;

		Window window(windowCreationInfo);
		CompatibleSurface surface(instance, window);
		Adapter adapter(instance, surface);

		adapter.DisplayInfos();

		Limits limits(adapter);
		limits.maxBufferSize = 150000 * sizeof(VertexAttributes);
		limits.maxVertexBufferArrayStride = sizeof(VertexAttributes);

		DeviceLostCallbackInfo deviceLostCallbackInfo(DeviceLostCallback, nullptr, nullptr);
		UncapturedErrorCallbackInfo uncapturedErrorCallbackInfo(UncapturedErrorCallback, nullptr, nullptr);
		DeviceDescriptor deviceDescriptor(adapter, limits, deviceLostCallbackInfo, uncapturedErrorCallbackInfo);
		Device device(adapter, deviceDescriptor);
		Queue queue(device);
		surface.Configure(adapter, device, window);

		std::vector<VertexAttributes> vertexData {};
		std::vector<BindGroupLayout> bindGroupLayouts {};
		std::vector<BindGroup> bindGroups {};

		// MARK: Vertex buffer layout
		std::vector<VertexAttribute> vertexAttributes;
		vertexAttributes.push_back(VertexAttribute(0, wgpu::VertexFormat::Float32x3, offsetof(VertexAttributes, position)));

		std::vector<VertexBufferLayout> vertexBufferLayouts {};
		VertexBufferLayout vertexBufferLayout(sizeof(VertexAttributes), vertexAttributes);
		vertexBufferLayouts.push_back(vertexBufferLayout);

		// MARK: Cube binding layouts
		std::vector<BindGroupLayoutEntry> bindGroupLayoutEntries {};
		bindGroupLayoutEntries.push_back(BufferBindingLayout(0, wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment, wgpu::BufferBindingType::Uniform, sizeof(MyUniforms)));
		bindGroupLayoutEntries.push_back(TextureBindingLayout(1, wgpu::ShaderStage::Fragment, wgpu::TextureSampleType::Float));
		bindGroupLayoutEntries[1].texture.viewDimension = wgpu::TextureViewDimension::Cube;
		bindGroupLayoutEntries.push_back(SamplerBindingLayout(2, wgpu::ShaderStage::Fragment, wgpu::SamplerBindingType::Filtering));

		BindGroupLayoutDescriptor bindGroupLayoutDescriptor(bindGroupLayoutEntries);
		bindGroupLayouts.emplace_back(device, bindGroupLayoutDescriptor);

		// MARK: Cube vertex buffer
		BufferDescriptor vertexBufferDescriptor(vertexData.size() * sizeof(VertexAttributes), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex, "vertex_buffer");
		Buffer vertexBuffer(device, vertexBufferDescriptor);

		//queue->writeBuffer(vertexBuffer.Handle(), 0, vertexData.data(), vertexBufferDescriptor.size);
		//int indexCount = static_cast<int>(vertexData.size());

		// MARK: Cube binding handles
		BufferDescriptor uniformBufferDescriptor(sizeof(MyUniforms), wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform, "uniform_buffer");
		Buffer uniformBuffer(device, uniformBufferDescriptor);

		TextureDescriptor textureDescriptor(
			wgpu::TextureFormat::RGBA8Unorm,
			wgpu::TextureUsage::CopyDst | wgpu::TextureUsage::TextureBinding,
			Extent3D(windowWidth, windowHeight, 6)
		);

		textureDescriptor.size.width = 4096;
		textureDescriptor.size.height = 4096;
		textureDescriptor.size.depthOrArrayLayers = 6;

		TextureViewDescriptor textureViewDescriptor(
			wgpu::TextureAspect::All,
			wgpu::TextureFormat::RGBA8Unorm
		);
		textureViewDescriptor.dimension = wgpu::TextureViewDimension::Cube;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.arrayLayerCount = 6;

		//Texture2D texture("resources/futuristic.png", device, queue, textureDescriptor, textureViewDescriptor);

		Cubemap skyboxCubemap({
			"resources/stars_px.jpg",
			"resources/stars_nx.jpg",
			"resources/stars_py.jpg",
			"resources/stars_ny.jpg",
			"resources/stars_pz.jpg",
			"resources/stars_nz.jpg"
		}, device, queue, textureDescriptor, textureViewDescriptor);

		SamplerDescriptor samplerDescriptor(0.0f, 8.0f);
		Sampler sampler(device, samplerDescriptor);
 
		// MARK: Cube bindings array
		std::vector<BindGroupEntry> bindGroupEntries {};
		bindGroupEntries.push_back(BufferBinding(0, uniformBuffer, sizeof(MyUniforms), 0));
		bindGroupEntries.push_back(TextureBinding(1, skyboxCubemap.View()));
		bindGroupEntries.push_back(SamplerBinding(2, sampler));

		BindGroupDescriptor bindGroupDescriptor(bindGroupLayouts[0], bindGroupEntries);
		bindGroups.emplace_back(device, bindGroupDescriptor);

		// MARK: Cube depth texture
		wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;

		std::vector<wgpu::TextureFormat> viewFormats { depthTextureFormat };
		TextureDescriptor depthTextureDescriptor(depthTextureFormat, wgpu::TextureUsage::RenderAttachment, { static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight), 1 }, viewFormats);
		Texture depthTexture(device, depthTextureDescriptor);
	
		TextureViewDescriptor depthTextureViewDescriptor(wgpu::TextureAspect::DepthOnly, depthTextureFormat);
		depthTextureViewDescriptor.arrayLayerCount = 1;
		TextureView depthTextureView(depthTexture, depthTextureViewDescriptor);

		// MARK: Cube render pipeline
		StencilFaceState stencilBackFaceState;
		StencilFaceState stencilFrontFaceState;
		DepthStencilState depthStencilState(stencilFrontFaceState, stencilBackFaceState, depthTextureFormat);
		depthStencilState.depthCompare = wgpu::CompareFunction::LessEqual;

		PrimitiveState primitiveState;
		primitiveState.cullMode = wgpu::CullMode::None; // for debug purposes, put it to None

		MultisampleState multisampleState;

		ShaderModule skyboxShaderModule(device, "resources/skybox.wgsl");

		std::vector<ConstantEntry> vertexConstantEntries {};
		//VertexState vertexState(wgpu::StringView("vs"), skyboxShaderModule, vertexBufferLayouts, vertexConstantEntries);
		VertexState vertexState(wgpu::StringView("vs"), skyboxShaderModule, {}, {});
	
		BlendComponent colorComponent(wgpu::BlendFactor::SrcAlpha, wgpu::BlendFactor::OneMinusSrcAlpha, wgpu::BlendOperation::Add);
		BlendComponent alphaComponent(wgpu::BlendFactor::Zero, wgpu::BlendFactor::One, wgpu::BlendOperation::Add);
		BlendState blendState(colorComponent, alphaComponent);
		std::vector<ColorTargetState> colorTargetStates {};
		ColorTargetState colorTargetState(adapter, surface, blendState);
		colorTargetStates.push_back(colorTargetState);
		std::vector<ConstantEntry> fragmentConstantEntries {};
		FragmentState fragmentState(wgpu::StringView("fs"), skyboxShaderModule, colorTargetStates, fragmentConstantEntries);
	
		PipelineLayoutDescriptor pipelineLayoutDescriptor(bindGroupLayouts);
		PipelineLayout pipelineLayout(device, pipelineLayoutDescriptor);

		RenderPipelineDescriptor cubeRenderPipelineDescriptor(depthStencilState, primitiveState, multisampleState, vertexState, fragmentState, pipelineLayout);
		RenderPipeline cubeRenderPipeline(device, cubeRenderPipelineDescriptor);

		float angleX = 0.0f;
		float angleZ = 0.0f;
		Math::Matrix4x4 view = Math::Matrix4x4::Identity();

		//float time = 0.0f;

		// MARK: Uniforms initialization
		//Math::Matrix4x4 view = Math::Matrix4x4::LookAt(
		//	cameraPosition,
		//	Math::Vector3( 0.0f,  0.0f, 0.0f),
		//	Math::Vector3( 0.0f,  1.0f, 0.0f)
		//);

		float ratio = windowWidth / windowHeight;
		float vfov = 60.0f * PI / 180.0f;
		float near = 0.1f;
		float far = 1000.0f;
		Math::Matrix4x4 projection = Math::Matrix4x4::Perspective(vfov, ratio, near, far);
		Math::Matrix4x4 viewProjection = projection * view;

		MyUniforms uniforms = {
			.viewDirectionProjectionInverse = Math::Matrix4x4::Transpose(Math::Matrix4x4::Inverse(viewProjection))
		};

		queue->writeBuffer(uniformBuffer.Handle(), 0, &uniforms, sizeof(MyUniforms));

		Uint8 const* keyboard = SDL_GetKeyboardState(nullptr);

		running = true;
		//uint64_t frameCount = 0;

		SDL_WarpMouseInWindow(window.Handle(), static_cast<int>(windowWidth / 2), static_cast<int>(windowHeight / 2));
		SDL_SetRelativeMouseMode(SDL_TRUE);

		double sensitivity = 0.005f; // Adjust sensitivity as needed

		// MARK: Main loop
		SDL_Event event {};
		while (running) {
			queue->onSubmittedWorkDone(wgpu::CallbackMode::AllowSpontaneous, [](WGPUQueueWorkDoneStatus status) {
			static int i = 0;
			i++;
			std::cout << "Job completed: " << i << std::endl;
			});
    		queue->submit(0, nullptr); // Submitting nothing just to check for ongoing asynchronous operations and call their callbacks if needed

			//frameBegin = SDL_GetTicks64();

			//std::cout << "[" << std::setw(20) << frameCount++ << "]\r";

			// MARK: Events handling
			while (SDL_PollEvent(&event) > 0) {
				if (event.type == SDL_QUIT || keyboard[SDL_SCANCODE_ESCAPE]) {
					running = false;
				}

				if (keyboard[SDL_SCANCODE_F11]) {
					if (SDL_GetWindowFlags(window.Handle()) & SDL_WINDOW_FULLSCREEN) {
						SDL_SetWindowFullscreen(window.Handle(), 0);
					} else {
						SDL_SetWindowFullscreen(window.Handle(), SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
				}

				if (event.type == SDL_MOUSEMOTION) {
					if (angleX - event.motion.yrel * sensitivity > PI / 2.0f) {
						angleX = PI / 2.0f;
					}
					
					else if (angleX - event.motion.yrel * sensitivity < -PI / 2.0f) {
						angleX = -PI / 2.0f;
					}
					
					else {
						angleX -= event.motion.yrel * sensitivity; // Adjust sensitivity as needed
					}

					if (angleZ - event.motion.xrel * sensitivity > PI) {
						angleZ -= 2.0f * PI;
					}
					
					else if (angleZ - event.motion.xrel * sensitivity < -PI) {
						angleZ += 2.0f * PI;
					}
					
					else {
						angleZ -= event.motion.xrel * sensitivity; // Adjust sensitivity as needed
					}
				}
			}

			// MARK: Update	
			view = Math::Matrix4x4(Math::Matrix4x4::RotateX(angleX) * Math::Matrix4x4::RotateY(angleZ));

			TextureView textureView = std::move(GetNextTexture(window, adapter, device, surface));

			//time = static_cast<float>(frameBegin) / 1000.0f;

			viewProjection = projection * view;

			uniforms.viewDirectionProjectionInverse = Math::Matrix4x4::Transpose(Math::Matrix4x4::Inverse(viewProjection));
			queue->writeBuffer(uniformBuffer.Handle(), 0, &uniforms, sizeof(MyUniforms));
			
			// MARK: Render
			CommandEncoderDescriptor commandEncoderDescriptor;
			CommandEncoder commandEncoder(device, commandEncoderDescriptor);

			std::vector<RenderPassColorAttachment> renderPassColorAttachments {};
			renderPassColorAttachments.push_back(RenderPassColorAttachment(textureView));

			RenderPassDepthStencilAttachment renderPassDepthStencilAttachment(depthTextureView);

			QuerySet occlusionQuerySet;

			RenderPassDescriptor renderPassDescriptor(renderPassColorAttachments, renderPassDepthStencilAttachment);
			RenderPassEncoder renderPassEncoder(commandEncoder, renderPassDescriptor);

			renderPassEncoder->setPipeline(cubeRenderPipeline.Handle());
			//renderPassEncoder->setVertexBuffer(0, vertexBuffer.Handle(), 0, vertexData.size() * sizeof(VertexAttributes));
			renderPassEncoder->setBindGroup(0, bindGroups[0].Handle(), 0, nullptr);
			renderPassEncoder->draw(3, 1, 0, 0);

			renderPassEncoder->end();

			CommandBufferDescriptor commandBufferDescriptor;
			CommandBuffer commandBuffer(commandEncoder);
		
			queue->submit(commandBuffer.Handle());

			surface->present();
			
			//frameEnd = SDL_GetTicks64();
		}
	}

	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	logger.Info("Successfully exited.");

	SDL_Quit();

	return EXIT_SUCCESS;
}

