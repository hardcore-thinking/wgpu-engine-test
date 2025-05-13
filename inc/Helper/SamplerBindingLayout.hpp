#ifndef SAMPLERBINDINGLAYOUT_HPP
#define SAMPLERBINDINGLAYOUT_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

struct SamplerBindingLayout : public wgpu::BindGroupLayoutEntry {
    public:
        SamplerBindingLayout() = delete;
        SamplerBindingLayout(uint32_t binding, wgpu::ShaderStage visibility, wgpu::SamplerBindingType type);

    public:
        friend std::ostream& operator<<(std::ostream& out, SamplerBindingLayout const& bindGroupLayoutEntry);
};

#endif // SAMPLERBINDINGLAYOUT_HPP