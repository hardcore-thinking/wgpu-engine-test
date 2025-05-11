#ifndef SAMPLERBINDING_HPP
#define SAMPLERBINDING_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupEntry CreateSamplerBinding(
    uint32_t binding,
    wgpu::Sampler& sampler
);

void EditSamplerBinding(
    wgpu::BindGroupEntry& bindGroupEntry,
    uint32_t binding,
    wgpu::Sampler& sampler
);

void DisplaySamplerBinding(
    wgpu::BindGroupEntry const& bindGroupEntry
);

#endif // SAMPLERBINDING_HPP