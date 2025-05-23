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
        TextureView(TextureView const& textureView) = delete;
        TextureView(TextureView&& other);
        TextureView(wgpu::TextureView const& textureView) = delete;
        TextureView(wgpu::TextureView&& other);
        TextureView(Texture& texture, TextureViewDescriptor const& descriptor);
        ~TextureView();

    public:
        wgpu::TextureView& Handle() { return _handle; }
        wgpu::TextureView const& Handle() const { return _handle; }

        TextureView& operator = (TextureView const& textureView) = delete;
        TextureView& operator = (TextureView&& other);
        TextureView& operator = (wgpu::TextureView const& textureView) = delete;
        TextureView& operator = (wgpu::TextureView&& other);

        wgpu::TextureView* operator -> ();
        wgpu::TextureView const* operator -> () const;

    private:
        wgpu::TextureView _handle = nullptr;
};

#endif // TEXTUREVIEW_HPP