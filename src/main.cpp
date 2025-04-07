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

#include <Logger.hpp>
#include <Math.hpp>

constexpr float const PI = 3.1415926535897932384626433832795f;

constexpr float windowWidth = 640.0f;
constexpr float windowHeight = 480.0f;

struct MyUniforms {
	Math::Matrix4x4 projectionMatrix{};
	Math::Matrix4x4 viewMatrix{};
	Math::Matrix4x4 modelMatrix{};
	Math::Vector4 color{};
	Math::Vector3 cameraPosition{};
	float time = 0.0f;
};

static_assert(sizeof(MyUniforms) % 16 == 0, "MyUniforms must be aligned to 16 bytes.");

struct VertexAttributes {
	Math::Vector3 position{};
	Math::Vector3 normal{};
	Math::Vector3 color{};
	Math::Vector2 uv{};
};

Logger logger{};

wgpu::Instance instance = nullptr;
SDL_Window* window = nullptr;
wgpu::Surface surface = nullptr;
wgpu::Adapter adapter = nullptr;
auto DeviceLostCallback = [](WGPUDeviceLostReason reason, char const* message, void* userData) {
	std::cerr << "Lost device (" << reason << "): " << message << std::endl;
	};
wgpu::Device device = nullptr;
auto UncapturedErrorCallback = [](WGPUErrorType errorType, const char* message) {
	std::cerr << "Uncaptured error (" << errorType << "): " << message << std::endl;
	};
std::unique_ptr<wgpu::ErrorCallback> errorCallback = nullptr;
wgpu::Queue queue = nullptr;
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
wgpu::Texture depthTexture = nullptr;
wgpu::TextureView depthTextureView = nullptr;
wgpu::Texture texture = nullptr;
wgpu::Sampler sampler = nullptr;

auto CleanOnExit = [&](int code, std::string message = "", bool error = false) -> int {
	if (error) {
		logger.Error(message);
	}

	else {
		logger.Info(message);
	}

	if (surface != nullptr) {
		surface.unconfigure();
		surface.release();
		surface = nullptr;
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

	if (queue != nullptr) {
		queue.release();
		queue = nullptr;
	}

	if (device != nullptr) {
		device.release();
		device = nullptr;
	}

	if (adapter != nullptr) {
		adapter.release();
		adapter = nullptr;
	}

	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	if (instance != nullptr) {
		instance.release();
		instance = nullptr;
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

	if (depthTextureView != nullptr) {
		depthTextureView.release();
		depthTextureView = nullptr;
	}

	if (depthTexture != nullptr) {
		depthTexture.destroy();
		depthTexture.release();
		depthTexture = nullptr;
	}

	if (texture != nullptr) {
		texture.destroy();
		texture.release();
		texture = nullptr;
	}

	return code;
	};
bool running = false;

static wgpu::BindGroupLayoutEntry CreateBufferBindingLayout(uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::BufferBindingType type, uint32_t size, uint32_t minBindingSize, bool hasDynamicOffset = false, wgpu::ChainedStruct const* nextInChain = nullptr) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.buffer.hasDynamicOffset = hasDynamicOffset;
	bindGroupLayoutEntry.buffer.minBindingSize = minBindingSize;
	bindGroupLayoutEntry.buffer.type = type;

	bindGroupLayoutEntry.buffer.nextInChain = nextInChain;

	return bindGroupLayoutEntry;
}

static wgpu::BindGroupLayoutEntry CreateTextureBindingLayout(uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::TextureSampleType sampleType, wgpu::TextureViewDimension viewDimension, bool multisampled = false, wgpu::ChainedStruct const* nextInChain = nullptr) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.texture.multisampled = multisampled;
	bindGroupLayoutEntry.texture.sampleType = sampleType;
	bindGroupLayoutEntry.texture.viewDimension = viewDimension;

	bindGroupLayoutEntry.texture.nextInChain = nextInChain;
	
	return bindGroupLayoutEntry;
}

static wgpu::BindGroupLayoutEntry CreateSamplerBindingLayout(uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::SamplerBindingType type, wgpu::ChainedStruct const* nextInChain = nullptr) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;
	
	bindGroupLayoutEntry.sampler.type = type;

	bindGroupLayoutEntry.sampler.nextInChain = nextInChain;

	return bindGroupLayoutEntry;
}

static void EditBufferBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::BufferBindingType type, uint32_t size, uint32_t minBindingSize, bool hasDynamicOffset = false, wgpu::ChainedStruct const* nextInChain = nullptr) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.buffer.hasDynamicOffset = hasDynamicOffset;
	bindGroupLayoutEntry.buffer.minBindingSize = minBindingSize;
	bindGroupLayoutEntry.buffer.type = type;

	bindGroupLayoutEntry.buffer.nextInChain = nextInChain;
}

static void EditTextureBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::TextureSampleType sampleType, wgpu::TextureViewDimension viewDimension, bool multisampled = false, wgpu::ChainedStruct const* nextInChain = nullptr) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.texture.multisampled = multisampled;
	bindGroupLayoutEntry.texture.sampleType = sampleType;
	bindGroupLayoutEntry.texture.viewDimension = viewDimension;
	
	bindGroupLayoutEntry.texture.nextInChain = nextInChain;
}

static void EditSamplerBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStageFlags visibility, wgpu::SamplerBindingType type, wgpu::ChainedStruct const* nextInChain = nullptr) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.sampler.type = type;
	
	bindGroupLayoutEntry.sampler.nextInChain = nextInChain;
}

static wgpu::BindGroupEntry CreateBufferBinding(uint32_t binding, wgpu::Buffer& buffer, uint32_t size, uint32_t offset = 0) {
	wgpu::BindGroupEntry bindGroupEntry{};
	
	bindGroupEntry.binding = binding;
	bindGroupEntry.buffer = buffer;
	bindGroupEntry.size = size;
	bindGroupEntry.offset = offset;

	return bindGroupEntry;
}

static wgpu::BindGroupEntry CreateTextureBinding(uint32_t binding, wgpu::TextureView& textureView) {
	wgpu::BindGroupEntry bindGroupEntry{};
	
	bindGroupEntry.binding = binding;
	bindGroupEntry.textureView = textureView;

	return bindGroupEntry;
}

