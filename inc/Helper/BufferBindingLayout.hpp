#ifndef BUFFERBINDINGLAYOUT_HPP
#define BUFFERBINDINGLAYOUT_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupLayoutEntry CreateBufferBindingLayout(
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::BufferBindingType type,
    uint32_t size,
    uint32_t minBindingSize,
    bool hasDynamicOffset = false,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void EditBufferBindingLayout(
    wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry,
    uint32_t binding,
    wgpu::ShaderStage visibility,
    wgpu::BufferBindingType type,
    uint32_t size,
    uint32_t minBindingSize,
    bool hasDynamicOffset = false,
    wgpu::ChainedStruct const* nextInChain = nullptr
);

void DisplayBufferBindingLayout(
    wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry
);

struct BufferBindingLayout : public wgpu::BindGroupLayoutEntry {
    public:
        BufferBindingLayout() = delete;
        BufferBindingLayout(uint32_t binding, wgpu::ShaderStage visibility, wgpu::BufferBindingType type, uint32_t size, uint32_t minBindingSize);
};

#endif // BUFFERBINDINGLAYOUT_HPP