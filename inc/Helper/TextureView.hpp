#ifndef TEXTUREVIEW_HPP
#define TEXTUREVIEW_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Texture.hpp>
#include <Helper/TextureViewDescriptor.hpp>

class TextureView : public wgpu::TextureView {
    public:
        TextureView() = default;
        TextureView(Texture& texture, TextureViewDescriptor const& descriptor);
        ~TextureView();

    public:
        wgpu::TextureView& Handle() { return _handle; }
        wgpu::TextureView const& Handle() const { return _handle; }

        wgpu::TextureView* operator->();

    private:
        wgpu::TextureView _handle = nullptr;
};

#endif // TEXTUREVIEW_HPP