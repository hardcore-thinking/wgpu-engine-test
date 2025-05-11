#ifndef BUFFERBINDING_HPP
#define BUFFERBINDING_HPP

#include <wgpu-native/webgpu.hpp>

wgpu::BindGroupEntry CreateBufferBinding(
    uint32_t binding,
    wgpu::Buffer& buffer,
    uint32_t size,
    uint32_t offset = 0
);

void EditBufferBinding(
    wgpu::BindGroupEntry& bindGroupEntry,
    uint32_t binding,
    wgpu::Buffer& buffer,
    uint32_t size,
    uint32_t offset = 0
);

void DisplayBufferBinding(
    wgpu::BindGroupEntry const& bindGroupEntry
);

#endif // BUFFERBINDING_HPP