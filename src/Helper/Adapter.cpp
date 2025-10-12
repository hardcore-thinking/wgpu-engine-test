#include <Helper/Adapter.hpp>

Adapter::Adapter(Instance& instance, Surface& surface) {
    wgpu::RequestAdapterOptions options {};
    options.compatibleSurface = surface.Handle();
    //options.powerPreference = wgpu::PowerPreference::HighPerformance;
	options.powerPreference = wgpu::PowerPreference::LowPower;
	options.nextInChain = nullptr;
    options.forceFallbackAdapter = false;
    options.featureLevel = wgpu::FeatureLevel::Compatibility;

    _handle = instance->requestAdapter(options);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU adapter");
    }

    std::cout << "Adapter created successfully: " << Handle() << std::endl;

    _handle.getFeatures(&_features);
    _handle.getInfo(&_infos);
    _handle.getLimits(&_limits);
}

Adapter::Adapter(Instance& instance, wgpu::RequestAdapterOptions const& options) {
    _handle = instance->requestAdapter(options);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU adapter");
    }

    //std::cout << "Adapter created successfully: " << Handle() << std::endl;

    _handle.getFeatures(&_features);
    _handle.getInfo(&_infos);
    _handle.getLimits(&_limits);
}

Adapter::~Adapter() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    //std::cout << "Adapter destroyed successfully" << std::endl;
}

wgpu::Adapter* Adapter::operator->() {
	return &_handle;
}

