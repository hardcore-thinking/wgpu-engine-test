#ifndef SAMPLERBINDINGLAYOUT_HPP
#define SAMPLERBINDINGLAYOUT_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupLayoutEntry CreateSamplerBindingLayout(
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::SamplerBindingType type,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void EditSamplerBindingLayout(
    wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry,
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::SamplerBindingType type,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void DisplaySamplerBindingLayout(
    wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry
);

#endif // SAMPLERBINDINGLAYOUT_HPP