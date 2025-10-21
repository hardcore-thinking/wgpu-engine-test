#include <Helper/Device.hpp>

Device::Device(Adapter& adapter, DeviceDescriptor const& descriptor) {
	_handle = adapter->requestDevice(descriptor);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create WGPU device");
	}

	//std::cout << "Device created successfully: " << Handle() << std::endl;

	_handle.getLimits(&_limits);
}

Device::~Device() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Device destroyed successfully" << std::endl;
}

void Device::DisplayLimits() const {
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

wgpu::Device* Device::operator -> () {
	return &_handle;
}

wgpu::Device const* Device::operator -> () const {
	return &_handle;
}
