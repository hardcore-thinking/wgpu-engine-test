#ifndef BUFFERBINDINGLAYOUT_HPP
#define BUFFERBINDINGLAYOUT_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

struct BufferBindingLayout : public wgpu::BindGroupLayoutEntry {
    public:
        BufferBindingLayout() = delete;
        BufferBindingLayout(uint32_t bufferBinding, wgpu::ShaderStage bufferVisibility, wgpu::BufferBindingType bufferType, uint32_t bufferMinBindingSize);

    public:
        friend std::ostream& operator<<(std::ostream& out, BufferBindingLayout const& bindGroupLayoutEntry);       
};

#endif // BUFFERBINDINGLAYOUT_HPP