#ifndef SHADERMODULE_HPP
#define SHADERMODULE_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>

class ShaderModule {
    public:
        ShaderModule() = delete;
        ShaderModule(Device& device, std::filesystem::path const& path);
        ~ShaderModule();

    public:
        wgpu::ShaderModule& Handle() { return _handle; }
        wgpu::ShaderModule const& Handle() const { return _handle; }

        wgpu::ShaderModule* operator->();

    private:
        wgpu::ShaderModule _handle = nullptr;
        std::string _code = "";
};

#endif // SHADERMODULE_HPP