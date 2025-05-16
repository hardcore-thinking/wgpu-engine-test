#include <Helper/ShaderModule.hpp>

ShaderModule::ShaderModule(Device& device, std::filesystem::path const& path) {
    std::ifstream file(path);
	if (!file.is_open()) {
		throw std::runtime_error("Can't open " + path.string());
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	_code = std::string(size, ' ');
	file.seekg(0);
	file.read(_code.data(), size);

	wgpu::ShaderSourceWGSL shaderCodeDescriptor {};
	shaderCodeDescriptor.chain.next = nullptr;
	shaderCodeDescriptor.chain.sType = wgpu::SType::ShaderSourceWGSL;
	shaderCodeDescriptor.code = wgpu::StringView(_code.c_str());

	wgpu::ShaderModuleDescriptor shaderModuleDescriptor {};
	
	shaderModuleDescriptor.label = wgpu::StringView(path.stem().string().c_str());
	shaderModuleDescriptor.nextInChain = &shaderCodeDescriptor.chain;

	_handle = device.Handle().createShaderModule(shaderModuleDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create shader module from " + path.string());
    }

    //std::cout << "Shader module created: " << Handle() << std::endl;
}

ShaderModule::~ShaderModule() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Shader module destroyed" << std::endl;
}

wgpu::ShaderModule* ShaderModule::operator->() {
	return &_handle;
}

wgpu::ShaderModule const* ShaderModule::operator->() const {
	return &_handle;
}