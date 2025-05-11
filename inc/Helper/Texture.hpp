#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/TextureDescriptor.hpp>
#include <Helper/Device.hpp>

class Texture {
    public:
        Texture() = default;;
        Texture(Device& device, TextureDescriptor const& descriptor);
        ~Texture();

    public:
        wgpu::Texture& Handle() { return _handle; }
        wgpu::Texture const& Handle() const { return _handle; }

    private:
        wgpu::Texture _handle = nullptr;
};

#endif // TEXTURE_HPP