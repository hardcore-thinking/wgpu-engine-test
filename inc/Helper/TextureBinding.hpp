#ifndef TEXTUREBINDING_HPP
#define TEXTUREBINDING_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

#include <Helper/TextureView.hpp>

struct TextureBinding : public wgpu::BindGroupEntry {
    public:
        TextureBinding() = delete;
        TextureBinding(uint32_t binding, TextureView& textureView);

    public:
        friend std::ostream& operator<<(std::ostream& out, TextureBinding const& textureBinding);
};

#endif // TEXTUREBINDING_HPP