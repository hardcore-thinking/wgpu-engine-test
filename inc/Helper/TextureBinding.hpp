#ifndef TEXTUREBINDING_HPP
#define TEXTUREBINDING_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupEntry CreateTextureBinding(
    uint32_t binding,
    wgpu::TextureView& textureView
);

void EditTextureBinding(
    wgpu::BindGroupEntry& bindGroupEntry,
    uint32_t binding,
    wgpu::TextureView& textureView
);

void DisplayTextureBinding(
    wgpu::BindGroupEntry const& bindGroupEntry
);

#endif // TEXTUREBINDING_HPP