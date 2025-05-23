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
        Texture() = default;
        Texture(Texture const& texture) = delete;
        Texture(Device& device, TextureDescriptor const& descriptor);
        Texture(Texture&& texture);
        Texture(wgpu::Texture& texture) = delete;
        Texture(wgpu::Texture&& texture);
        ~Texture();

    public:
        wgpu::Texture& Handle() { return _handle; }
        wgpu::Texture const& Handle() const { return _handle; }

        Texture& operator = (Texture const& texture) = delete;
        Texture & operator = (Texture&& other);
        Texture & operator = (wgpu::Texture& other) = delete;
        Texture & operator = (wgpu::Texture&& other);

        wgpu::Texture* operator -> ();
        wgpu::Texture const* operator -> () const;
        
    private:
        wgpu::Texture _handle = nullptr;
};

#endif // TEXTURE_HPP