static wgpu::BindGroupEntry CreateSamplerBinding(uint32_t binding, wgpu::Sampler& sampler) {
	wgpu::BindGroupEntry bindGroupEntry{};

	bindGroupEntry.binding = binding;
	bindGroupEntry.sampler = sampler;

	return bindGroupEntry;
}

static void EditBufferBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::Buffer& buffer, uint32_t size, uint32_t offset = 0) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.buffer = buffer;
	bindGroupEntry.size = size;
	bindGroupEntry.offset = offset;
}

static void EditTextureBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::TextureView& textureView) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.textureView = textureView;
}

static void EditSamplerBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::Sampler& sampler) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.sampler = sampler;
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

static uint32_t CeilToNextMultiple(uint32_t value, uint32_t step) {
	uint32_t divideAndCeil = value / step + (value % step == 0 ? 0 : 1);
	return step * divideAndCeil;
}

static bool CreateInstance(wgpu::Instance& instance, wgpu::ChainedStruct* nextInChain = nullptr) {
	wgpu::InstanceDescriptor wgpuInstanceDescriptor = {};
	wgpuInstanceDescriptor.nextInChain = nextInChain;

	instance = wgpu::createInstance(wgpuInstanceDescriptor);
	
	return (instance != nullptr);
}

struct WindowCreationInfo {
	std::string title = "SDL window";
	int x = 0;
	int y = 0;
	int w = 640;
	int h = 480;
	Uint32 flags = SDL_WINDOW_SHOWN;
};

static bool CreateWindow(SDL_Window** window, WindowCreationInfo const& info) {
	*window = SDL_CreateWindow("Stop procrastinating !", info.x, info.y, info.w, info.h, info.flags);

	std::cout << "<SDL_Window " << window << ">" << std::endl;

	return (window != nullptr);
}

static bool RetrieveCompatibleSurface(wgpu::Surface& surface, wgpu::Instance& instance, SDL_Window* window) {
	surface = SDL_GetWGPUSurface(instance, window);
	
	return (surface != nullptr);
}

static void GetAdapterFeatures(wgpu::Adapter& adapter, std::vector<wgpu::FeatureName>& features, bool log = false, Logger* logger = nullptr) {
	if (adapter == nullptr) {
		if (logger != nullptr) {
			logger->Warn("Adapter is invalid.");
		}
		return;
	}

	size_t featuresCount = adapter.enumerateFeatures(nullptr);
	features.resize(featuresCount, wgpu::FeatureName::Undefined);
	featuresCount = adapter.enumerateFeatures(features.data());

	if (log) {
		std::cout << "\t - " << "Features (found " << featuresCount << "):" << std::endl;
		for (auto& feature : features) {
			switch (feature) {
			case wgpu::FeatureName::DepthClipControl:
				std::cout << "\t\t- " << "wgpu::FeatureName::DepthClipControl (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::Depth32FloatStencil8:
				std::cout << "\t\t- " << "wgpu::FeatureName::Depth32FloatStencil8 (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TimestampQuery:
				std::cout << "\t\t- " << "wgpu::FeatureName::TimestampQuery (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionBC:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionBC (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionETC2:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionETC2 (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionASTC:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionASTC (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::IndirectFirstInstance:
				std::cout << "\t\t- " << "wgpu::FeatureName::IndirectFirstInstance (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::ShaderF16:
				std::cout << "\t\t- " << "wgpu::FeatureName::ShaderF16 (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::RG11B10UfloatRenderable:
				std::cout << "\t\t- " << "wgpu::FeatureName::RG11B10UfloatRenderable (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::BGRA8UnormStorage:
				std::cout << "\t\t- " << "wgpu::FeatureName::BGRA8UnormStorage (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::Float32Filterable:
				std::cout << "\t\t- " << "wgpu::FeatureName::Float32Filterable (" << feature << ")" << std::endl;
				break;

			default:
				std::cout << "\t\t- " << "wgpu::FeatureName::Undefined (" << feature << ")" << std::endl;
				break;
			}
		}

		std::cout << std::endl;
	}
}

static void GetAdapterSupportedLimits(wgpu::Adapter& adapter, wgpu::SupportedLimits& supportedLimits, bool log = false, Logger* logger = nullptr) {
	if (adapter == nullptr) {
		if (logger != nullptr) {
			logger->Warn("Adapter is invalid.");
		}
		return;
	}

	wgpu::Bool retrievedLimits = adapter.getLimits(&supportedLimits);

	if (log) {
		if (retrievedLimits) {
			std::cout << "\t- " << "Limits: " << std::endl;

			std::cout << "\t\t- " << "Max bind groups: " << supportedLimits.limits.maxBindGroups << std::endl;
			std::cout << "\t\t- " << "Max bind groups plus vertex buffers: " << supportedLimits.limits.maxBindGroupsPlusVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Max bindings per bind group: " << supportedLimits.limits.maxBindingsPerBindGroup << std::endl;
			std::cout << "\t\t- " << "Max buffer size: " << supportedLimits.limits.maxBufferSize << std::endl;
			std::cout << "\t\t- " << "Max color attachment bytes per sample: " << supportedLimits.limits.maxColorAttachmentBytesPerSample << std::endl;
			std::cout << "\t\t- " << "Max color attachments: " << supportedLimits.limits.maxColorAttachments << std::endl;
			std::cout << "\t\t- " << "Max compute invocations per workgroup: " << supportedLimits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size x: " << supportedLimits.limits.maxComputeWorkgroupSizeX << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size y: " << supportedLimits.limits.maxComputeWorkgroupSizeY << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size z: " << supportedLimits.limits.maxComputeWorkgroupSizeZ << std::endl;
			std::cout << "\t\t- " << "Max compute workgroups per dimension: " << supportedLimits.limits.maxComputeWorkgroupsPerDimension << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup storage size: " << supportedLimits.limits.maxComputeWorkgroupStorageSize << std::endl;
			std::cout << "\t\t- " << "Max dynamic storage buffers per pipeline layout: " << supportedLimits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
			std::cout << "\t\t- " << "Max dynamic uniform buffers per pipeline layout: " << supportedLimits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
			std::cout << "\t\t- " << "Max inter stage shader components: " << supportedLimits.limits.maxInterStageShaderComponents << std::endl;
			std::cout << "\t\t- " << "Max inter stage shader variables: " << supportedLimits.limits.maxInterStageShaderVariables << std::endl;
			std::cout << "\t\t- " << "Max sampled textures per shader stage: " << supportedLimits.limits.maxSampledTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max samplers per shader stage: " << supportedLimits.limits.maxSamplersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage buffer binding size: " << supportedLimits.limits.maxStorageBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max storage buffers per shader stage: " << supportedLimits.limits.maxStorageBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage textures per shader stage: " << supportedLimits.limits.maxStorageTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max texture array layers: " << supportedLimits.limits.maxTextureArrayLayers << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 1D: " << supportedLimits.limits.maxTextureDimension1D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 2D: " << supportedLimits.limits.maxTextureDimension2D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 3D: " << supportedLimits.limits.maxTextureDimension3D << std::endl;
			std::cout << "\t\t- " << "Max uniform buffer binding size: " << supportedLimits.limits.maxUniformBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max uniform buffers per shader stage: " << supportedLimits.limits.maxUniformBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max vertex attributes: " << supportedLimits.limits.maxVertexAttributes << std::endl;
			std::cout << "\t\t- " << "Max vertex buffer array stride: " << supportedLimits.limits.maxVertexBufferArrayStride << std::endl;
			std::cout << "\t\t- " << "Max vertex buffers: " << supportedLimits.limits.maxVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Min storage buffer offset alignment: " << supportedLimits.limits.minStorageBufferOffsetAlignment << std::endl;
			std::cout << "\t\t- " << "Min uniform buffer offset alignment: " << supportedLimits.limits.minUniformBufferOffsetAlignment << std::endl;

			std::cout << std::endl;
		}
	}
}

