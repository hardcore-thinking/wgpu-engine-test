#include <Helper/TextureView.hpp>

TextureView::TextureView(Texture& texture, TextureViewDescriptor const& descriptor) {
    _handle = texture->createView(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU texture view");
    }
    
    std::cout << "Texture view created successfully: " << Handle() << std::endl;
}

TextureView::~TextureView() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "Texture view released successfully" << std::endl;
}

wgpu::TextureView* TextureView::operator->() {
    return &_handle;
}