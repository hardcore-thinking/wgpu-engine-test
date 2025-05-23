#include <Helper/Texture.hpp>

Texture::Texture(Device& device, TextureDescriptor const& descriptor) {
    _handle = device->createTexture(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU texture");
    }
    
    std::cout << "Texture created successfully: " << Handle() << std::endl;
}

Texture::Texture(Texture&& other) {
    _handle = other._handle;
    other._handle = nullptr;
    //std::cout << "Texture created successfully: " << Handle() << std::endl;
}

Texture::Texture(wgpu::Texture&& other) {
    _handle = other;
    other = nullptr;
}

Texture::~Texture() {
    std::cout << "Texture destructor called" << std::endl;
    if (_handle != nullptr) {
        _handle.destroy();
        _handle.release();
        _handle = nullptr;

        //std::cout << "Texture destroyed and released successfully" << std::endl;
    }
}

Texture& Texture::operator = (Texture&& other) {
    if (this != &other) {
        if (_handle != nullptr) {
            _handle.destroy();
            _handle.release();
        }
        _handle = other._handle;
        other._handle = nullptr;
    }
    return *this;
}

Texture& Texture::operator = (wgpu::Texture&& other) {
    if (this->Handle() != other) {
        if (_handle != nullptr) {
            _handle.destroy();
            _handle.release();
        }
        _handle = other;
        other = nullptr;
    }
    return *this;
}

wgpu::Texture* Texture::operator -> () {
    return &_handle;
}

wgpu::Texture const* Texture::operator -> () const {
    return &_handle;
}