static void GetAdapterProperties(wgpu::Adapter& adapter, wgpu::AdapterProperties& adapterProperties, bool log = false, Logger* logger = nullptr) {
	if (adapter == nullptr) {
		if (logger != nullptr) {
			logger->Warn("Adapter is invalid.");
		}
		return;
	}

	adapter.getProperties(&adapterProperties);

	if (log) {
		std::cout << "\t- " << "Properties: " << std::endl;

		std::cout << "\t\t" << "Adapter type: ";
		switch (adapterProperties.adapterType) {
		case WGPUAdapterType_DiscreteGPU:
			std::cout << "Discrete GPU" << std::endl;
			break;

		case WGPUAdapterType_IntegratedGPU:
			std::cout << "Integrated GPU" << std::endl;
			break;

		case WGPUAdapterType_CPU:
			std::cout << "CPU" << std::endl;
			break;

		default:
			std::cout << "Unknown" << std::endl;
			break;
		};

		std::cout << "\t\t" << "Architecture: " << adapterProperties.architecture << std::endl;
		std::cout << "\t\t" << "Backend type: ";
		switch (adapterProperties.backendType) {
		case WGPUBackendType_WebGPU:
			std::cout << "WebGPU" << std::endl;
			break;

		case WGPUBackendType_D3D11:
			std::cout << "Direct3D 11" << std::endl;
			break;

		case WGPUBackendType_D3D12:
			std::cout << "Direct3D 12" << std::endl;
			break;

		case WGPUBackendType_Metal:
			std::cout << "Metal" << std::endl;
			break;

		case WGPUBackendType_Vulkan:
			std::cout << "Vulkan" << std::endl;
			break;

		case WGPUBackendType_OpenGL:
			std::cout << "OpenGL" << std::endl;
			break;

		case WGPUBackendType_OpenGLES:
			std::cout << "OpenGLES" << std::endl;
			break;

		default:
			std::cout << "Unknown" << std::endl;
			break;
		}

		std::cout << "\t\t" << "Device ID: " << adapterProperties.deviceID << std::endl;
		std::cout << "\t\t" << "Driver description: " << adapterProperties.driverDescription << std::endl;
		std::cout << "\t\t" << "Name: " << adapterProperties.name << std::endl;
		std::cout << "\t\t" << "Vendor ID: " << adapterProperties.vendorID << std::endl;
		std::cout << "\t\t" << "Vendor name: " << adapterProperties.vendorName << std::endl;
	}
}

static bool RequestAdapter(wgpu::Adapter& adapter, wgpu::Instance& instance, wgpu::RequestAdapterOptions const& options, Logger* logger = nullptr) {
	std::vector<wgpu::Adapter> adapters{};
	wgpu::InstanceEnumerateAdapterOptions enumerateAdapterOptions{};
	enumerateAdapterOptions.nextInChain = nullptr;
	size_t adapterCount = instance.enumerateAdapters(enumerateAdapterOptions, nullptr);

	if (adapterCount == 0) {
		if (logger != nullptr) {
			logger->Error("No adapters found.");
		}
		return false;
	}

	adapters.resize(adapterCount, nullptr);
	logger->Info(std::format("{} adapters found.", adapterCount));

	adapterCount = instance.enumerateAdapters(enumerateAdapterOptions, adapters.data());

	for (auto& adapter : adapters) {
		std::vector<wgpu::FeatureName> features{};
		wgpu::SupportedLimits supportedLimits{};
		wgpu::AdapterProperties properties{};

		GetAdapterFeatures(adapter, features);
		GetAdapterSupportedLimits(adapter, supportedLimits);
		GetAdapterProperties(adapter, properties);
	}

	adapter = instance.requestAdapter(options);

	return (adapter != nullptr);
}

static bool RequestDevice(wgpu::Device& device, wgpu::Adapter& adapter, wgpu::DeviceDescriptor const& deviceDescriptor) {
	device = adapter.requestDevice(deviceDescriptor);

	return (device != nullptr);
}

