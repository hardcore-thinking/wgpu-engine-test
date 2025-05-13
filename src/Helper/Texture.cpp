#include <Helper/Texture.hpp>

Texture::Texture(Device& device, TextureDescriptor const& descriptor) {
    _handle = device->createTexture(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU texture");
    }
    
    std::cout << "Texture created successfully: " << Handle() << std::endl;
}

Texture::~Texture() {
    if (_handle != nullptr) {
        _handle.destroy();
        _handle.release();
        _handle = nullptr;

        std::cout << "Texture destroyed and released successfully" << std::endl;
    }
}

wgpu::Texture* Texture::operator->() {
    return &_handle;
}