void Adapter::DisplayFeatures() const {
	std::cout << "\t - " << "Features (found " << _features.featureCount << "):" << std::endl;
	for (size_t i = 0; i < _features.featureCount; ++i) {
		wgpu::FeatureName feature = _features.features[i];
		switch (feature) {
			case wgpu::FeatureName::BGRA8UnormStorage:
				std::cout << "\t\t- " << "wgpu::FeatureName::BGRA8UnormStorage (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::ClipDistances:
				std::cout << "\t\t- " << "wgpu::FeatureName::ClipDistances (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::Depth32FloatStencil8:
				std::cout << "\t\t- " << "wgpu::FeatureName::Depth32FloatStencil8 (" << feature << ")" << std::endl;
				break;
		
			case wgpu::FeatureName::DepthClipControl:
				std::cout << "\t\t- " << "wgpu::FeatureName::DepthClipControl (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::DualSourceBlending:
				std::cout << "\t\t- " << "wgpu::FeatureName::DualSourceBlending (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::Float32Blendable:
				std::cout << "\t\t- " << "wgpu::FeatureName::Float32Blendable (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::Float32Filterable:
				std::cout << "\t\t- " << "wgpu::FeatureName::Float32Filterable (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::IndirectFirstInstance:
				std::cout << "\t\t- " << "wgpu::FeatureName::IndirectFirstInstance (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::RG11B10UfloatRenderable:
				std::cout << "\t\t- " << "wgpu::FeatureName::RG11B10UfloatRenderable (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::ShaderF16:
				std::cout << "\t\t- " << "wgpu::FeatureName::ShaderF16 (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionASTC:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionASTC (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionASTCSliced3D:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionASTCSliced3D (" << feature << ")" << std::endl;
				break;
			
			case wgpu::FeatureName::TextureCompressionBC:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionBC (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionBCSliced3D:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionBCSliced3D (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TextureCompressionETC2:
				std::cout << "\t\t- " << "wgpu::FeatureName::TextureCompressionETC2 (" << feature << ")" << std::endl;
				break;

			case wgpu::FeatureName::TimestampQuery:
				std::cout << "\t\t- " << "wgpu::FeatureName::TimestampQuery (" << feature << ")" << std::endl;
				break;

			default:
				std::cout << "\t\t- " << "wgpu::FeatureName::Undefined (" << feature << ")" << std::endl;
				break;
		}
	}

	std::cout << std::endl;
}

void Adapter::DisplayInfos() const {
	std::cout << "\t- " << "Infos: " << std::endl;

	std::cout << "\t\t" << "Adapter type: ";
	switch (_infos.adapterType) {
		case wgpu::AdapterType::CPU:
			std::cout << "CPU" << std::endl;
			break;

		case wgpu::AdapterType::DiscreteGPU:
			std::cout << "Discrete GPU" << std::endl;
			break;
		
		case wgpu::AdapterType::IntegratedGPU:
			std::cout << "Integrated GPU" << std::endl;
			break;

		default:
			std::cout << "Unknown" << std::endl;
			break;
	};

	//std::cout << "\t\t" << "Architecture: " << _infos.architecture.data << std::endl;
	std::cout << "\t\t" << "Backend type: ";
	switch (_infos.backendType) {
		case wgpu::BackendType::D3D11:
			std::cout << "Direct3D 11" << std::endl;
			break;

		case wgpu::BackendType::D3D12:
			std::cout << "Direct3D 12" << std::endl;
			break;

		case wgpu::BackendType::Metal:
			std::cout << "Metal" << std::endl;
			break;

		case wgpu::BackendType::Null:
			std::cout << "Null" << std::endl;
			break;

		case wgpu::BackendType::OpenGL:
			std::cout << "OpenGL" << std::endl;
			break;

		case wgpu::BackendType::OpenGLES:
			std::cout << "OpenGLES" << std::endl;
			break;

		case wgpu::BackendType::Vulkan:
			std::cout << "Vulkan" << std::endl;
			break;

		case wgpu::BackendType::WebGPU:
			std::cout << "WebGPU" << std::endl;
			break;

		default:
			std::cout << "Undefined" << std::endl;
			break;
	}

	std::cout << "\t\t" << "Device ID: " << _infos.deviceID << std::endl;
	std::cout << "\t\t" << "Driver description: " << _infos.description.data[0] << std::endl;
	std::cout << "\t\t" << "Name: " << _infos.device.data << std::endl;
	std::cout << "\t\t" << "Vendor ID: " << _infos.vendorID << std::endl;
	std::cout << "\t\t" << "Vendor name: " << _infos.vendor.data << std::endl;
}

void Adapter::DisplayLimits() const {
    std::cout << "\t- " << "Limits: " << std::endl;

	std::cout << "\t\t- " << "Max bind groups: " << _limits.maxBindGroups << std::endl;
	std::cout << "\t\t- " << "Max bind groups plus vertex buffers: " << _limits.maxBindGroupsPlusVertexBuffers << std::endl;
	std::cout << "\t\t- " << "Max bindings per bind group: " << _limits.maxBindingsPerBindGroup << std::endl;
	std::cout << "\t\t- " << "Max buffer size: " << _limits.maxBufferSize << std::endl;
	std::cout << "\t\t- " << "Max color attachment bytes per sample: " << _limits.maxColorAttachmentBytesPerSample << std::endl;
	std::cout << "\t\t- " << "Max color attachments: " << _limits.maxColorAttachments << std::endl;
	std::cout << "\t\t- " << "Max compute invocations per workgroup: " << _limits.maxComputeInvocationsPerWorkgroup << std::endl;
	std::cout << "\t\t- " << "Max compute workgroup size x: " << _limits.maxComputeWorkgroupSizeX << std::endl;
	std::cout << "\t\t- " << "Max compute workgroup size y: " << _limits.maxComputeWorkgroupSizeY << std::endl;
	std::cout << "\t\t- " << "Max compute workgroup size z: " << _limits.maxComputeWorkgroupSizeZ << std::endl;
	std::cout << "\t\t- " << "Max compute workgroups per dimension: " << _limits.maxComputeWorkgroupsPerDimension << std::endl;
	std::cout << "\t\t- " << "Max compute workgroup storage size: " << _limits.maxComputeWorkgroupStorageSize << std::endl;
	std::cout << "\t\t- " << "Max dynamic storage buffers per pipeline layout: " << _limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
	std::cout << "\t\t- " << "Max dynamic uniform buffers per pipeline layout: " << _limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
	//std::cout << "\t\t- " << "Max inter stage shader components: " << _limits.maxInterStageShaderComponents << std::endl;
	std::cout << "\t\t- " << "Max inter stage shader variables: " << _limits.maxInterStageShaderVariables << std::endl;
	std::cout << "\t\t- " << "Max sampled textures per shader stage: " << _limits.maxSampledTexturesPerShaderStage << std::endl;
	std::cout << "\t\t- " << "Max samplers per shader stage: " << _limits.maxSamplersPerShaderStage << std::endl;
	std::cout << "\t\t- " << "Max storage buffer binding size: " << _limits.maxStorageBufferBindingSize << std::endl;
	std::cout << "\t\t- " << "Max storage buffers per shader stage: " << _limits.maxStorageBuffersPerShaderStage << std::endl;
	std::cout << "\t\t- " << "Max storage textures per shader stage: " << _limits.maxStorageTexturesPerShaderStage << std::endl;
	std::cout << "\t\t- " << "Max texture array layers: " << _limits.maxTextureArrayLayers << std::endl;
	std::cout << "\t\t- " << "Max texture dimension 1D: " << _limits.maxTextureDimension1D << std::endl;
	std::cout << "\t\t- " << "Max texture dimension 2D: " << _limits.maxTextureDimension2D << std::endl;
	std::cout << "\t\t- " << "Max texture dimension 3D: " << _limits.maxTextureDimension3D << std::endl;
	std::cout << "\t\t- " << "Max uniform buffer binding size: " << _limits.maxUniformBufferBindingSize << std::endl;
	std::cout << "\t\t- " << "Max uniform buffers per shader stage: " << _limits.maxUniformBuffersPerShaderStage << std::endl;
	std::cout << "\t\t- " << "Max vertex attributes: " << _limits.maxVertexAttributes << std::endl;
	std::cout << "\t\t- " << "Max vertex buffer array stride: " << _limits.maxVertexBufferArrayStride << std::endl;
	std::cout << "\t\t- " << "Max vertex buffers: " << _limits.maxVertexBuffers << std::endl;
	std::cout << "\t\t- " << "Min storage buffer offset alignment: " << _limits.minStorageBufferOffsetAlignment << std::endl;
	std::cout << "\t\t- " << "Min uniform buffer offset alignment: " << _limits.minUniformBufferOffsetAlignment << std::endl;

	std::cout << std::endl;
}

/*
    std::vector<wgpu::Adapter> adapters {};
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
		wgpu::SupportedFeatures features;
		wgpu::Limits limits {};
		wgpu::AdapterInfo infos {};

		GetAdapterFeatures(adapter, &features);
		GetAdapterSupportedLimits(adapter, &limits);
		GetAdapterProperties(adapter, &infos);
	}

	adapter = instance.requestAdapter(options);
*/
