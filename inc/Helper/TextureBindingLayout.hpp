#ifndef TEXUREBINDINGLAYOUT_HPP
#define TEXUREBINDINGLAYOUT_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupLayoutEntry CreateTextureBindingLayout(
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::TextureSampleType sampleType,
    wgpu::TextureViewDimension viewDimension,
    bool multisampled = false,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void EditTextureBindingLayout(
    wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry,
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::TextureSampleType sampleType,
    wgpu::TextureViewDimension viewDimension,
    bool multisampled = false,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void DisplayTextureBindingLayout(
    wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry
);

#endif // TEXTUREBINDINGLAYOUT_HPP