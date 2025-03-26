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

uint32_t CeilToNextMultiple(uint32_t value, uint32_t step) {
	uint32_t divideAndCeil = value / step + (value % step == 0 ? 0 : 1);
	return step * divideAndCeil;
}

struct MyUniforms {
	std::array<float, 4> color {};
	float time = 0.0f;
	float _pad[3];
};

class Logger {
public:
	Logger() = default;
	~Logger() = default;

	void Info(std::string message) const {
		std::cout << "INFO : " << message << std::endl;
	};

	void Warn(std::string message) const {
		std::cout << "WARNING: " << message << std::endl;
	};

	void Error(std::string message) const {
		std::cerr << "ERROR: " << message << std::endl;
	}
};

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

	std::cout << "Surface texture: ";
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
		std::cout << "Success" << std::endl;
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

int main() {
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
	std::vector<wgpu::BindGroupLayout> bindGroupLayouts {};
	std::vector<wgpu::BindGroup> bindGroups {};
	uint32_t uniformStride = 0;
	wgpu::Texture depthTexture = nullptr;
	wgpu::TextureView depthTextureView = nullptr;

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

			return code;
		};
	bool running = false;

	// creating the WebGPU instance
	{
		logger.Info("Creating instance...");

		if (!CreateInstance(instance)) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create WebGPU instance.", true);
		}

		logger.Info("Instance created.");
	}

	// creating window
	{
		logger.Info("Creating window...");

		WindowCreationInfo windowCreationInfo {};
		windowCreationInfo.title = "Stop procrastinating !";
		windowCreationInfo.x = SDL_WINDOWPOS_CENTERED;
		windowCreationInfo.y = SDL_WINDOWPOS_CENTERED;
		windowCreationInfo.w = 800;
		windowCreationInfo.h = 600;
		windowCreationInfo.flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

		if (!CreateWindow(&window, windowCreationInfo)) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create window.", true);
		}

		std::cout << "<SDL_Window " << window << ">" << std::endl;

		logger.Info("Window created.");
	}

	// retrieving compatible surface
	{
		logger.Info("Retrieving compatible surface...");

		if (!RetrieveCompatibleSurface(surface, instance, window)) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create surface.", true);
		}

		logger.Info("Compatible surface retrieved.");
	}

	// requesting the correct adapter
	{
		logger.Info("Creating adapter...");

		wgpu::RequestAdapterOptions requestAdapterOptions{};
		requestAdapterOptions.compatibleSurface = surface;
		requestAdapterOptions.powerPreference = wgpu::PowerPreference::HighPerformance;
		requestAdapterOptions.nextInChain = nullptr;
		requestAdapterOptions.forceFallbackAdapter = false;

		if (!RequestAdapter(adapter, instance, requestAdapterOptions, &logger)) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create adapter.", true);
		}

		logger.Info("Adapter created.");
	}

	// request the device
	{
		logger.Info("Creating device...");

		wgpu::QueueDescriptor defaultQueueDescriptor {};
		defaultQueueDescriptor.label = "default_queue";
		defaultQueueDescriptor.nextInChain = nullptr;

		std::vector<wgpu::FeatureName> requiredFeatures {};

		wgpu::SupportedLimits adapterSupportedLimits {};
		GetAdapterSupportedLimits(adapter, adapterSupportedLimits, true);

		wgpu::RequiredLimits requiredLimits {};
		requiredLimits.limits.maxTextureDimension1D = 600;
		requiredLimits.limits.maxTextureDimension2D = 800;
		requiredLimits.limits.maxTextureArrayLayers = 1;

		requiredLimits.limits.maxVertexAttributes = 2;
		requiredLimits.limits.maxVertexBuffers = 1;
		requiredLimits.limits.maxBufferSize = 15 * 5 * sizeof(float);
		requiredLimits.limits.maxVertexBufferArrayStride = 6 * sizeof(float);
		requiredLimits.limits.maxInterStageShaderComponents = 5 * sizeof(float);
		requiredLimits.limits.maxBindGroups = 1;
		requiredLimits.limits.maxUniformBuffersPerShaderStage = 1;
		requiredLimits.limits.maxUniformBufferBindingSize = 16 * 4;
		requiredLimits.limits.maxDynamicUniformBuffersPerPipelineLayout = 1;

		requiredLimits.limits.minStorageBufferOffsetAlignment = adapterSupportedLimits.limits.minStorageBufferOffsetAlignment;
		requiredLimits.limits.minUniformBufferOffsetAlignment = adapterSupportedLimits.limits.minUniformBufferOffsetAlignment;
		requiredLimits.nextInChain = nullptr;

		wgpu::DeviceDescriptor deviceDescriptor {};
		deviceDescriptor.defaultQueue = defaultQueueDescriptor;
		deviceDescriptor.deviceLostCallback = (WGPUDeviceLostCallback)(DeviceLostCallback);
		deviceDescriptor.deviceLostUserdata = nullptr;
		deviceDescriptor.label = "device";
		deviceDescriptor.nextInChain = nullptr;
		deviceDescriptor.requiredFeatureCount = requiredFeatures.size();
		deviceDescriptor.requiredFeatures = (WGPUFeatureName*)(requiredFeatures.data());
		deviceDescriptor.requiredLimits = &requiredLimits;

		if (!RequestDevice(device, adapter, deviceDescriptor)) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create device.", true);
		}

		wgpu::SupportedLimits deviceSupportedLimits {};
		GetDeviceSupportedLimits(device, deviceSupportedLimits, true);

		uniformStride = CeilToNextMultiple(static_cast<uint32_t>(sizeof(MyUniforms)),
			                               static_cast<uint32_t>(deviceSupportedLimits.limits.minUniformBufferOffsetAlignment));

		errorCallback = device.setUncapturedErrorCallback(UncapturedErrorCallback);
		if (errorCallback == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to set uncaptured device error callback.", true);
		}

		logger.Info("Device created.");
	}

	// configure compatible surface
	{
		logger.Info("Configuring compatible surface...");

		std::vector<wgpu::TextureFormat> textureFormats {};

		wgpu::SurfaceConfiguration surfaceConfiguration {};
		surfaceConfiguration.alphaMode = wgpu::CompositeAlphaMode::Auto;
		surfaceConfiguration.device = device;
		surfaceConfiguration.format = surface.getPreferredFormat(adapter);
		surfaceConfiguration.height = 600;
		surfaceConfiguration.nextInChain = nullptr;
		surfaceConfiguration.presentMode = wgpu::PresentMode::Fifo;
		surfaceConfiguration.usage = wgpu::TextureUsage::RenderAttachment;
		surfaceConfiguration.viewFormatCount = textureFormats.size();
		surfaceConfiguration.viewFormats = (WGPUTextureFormat*)(textureFormats.data());
		surfaceConfiguration.width = 800;
		surface.configure(surfaceConfiguration);

		logger.Info("Compatible surface configured.");
	}

	// retrieve the queue
	{
		logger.Info("Retrieving queue...");

		queue = device.getQueue();
		if (queue == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create queue.", true);
		}

		logger.Info("Queue retrieved.");
	}

	// create the render pipeline
	{
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
		depthTextureDescriptor.size = { 800, 600, 1 };
		depthTextureDescriptor.usage = wgpu::TextureUsage::RenderAttachment;
		depthTextureDescriptor.viewFormatCount = 1;
		depthTextureDescriptor.viewFormats = (WGPUTextureFormat*)(&depthTextureFormat);
		depthTexture = device.createTexture(depthTextureDescriptor);
		if (depthTexture == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create depth texture.", true);
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
			return CleanOnExit(EXIT_FAILURE, "Failed to create depth texture view.", true);
		}

		std::vector<wgpu::ConstantEntry> fragmentConstantEntries {};

		std::vector<wgpu::ShaderModuleCompilationHint> fragmentShaderModuleCompilationHints {};

		wgpu::ChainedStruct fragmentShaderModuleChainedStruct {};
		fragmentShaderModuleChainedStruct.next = nullptr;
		fragmentShaderModuleChainedStruct.sType = wgpu::SType::ShaderModuleWGSLDescriptor;

		fragmentShaderModule = LoadShaderModule("resources/shader.wgsl", device);
		if (fragmentShaderModule == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create fragment shader module.", true);
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
		colorTargetStates.resize(1, wgpu::ColorTargetState {});
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

		wgpu::BindGroupLayoutEntry bindGroupLayoutEntry = wgpu::Default;
		bindGroupLayoutEntry.binding = 0;
		bindGroupLayoutEntry.visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;

		bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::Uniform;
		bindGroupLayoutEntry.buffer.minBindingSize = sizeof(MyUniforms);
		bindGroupLayoutEntry.buffer.hasDynamicOffset = false;
		bindGroupLayoutEntry.buffer.nextInChain = nullptr;

		wgpu::BindGroupLayoutDescriptor bindGroupLayoutDescriptor {};
		bindGroupLayoutDescriptor.entryCount = 1;
		bindGroupLayoutDescriptor.entries = &bindGroupLayoutEntry;
		bindGroupLayouts.push_back(device.createBindGroupLayout(bindGroupLayoutDescriptor));
		if (bindGroupLayouts[0] == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create bind group layout.", true);
		}

		wgpu::BufferDescriptor bufferDesc {};
		bufferDesc.size = uniformStride + sizeof(MyUniforms);
		bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
		bufferDesc.mappedAtCreation = false;
		uniformBuffer = device.createBuffer(bufferDesc);
		if (uniformBuffer == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create uniform buffer.", true);
		}

		wgpu::BindGroupEntry bindGroupEntry {};
		bindGroupEntry.binding = 0;
		bindGroupEntry.buffer = uniformBuffer;
		bindGroupEntry.offset = 0;
		bindGroupEntry.size = sizeof(MyUniforms);

		wgpu::BindGroupDescriptor bindGroupDescriptor {};
		bindGroupDescriptor.layout = bindGroupLayouts[0];
		bindGroupDescriptor.entryCount = 1;
		bindGroupDescriptor.entries = &bindGroupEntry;
		bindGroups.push_back(device.createBindGroup(bindGroupDescriptor));
		if (bindGroups[0] == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create bind group.", true);
		}

		wgpu::PipelineLayoutDescriptor pipelineLayoutDescriptor {};
		pipelineLayoutDescriptor.bindGroupLayoutCount = bindGroupLayouts.size();
		pipelineLayoutDescriptor.bindGroupLayouts = (WGPUBindGroupLayout*)(bindGroupLayouts.data());
		pipelineLayoutDescriptor.label = "pipeline_layout";
		pipelineLayoutDescriptor.nextInChain = nullptr;
		pipelineLayout = device.createPipelineLayout(pipelineLayoutDescriptor);
		if (pipelineLayout == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create pipeline layout.", true);
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

		std::vector<wgpu::VertexBufferLayout> vertexBufferLayouts {};

		wgpu::VertexBufferLayout vertexBufferLayout {};

		std::vector<wgpu::VertexAttribute> vertexAttributes(2);

		// position	attribute
		vertexAttributes[0].shaderLocation = 0;
		vertexAttributes[0].format = wgpu::VertexFormat::Float32x3;
		vertexAttributes[0].offset = 0;

		// color attribute
		vertexAttributes[1].shaderLocation = 1;
		vertexAttributes[1].format = wgpu::VertexFormat::Float32x3;
		vertexAttributes[1].offset = 3 * sizeof(float);

		// actual data
		vertexBufferLayout.attributeCount = static_cast<uint32_t>(vertexAttributes.size());
		vertexBufferLayout.attributes = vertexAttributes.data();

		// data stride and stepthrough method
		vertexBufferLayout.arrayStride = 6 * sizeof(float);
		vertexBufferLayout.stepMode = wgpu::VertexStepMode::Vertex;

		vertexBufferLayouts.push_back(vertexBufferLayout);

		std::vector<wgpu::ConstantEntry> vertexConstantEntries {};

		std::vector<wgpu::ShaderModuleCompilationHint> vertexShaderModuleCompilationHints {};

		vertexShaderModule = LoadShaderModule("resources/shader.wgsl", device);
		if (vertexShaderModule == nullptr) {
			return CleanOnExit(EXIT_FAILURE, "Failed to create vertex shader module.", true);
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
			return CleanOnExit(EXIT_FAILURE, "Failed to create render pipeline.", true);
		}

		logger.Info("Render pipeline created.");
	}

	std::vector<float> vertexData; /* = {
		-0.5, -0.5, 1.0, 0.0, 0.0,
		+0.5, -0.5, 0.0, 1.0, 0.0,
		+0.5, +0.5, 0.0, 0.0, 1.0,
		-0.5, +0.5, 1.0, 1.0, 1.0
	};
	*/

	std::vector<uint16_t> indexData; /* = {
		0, 1, 2,
		0, 2, 3
	};
	*/

	bool success = LoadGeometry("resources/pyramid.txt", vertexData, indexData, 3);
	if (!success) {
		return CleanOnExit(EXIT_FAILURE, "Failed to load geometry.", true);
	}

	uint32_t vertexCount = vertexData.size() / 5;
	uint32_t indexCount = indexData.size();

	wgpu::BufferDescriptor bufferDesc {};
	bufferDesc.size = vertexData.size() * sizeof(float);
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
	bufferDesc.mappedAtCreation = false;
	vertexBuffer = device.createBuffer(bufferDesc);
	if (vertexBuffer == nullptr) {
		return CleanOnExit(EXIT_FAILURE, "Failed to create vertex buffer.", true);
	}

	queue.writeBuffer(vertexBuffer, 0, vertexData.data(), bufferDesc.size);

	bufferDesc.size = indexData.size() * sizeof(uint16_t);
	bufferDesc.size = (bufferDesc.size + 3) & ~3;
	bufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Index;
	indexBuffer = device.createBuffer(bufferDesc);
	if (indexBuffer == nullptr) {
		return CleanOnExit(EXIT_FAILURE, "Failed to create index buffer.", true);
	}

	queue.writeBuffer(indexBuffer, 0, indexData.data(), bufferDesc.size);

	MyUniforms uniforms {};
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

		uniforms.time = static_cast<float>(SDL_GetTicks()) / 1000;
		queue.writeBuffer(uniformBuffer, offsetof(MyUniforms, time), &uniforms.time, sizeof(MyUniforms::time));

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
		renderPassColorAttachments[0].clearValue = wgpu::Color{ 0.1f, 0.1f, 0.1f, 1.0f };

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
		renderPassEncoder.setVertexBuffer(0, vertexBuffer, 0, vertexBuffer.getSize());
		renderPassEncoder.setIndexBuffer(indexBuffer, wgpu::IndexFormat::Uint16, 0, indexBuffer.getSize());

		renderPassEncoder.setBindGroup(0, bindGroups[0], 0, nullptr);
		renderPassEncoder.drawIndexed(indexCount, 1, 0, 0, 0);
		
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
