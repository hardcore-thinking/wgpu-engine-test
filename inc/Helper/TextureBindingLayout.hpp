#ifndef TEXUREBINDINGLAYOUT_HPP
#define TEXUREBINDINGLAYOUT_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

struct TextureBindingLayout : public wgpu::BindGroupLayoutEntry {
    public:
        TextureBindingLayout() = delete;
        TextureBindingLayout(uint32_t textureBinding, wgpu::ShaderStage textureVisibility, wgpu::TextureSampleType textureSampleType);

    public:
        friend std::ostream& operator<<(std::ostream& out, TextureBindingLayout const& bindGroupLayoutEntry);
};

#endif // TEXTUREBINDINGLAYOUT_HPP