static void GetDeviceSupportedLimits(wgpu::Device& device, wgpu::SupportedLimits& supportedLimits, bool log = false, Logger* logger = nullptr) {
	if (device == nullptr) {
		if (logger != nullptr) {
			logger->Warn("Device is invalid.");
		}
		return;
	}

	wgpu::Bool retrievedLimits = device.getLimits(&supportedLimits);

	if (log) {
		if (retrievedLimits) {
			std::cout << "\t- " << "Limits: " << std::endl;

			std::cout << "\t\t- " << "Max bind groups: " << supportedLimits.limits.maxBindGroups << std::endl;
			std::cout << "\t\t- " << "Max bind groups plus vertex buffers: " << supportedLimits.limits.maxBindGroupsPlusVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Max bindings per bind group: " << supportedLimits.limits.maxBindingsPerBindGroup << std::endl;
			std::cout << "\t\t- " << "Max buffer size: " << supportedLimits.limits.maxBufferSize << std::endl;
			std::cout << "\t\t- " << "Max color attachment bytes per sample: " << supportedLimits.limits.maxColorAttachmentBytesPerSample << std::endl;
			std::cout << "\t\t- " << "Max color attachments: " << supportedLimits.limits.maxColorAttachments << std::endl;
			std::cout << "\t\t- " << "Max compute invocations per workgroup: " << supportedLimits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size x: " << supportedLimits.limits.maxComputeWorkgroupSizeX << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size y: " << supportedLimits.limits.maxComputeWorkgroupSizeY << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup size z: " << supportedLimits.limits.maxComputeWorkgroupSizeZ << std::endl;
			std::cout << "\t\t- " << "Max compute workgroups per dimension: " << supportedLimits.limits.maxComputeWorkgroupsPerDimension << std::endl;
			std::cout << "\t\t- " << "Max compute workgroup storage size: " << supportedLimits.limits.maxComputeWorkgroupStorageSize << std::endl;
			std::cout << "\t\t- " << "Max dynamic storage buffers per pipeline layout: " << supportedLimits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
			std::cout << "\t\t- " << "Max dynamic uniform buffers per pipeline layout: " << supportedLimits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
			std::cout << "\t\t- " << "Max inter stage shader components: " << supportedLimits.limits.maxInterStageShaderComponents << std::endl;
			std::cout << "\t\t- " << "Max inter stage shader variables: " << supportedLimits.limits.maxInterStageShaderVariables << std::endl;
			std::cout << "\t\t- " << "Max sampled textures per shader stage: " << supportedLimits.limits.maxSampledTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max samplers per shader stage: " << supportedLimits.limits.maxSamplersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage buffer binding size: " << supportedLimits.limits.maxStorageBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max storage buffers per shader stage: " << supportedLimits.limits.maxStorageBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max storage textures per shader stage: " << supportedLimits.limits.maxStorageTexturesPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max texture array layers: " << supportedLimits.limits.maxTextureArrayLayers << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 1D: " << supportedLimits.limits.maxTextureDimension1D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 2D: " << supportedLimits.limits.maxTextureDimension2D << std::endl;
			std::cout << "\t\t- " << "Max texture dimension 3D: " << supportedLimits.limits.maxTextureDimension3D << std::endl;
			std::cout << "\t\t- " << "Max uniform buffer binding size: " << supportedLimits.limits.maxUniformBufferBindingSize << std::endl;
			std::cout << "\t\t- " << "Max uniform buffers per shader stage: " << supportedLimits.limits.maxUniformBuffersPerShaderStage << std::endl;
			std::cout << "\t\t- " << "Max vertex attributes: " << supportedLimits.limits.maxVertexAttributes << std::endl;
			std::cout << "\t\t- " << "Max vertex buffer array stride: " << supportedLimits.limits.maxVertexBufferArrayStride << std::endl;
			std::cout << "\t\t- " << "Max vertex buffers: " << supportedLimits.limits.maxVertexBuffers << std::endl;
			std::cout << "\t\t- " << "Min storage buffer offset alignment: " << supportedLimits.limits.minStorageBufferOffsetAlignment << std::endl;
			std::cout << "\t\t- " << "Min uniform buffer offset alignment: " << supportedLimits.limits.minUniformBufferOffsetAlignment << std::endl;

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

	case wgpu::SurfaceGetCurrentTextureStatus::Success:
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
	textureViewDescriptor.label = "current_surface_texture_view";
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

	wgpu::ShaderModuleWGSLDescriptor shaderCodeDesc{};
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = wgpu::SType::ShaderModuleWGSLDescriptor;
	shaderCodeDesc.code = shaderSource.c_str();

	wgpu::ShaderModuleDescriptor shaderDesc{};
	
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;
	
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

static int InitInstance() {
	logger.Info("Creating instance...");

	if (!CreateInstance(instance)) {
		logger.Error("Failed to create WebGPU instance.");
		return EXIT_FAILURE;
	}

	logger.Info("Instance created.");

	return EXIT_SUCCESS;
}

static int InitWindow() {
	logger.Info("Creating window...");

	WindowCreationInfo windowCreationInfo{};
	windowCreationInfo.title = "Stop procrastinating !";
	windowCreationInfo.x = SDL_WINDOWPOS_CENTERED;
	windowCreationInfo.y = SDL_WINDOWPOS_CENTERED;
	windowCreationInfo.w = windowWidth;
	windowCreationInfo.h = windowHeight;
	windowCreationInfo.flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

	if (!CreateWindow(&window, windowCreationInfo)) {
		logger.Error("Failed to create window.");
		return EXIT_FAILURE;
	}

	std::cout << "<SDL_Window " << window << ">" << std::endl;

	logger.Info("Window created.");

	return EXIT_SUCCESS;
}

static int InitCompatibleSurface() {
	logger.Info("Retrieving compatible surface...");

	if (!RetrieveCompatibleSurface(surface, instance, window)) {
		logger.Error("Failed to create surface.");
		return EXIT_FAILURE;
	}

	logger.Info("Compatible surface retrieved.");

	return EXIT_SUCCESS;
}

static int InitAdapter() {
	logger.Info("Creating adapter...");

	wgpu::RequestAdapterOptions requestAdapterOptions{};
	requestAdapterOptions.compatibleSurface = surface;
	requestAdapterOptions.powerPreference = wgpu::PowerPreference::HighPerformance;
	requestAdapterOptions.nextInChain = nullptr;
	requestAdapterOptions.forceFallbackAdapter = false;

	if (!RequestAdapter(adapter, instance, requestAdapterOptions, &logger)) {
		logger.Error("Failed to create adapter.");
		return EXIT_FAILURE;
	}

	logger.Info("Adapter created.");

	return EXIT_SUCCESS;
}

static int InitDevice() {
	logger.Info("Creating device...");

	wgpu::QueueDescriptor defaultQueueDescriptor{};
	defaultQueueDescriptor.label = "default_queue";
	defaultQueueDescriptor.nextInChain = nullptr;

	std::vector<wgpu::FeatureName> requiredFeatures{};

	wgpu::SupportedLimits adapterSupportedLimits{};
	GetAdapterSupportedLimits(adapter, adapterSupportedLimits, true);

	wgpu::RequiredLimits requiredLimits{};
	requiredLimits.limits.maxTextureDimension1D = windowHeight;
	requiredLimits.limits.maxTextureDimension2D = windowWidth;
	requiredLimits.limits.maxTextureArrayLayers = 1;

	requiredLimits.limits.maxVertexAttributes = 4;
	requiredLimits.limits.maxVertexBuffers = 1;
	requiredLimits.limits.maxBufferSize = 1000000 * sizeof(VertexAttributes);
	requiredLimits.limits.maxVertexBufferArrayStride = sizeof(VertexAttributes);
	requiredLimits.limits.maxInterStageShaderComponents = 8;
	requiredLimits.limits.maxBindGroups = 1;
	requiredLimits.limits.maxUniformBuffersPerShaderStage = 1;
	requiredLimits.limits.maxUniformBufferBindingSize = static_cast<uint64_t>(16ULL * 4 * sizeof(float));
	requiredLimits.limits.maxDynamicUniformBuffersPerPipelineLayout = 1;
	requiredLimits.limits.maxSampledTexturesPerShaderStage = 1;
	requiredLimits.limits.maxBindingsPerBindGroup = 2;
	requiredLimits.limits.maxSamplersPerShaderStage = 1;

	requiredLimits.limits.minStorageBufferOffsetAlignment = adapterSupportedLimits.limits.minStorageBufferOffsetAlignment;
	requiredLimits.limits.minUniformBufferOffsetAlignment = adapterSupportedLimits.limits.minUniformBufferOffsetAlignment;
	requiredLimits.nextInChain = nullptr;

	wgpu::DeviceDescriptor deviceDescriptor{};
	deviceDescriptor.defaultQueue = defaultQueueDescriptor;
	deviceDescriptor.deviceLostCallback = (WGPUDeviceLostCallback)(DeviceLostCallback);
	deviceDescriptor.deviceLostUserdata = nullptr;
	deviceDescriptor.label = "device";
	deviceDescriptor.nextInChain = nullptr;
	deviceDescriptor.requiredFeatureCount = requiredFeatures.size();
	deviceDescriptor.requiredFeatures = (WGPUFeatureName*)(requiredFeatures.data());
	deviceDescriptor.requiredLimits = &requiredLimits;

	if (!RequestDevice(device, adapter, deviceDescriptor)) {
		logger.Error("Failed to create device.");
		return EXIT_FAILURE;
	}

	wgpu::SupportedLimits deviceSupportedLimits{};
	GetDeviceSupportedLimits(device, deviceSupportedLimits, true);

	uniformStride = CeilToNextMultiple(static_cast<uint32_t>(sizeof(MyUniforms)),
		static_cast<uint32_t>(deviceSupportedLimits.limits.minUniformBufferOffsetAlignment));

	errorCallback = device.setUncapturedErrorCallback(UncapturedErrorCallback);
	if (errorCallback == nullptr) {
		logger.Error("Failed to set uncaptured device error callback.");
		return EXIT_FAILURE;
	}

	logger.Info("Device created.");

	return EXIT_SUCCESS;
}

static void ConfigureSurface() {
	logger.Info("Configuring compatible surface...");

	std::vector<wgpu::TextureFormat> textureFormats{};

	wgpu::SurfaceConfiguration surfaceConfiguration{};
	surfaceConfiguration.alphaMode = wgpu::CompositeAlphaMode::Auto;
	surfaceConfiguration.device = device;
	surfaceConfiguration.format = surface.getPreferredFormat(adapter);
	surfaceConfiguration.height = windowHeight;
	surfaceConfiguration.nextInChain = nullptr;
	surfaceConfiguration.presentMode = wgpu::PresentMode::Fifo;
	surfaceConfiguration.usage = wgpu::TextureUsage::RenderAttachment;
	surfaceConfiguration.viewFormatCount = textureFormats.size();
	surfaceConfiguration.viewFormats = (WGPUTextureFormat*)(textureFormats.data());
	surfaceConfiguration.width = windowWidth;
	surface.configure(surfaceConfiguration);

	logger.Info("Compatible surface configured.");
}

static int InitQueue() {
	logger.Info("Retrieving queue...");

	queue = device.getQueue();
	if (queue == nullptr) {
		logger.Error("Failed to create queue.");
		return EXIT_FAILURE;
	}

	logger.Info("Queue retrieved.");

	return EXIT_SUCCESS;
}

static int InitRenderPipeline(std::vector<wgpu::VertexBufferLayout> const& vertexBufferLayouts, std::vector<wgpu::BindGroupLayoutEntry> const& bindGroupLayoutEntries, std::vector<wgpu::BindGroupEntry> const& bindGroupEntries) {
	logger.Info("Creating render pipeline...");

	wgpu::StencilFaceState stencilBackFaceState {};
	stencilBackFaceState.compare = wgpu::CompareFunction::Always;
	stencilBackFaceState.depthFailOp = wgpu::StencilOperation::Keep;
	stencilBackFaceState.failOp = wgpu::StencilOperation::Keep;
	stencilBackFaceState.passOp = wgpu::StencilOperation::Keep;

	wgpu::StencilFaceState stencilFrontFaceState {};
	stencilFrontFaceState.compare = wgpu::CompareFunction::Always;
	stencilFrontFaceState.depthFailOp = wgpu::StencilOperation::Keep;
	stencilFrontFaceState.failOp = wgpu::StencilOperation::Keep;
	stencilFrontFaceState.passOp = wgpu::StencilOperation::Keep;

	wgpu::TextureFormat depthTextureFormat = wgpu::TextureFormat::Depth24Plus;

	wgpu::DepthStencilState depthStencilState {};
	depthStencilState.depthBias = 0;
	depthStencilState.depthBiasClamp = 0.0f;
	depthStencilState.depthBiasSlopeScale = 0.0f;
	depthStencilState.depthCompare = wgpu::CompareFunction::Less;
	depthStencilState.depthWriteEnabled = true;
	depthStencilState.format = depthTextureFormat;
	depthStencilState.nextInChain = nullptr;
	depthStencilState.stencilBack = stencilBackFaceState;
	depthStencilState.stencilFront = stencilFrontFaceState;
	depthStencilState.stencilReadMask = 0;
	depthStencilState.stencilWriteMask = 0;

	wgpu::TextureDescriptor depthTextureDescriptor {};
	depthTextureDescriptor.dimension = wgpu::TextureDimension::_2D;
	depthTextureDescriptor.format = depthTextureFormat;
	depthTextureDescriptor.mipLevelCount = 1;
	depthTextureDescriptor.sampleCount = 1;
	depthTextureDescriptor.size = { static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight), 1 };
	depthTextureDescriptor.usage = wgpu::TextureUsage::RenderAttachment;
	depthTextureDescriptor.viewFormatCount = 1;
	depthTextureDescriptor.viewFormats = (WGPUTextureFormat*)(&depthTextureFormat);
	depthTexture = device.createTexture(depthTextureDescriptor);
	if (depthTexture == nullptr) {
		logger.Error("Failed to create depth texture.");
		return EXIT_FAILURE;
	}

	wgpu::TextureViewDescriptor depthTextureViewDescriptor {};
	depthTextureViewDescriptor.aspect = wgpu::TextureAspect::DepthOnly;
	depthTextureViewDescriptor.baseArrayLayer = 0;
	depthTextureViewDescriptor.arrayLayerCount = 1;
	depthTextureViewDescriptor.baseMipLevel = 0;
	depthTextureViewDescriptor.mipLevelCount = 1;
	depthTextureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
	depthTextureViewDescriptor.format = depthTextureFormat;
	depthTextureView = depthTexture.createView(depthTextureViewDescriptor);
	if (depthTextureView == nullptr) {
		logger.Error("Failed to create depth texture view.");
		return EXIT_FAILURE;
	}

	std::vector<wgpu::ConstantEntry> fragmentConstantEntries {};
	std::vector<wgpu::ShaderModuleCompilationHint> fragmentShaderModuleCompilationHints {};

	wgpu::ChainedStruct fragmentShaderModuleChainedStruct {};
	fragmentShaderModuleChainedStruct.next = nullptr;
	fragmentShaderModuleChainedStruct.sType = wgpu::SType::ShaderModuleWGSLDescriptor;

	fragmentShaderModule = LoadShaderModule("resources/shader.wgsl", device);
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
	colorTargetStates[0].format = surface.getPreferredFormat(adapter);
	colorTargetStates[0].nextInChain = nullptr;
	colorTargetStates[0].writeMask = wgpu::ColorWriteMask::All;

	wgpu::FragmentState fragmentState {};
	fragmentState.constantCount = fragmentConstantEntries.size();
	fragmentState.constants = fragmentConstantEntries.data();
	fragmentState.entryPoint = "frag_main";
	fragmentState.module = fragmentShaderModule;
	fragmentState.nextInChain = nullptr;
	fragmentState.targetCount = colorTargetStates.size();
	fragmentState.targets = colorTargetStates.data();

	wgpu::BindGroupLayoutDescriptor bindGroupLayoutDescriptor {};
	bindGroupLayoutDescriptor.entryCount = bindGroupLayoutEntries.size();
	bindGroupLayoutDescriptor.entries = bindGroupLayoutEntries.data();
	bindGroupLayouts.push_back(device.createBindGroupLayout(bindGroupLayoutDescriptor));
	if (bindGroupLayouts[0] == nullptr) {
		logger.Error("Failed to create bind group layout.");
		return EXIT_FAILURE;
	}
	
	wgpu::BindGroupDescriptor bindGroupDescriptor {};
	bindGroupDescriptor.layout = bindGroupLayouts[0];
	bindGroupDescriptor.entryCount = (uint32_t)(bindGroupEntries.size());
	bindGroupDescriptor.entries = bindGroupEntries.data();
	bindGroups.push_back(device.createBindGroup(bindGroupDescriptor));
	if (bindGroups[0] == nullptr) {
		logger.Error("Failed to create bind group.");
		return EXIT_FAILURE;
	}

	wgpu::PipelineLayoutDescriptor pipelineLayoutDescriptor {};
	pipelineLayoutDescriptor.bindGroupLayoutCount = bindGroupLayouts.size();
	pipelineLayoutDescriptor.bindGroupLayouts = (WGPUBindGroupLayout*)(bindGroupLayouts.data());
	pipelineLayoutDescriptor.label = "pipeline_layout";
	pipelineLayoutDescriptor.nextInChain = nullptr;
	pipelineLayout = device.createPipelineLayout(pipelineLayoutDescriptor);
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
	std::vector<wgpu::ShaderModuleCompilationHint> vertexShaderModuleCompilationHints {};

	vertexShaderModule = LoadShaderModule("resources/shader.wgsl", device);
	if (vertexShaderModule == nullptr) {
		logger.Error("Failed to create vertex shader module.");
		return EXIT_FAILURE;
	}

	wgpu::VertexState vertexState {};
	vertexState.bufferCount = vertexBufferLayouts.size();
	vertexState.buffers = vertexBufferLayouts.data();
	vertexState.constantCount = vertexConstantEntries.size();
	vertexState.constants = vertexConstantEntries.data();
	vertexState.entryPoint = "vert_main";
	vertexState.module = vertexShaderModule;

	wgpu::RenderPipelineDescriptor renderPipelineDescriptor {};
	renderPipelineDescriptor.depthStencil = &depthStencilState;
	renderPipelineDescriptor.fragment = &fragmentState;
	renderPipelineDescriptor.label = "render_pipeline";
	renderPipelineDescriptor.layout = pipelineLayout;
	renderPipelineDescriptor.multisample = multisampleState;
	renderPipelineDescriptor.nextInChain = nullptr;
	renderPipelineDescriptor.primitive = primitiveState;
	renderPipelineDescriptor.vertex = vertexState;

	if (!CreateRenderPipeline(renderPipeline, device, renderPipelineDescriptor)) {
		logger.Error("Failed to create render pipeline.");
		return EXIT_FAILURE;
	}

	logger.Info("Render pipeline created.");

	return EXIT_SUCCESS;
}

int main() {
	// initialiazion of the application and assets
	{
		if (InitInstance() == EXIT_FAILURE)          return CleanOnExit(EXIT_FAILURE, "Failed to create instance.", true);
		if (InitWindow() == EXIT_FAILURE)            return CleanOnExit(EXIT_FAILURE, "Failed to create window.", true);
		if (InitCompatibleSurface() == EXIT_FAILURE) return CleanOnExit(EXIT_FAILURE, "Failed to create compatible surface.", true);
		if (InitAdapter() == EXIT_FAILURE)           return CleanOnExit(EXIT_FAILURE, "Failed to create adapter.", true);
		if (InitDevice() == EXIT_FAILURE)            return CleanOnExit(EXIT_FAILURE, "Failed to create device.", true);
		ConfigureSurface();
		if (InitQueue() == EXIT_FAILURE)             return CleanOnExit(EXIT_FAILURE, "Failed to create queue.", true);

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

		wgpu::BufferDescriptor bufferDescriptor{};
		bufferDescriptor.label = "uniform_buffer";
		bufferDescriptor.size = sizeof(MyUniforms);
		bufferDescriptor.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
		bufferDescriptor.mappedAtCreation = false;
		uniformBuffer = device.createBuffer(bufferDescriptor);
		if (uniformBuffer == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create uniform buffer.", true);
		}

		wgpu::TextureDescriptor textureDescriptor{};
		textureDescriptor.dimension = wgpu::TextureDimension::_2D;
		textureDescriptor.size = { 256, 256, 1 };
		textureDescriptor.mipLevelCount = 8;
		textureDescriptor.sampleCount = 1;
		textureDescriptor.format = wgpu::TextureFormat::RGBA8Unorm;
		textureDescriptor.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
		textureDescriptor.viewFormatCount = 0;
		textureDescriptor.viewFormats = nullptr;
		texture = device.createTexture(textureDescriptor);
		if (texture == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create texture.", true);
		}

		wgpu::ImageCopyTexture destination{};
		destination.texture = texture;
		destination.origin = { 0, 0, 0 };
		destination.aspect = wgpu::TextureAspect::All;

		wgpu::TextureDataLayout source{};
		source.offset = 0;

		wgpu::Extent3D mipLevelSize = textureDescriptor.size;
		std::vector<uint8_t> previousLevelPixels;
		for (uint32_t level = 0; level < textureDescriptor.mipLevelCount; ++level) {
			std::vector<uint8_t> pixels(4 * mipLevelSize.width * mipLevelSize.height);
			for (uint32_t i = 0; i < mipLevelSize.width; ++i) {
				for (uint32_t j = 0; j < mipLevelSize.height; ++j) {
					uint8_t* p = &pixels[4 * (j * mipLevelSize.width + i)];
					if (level == 0) {
						// Our initial texture formula
						p[0] = (i / 16) % 2 == (j / 16) % 2 ? 255 : 0; // r
						p[1] = ((i - j) / 16) % 2 == 0 ? 255 : 0; // g
						p[2] = ((i + j) / 16) % 2 == 0 ? 255 : 0; // b
					}

					else {
						// Get the corresponding 4 pixels from the previous level
						uint8_t* p00 = &previousLevelPixels[4 * ((2 * j + 0) * (2 * mipLevelSize.width) + (2 * i + 0))];
						uint8_t* p01 = &previousLevelPixels[4 * ((2 * j + 0) * (2 * mipLevelSize.width) + (2 * i + 1))];
						uint8_t* p10 = &previousLevelPixels[4 * ((2 * j + 1) * (2 * mipLevelSize.width) + (2 * i + 0))];
						uint8_t* p11 = &previousLevelPixels[4 * ((2 * j + 1) * (2 * mipLevelSize.width) + (2 * i + 1))];

						// Average
						p[0] = (p00[0] + p01[0] + p10[0] + p11[0]) / 4;
						p[1] = (p00[1] + p01[1] + p10[1] + p11[1]) / 4;
						p[2] = (p00[2] + p01[2] + p10[2] + p11[2]) / 4;
					}

					/*
					else {
						// Some debug value for visualizing mip levels
						p[0] = level % 2 == 0 ? 255 : 0;
						p[1] = (level / 2) % 2 == 0 ? 255 : 0;
						p[2] = (level / 4) % 2 == 0 ? 255 : 0;
					}
					*/

					p[3] = 255; // a
				}
			}

			destination.mipLevel = level;

			source.bytesPerRow = 4 * mipLevelSize.width;
			source.rowsPerImage = mipLevelSize.height;

			queue.writeTexture(destination, pixels.data(), pixels.size(), source, mipLevelSize);

			mipLevelSize.width /= 2;
			mipLevelSize.height /= 2;

			previousLevelPixels = std::move(pixels);
		}

		wgpu::TextureViewDescriptor textureViewDescriptor{};
		textureViewDescriptor.aspect = wgpu::TextureAspect::All;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.mipLevelCount = textureDescriptor.mipLevelCount;
		textureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
		textureViewDescriptor.format = textureDescriptor.format;
		wgpu::TextureView textureView = texture.createView(textureViewDescriptor);
		if (textureView == nullptr) {
			logger.Error("Failed to create texture view.");
			return EXIT_FAILURE;
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
		sampler = device.createSampler(samplerDescriptor);

		std::vector<wgpu::BindGroupEntry> bindGroupEntries(3, wgpu::Default);
		EditBufferBinding(bindGroupEntries[0], 0, uniformBuffer, sizeof(MyUniforms));
		EditTextureBinding(bindGroupEntries[1], 1, textureView);
		EditSamplerBinding(bindGroupEntries[2], 2, sampler);

		if (InitRenderPipeline(vertexBufferLayouts, bindGroupLayoutEntries, bindGroupEntries) == EXIT_FAILURE)    return CleanOnExit(EXIT_FAILURE, "Failed to create render pipeline.", true);
	}

	std::vector<VertexAttributes> vertexData; /* = {
		-0.5, -0.5, 1.0, 0.0, 0.0,
		+0.5, -0.5, 0.0, 1.0, 0.0,
		+0.5, +0.5, 0.0, 0.0, 1.0,
		-0.5, +0.5, 1.0, 1.0, 1.0
	};
	*/

	bool success = LoadGeometryFromOBJ("resources/cube.obj", vertexData);
	if (!success) {
		return CleanOnExit(EXIT_FAILURE, "Failed to load geometry.", true);
	}

	wgpu::BufferDescriptor vertexBufferDescriptor {};
	vertexBufferDescriptor.label = "vertex_buffer";
	vertexBufferDescriptor.size = vertexData.size() * sizeof(VertexAttributes);
	vertexBufferDescriptor.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
	vertexBufferDescriptor.mappedAtCreation = false;
	vertexBuffer = device.createBuffer(vertexBufferDescriptor);
	if (vertexBuffer == nullptr) {
		return CleanOnExit(EXIT_FAILURE, "Failed to create vertex buffer.", true);
	}

	queue.writeBuffer(vertexBuffer, 0, vertexData.data(), vertexBufferDescriptor.size);

	int indexCount = static_cast<int>(vertexData.size());

	MyUniforms uniforms {};
	
	Math::Vector3 cameraPosition(0.0);
	uniforms.cameraPosition = cameraPosition;

	Math::Matrix4x4 S = Math::Matrix4x4::Scale(100.0f);

	uniforms.modelMatrix = Math::Matrix4x4::Transpose(S);
	
	Math::Matrix4x4 L = Math::Matrix4x4::LookAt(
		Math::Vector3(-2.0f, -3.0f, 2.0f), // eye
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

	running = true;
	SDL_Event event {};
	while (running) {
		if (SDL_PollEvent(&event) > 0) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		wgpu::TextureView textureView = GetNextTexture(device, surface);
		if (textureView == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to get texture view.", true);
		}

		uniforms.time = -static_cast<float>(SDL_GetTicks()) / 1000;
		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, time), &uniforms.time, sizeof(MyUniforms::time));

		//R1 = Matrix4x4::RotateZ(uniforms.time);
		//uniforms.modelMatrix = Matrix4x4::Transpose(S);

		/*
		uniforms.modelMatrix = Matrix4x4::Transpose(S);
		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, modelMatrix), &uniforms.modelMatrix, sizeof(MyUniforms::modelMatrix));
		*/

		cameraPosition = Math::Vector3(-300.0f * std::cosf(uniforms.time), -400.0f, -300.0f * std::sinf(uniforms.time));
		uniforms.cameraPosition = cameraPosition;

		std::cout << "cameraPosition: " << cameraPosition << std::endl;

		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, cameraPosition), &uniforms.cameraPosition, sizeof(MyUniforms::cameraPosition));

		L = Math::Matrix4x4::LookAt(
			cameraPosition, // eye
			Math::Vector3(0.0f, 0.0f, 0.0f), // target
			Math::Vector3(0.0f, 0.0f, 1.0f)  // up
		);

		uniforms.viewMatrix = Math::Matrix4x4::Transpose(L);
		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, viewMatrix), &uniforms.viewMatrix, sizeof(MyUniforms::viewMatrix));

		/*
		float viewZ = Mix(0.0f, 0.25f, std::cos(2 * PI * uniforms.time / 4) * 0.5 + 0.5);
		Matrix4x4 L2 = Matrix4x4::LookAt(Vector3(-0.5f, -1.5f, viewZ + 0.25f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
		uniforms.viewMatrix = Matrix4x4::Transpose(L2);
		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, viewMatrix), &uniforms.viewMatrix, sizeof(MyUniforms::viewMatrix));
		*/

		wgpu::CommandEncoderDescriptor commandEncoderDescriptor {};
		commandEncoderDescriptor.label = "command_encoder";
		commandEncoderDescriptor.nextInChain = nullptr;
		
		wgpu::CommandEncoder commandEncoder = device.createCommandEncoder(commandEncoderDescriptor);
		if (commandEncoder == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create command encoder.", true);
		}

		std::vector<wgpu::RenderPassColorAttachment> renderPassColorAttachments {};
		renderPassColorAttachments.resize(1, wgpu::RenderPassColorAttachment{});
		renderPassColorAttachments[0].loadOp = wgpu::LoadOp::Clear;
		renderPassColorAttachments[0].resolveTarget = nullptr;
		renderPassColorAttachments[0].storeOp = wgpu::StoreOp::Store;
		renderPassColorAttachments[0].view = textureView;
		renderPassColorAttachments[0].clearValue = wgpu::Color{ 0.05f, 0.05f, 0.05f, 1.0f };

		std::vector<wgpu::RenderPassDepthStencilAttachment> renderPassDepthStencilAttachments {};
		renderPassDepthStencilAttachments.resize(1, wgpu::RenderPassDepthStencilAttachment {});

		renderPassDepthStencilAttachments[0].view = depthTextureView;
		renderPassDepthStencilAttachments[0].depthClearValue = 1.0f;
		renderPassDepthStencilAttachments[0].depthLoadOp = wgpu::LoadOp::Clear;
		renderPassDepthStencilAttachments[0].depthStoreOp = wgpu::StoreOp::Store;
		renderPassDepthStencilAttachments[0].depthReadOnly = false;
		renderPassDepthStencilAttachments[0].stencilClearValue = 0;
		renderPassDepthStencilAttachments[0].stencilLoadOp = wgpu::LoadOp::Clear;
		renderPassDepthStencilAttachments[0].stencilStoreOp = wgpu::StoreOp::Store;
		renderPassDepthStencilAttachments[0].stencilReadOnly = true;

		wgpu::QuerySet occlusionQuerySet {};

		wgpu::RenderPassDescriptor renderPassDescriptor {};
		renderPassDescriptor.colorAttachmentCount = renderPassColorAttachments.size();
		renderPassDescriptor.colorAttachments = renderPassColorAttachments.data();
		renderPassDescriptor.depthStencilAttachment = &renderPassDepthStencilAttachments[0];
		renderPassDescriptor.label = "render_pass";
		renderPassDescriptor.nextInChain = nullptr;
		renderPassDescriptor.occlusionQuerySet = occlusionQuerySet;
		renderPassDescriptor.timestampWrites = nullptr;

		wgpu::RenderPassEncoder renderPassEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);
		if (renderPassEncoder == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create render pass encoder.", true);
		}

		renderPassEncoder.setPipeline(renderPipeline);
		renderPassEncoder.setVertexBuffer(0, vertexBuffer, 0, vertexData.size() * sizeof(VertexAttributes));

		renderPassEncoder.setBindGroup(0, bindGroups[0], 0, nullptr);
		renderPassEncoder.draw(indexCount, 1, 0, 0);
		
		renderPassEncoder.end();

		wgpu::CommandBufferDescriptor commandBufferDescriptor {};
		commandBufferDescriptor.label = "command_buffer";
		commandBufferDescriptor.nextInChain = nullptr;
		wgpu::CommandBuffer commandBuffer = commandEncoder.finish();
		
		queue.submit(commandBuffer);

		renderPassEncoder.release();
		commandEncoder.release();
		commandBuffer.release();

		surface.present();		
	}

	return CleanOnExit(EXIT_SUCCESS, "Successfully exited.");
}
