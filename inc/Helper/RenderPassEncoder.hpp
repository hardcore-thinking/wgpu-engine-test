#ifndef RENDERPASSENCODER_HPP
#define RENDERPASSENCODER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/CommandEncoder.hpp>
#include <Helper/RenderPassDescriptor.hpp>

class RenderPassEncoder {
    public:
        RenderPassEncoder() = delete;
        RenderPassEncoder(CommandEncoder& commandEncoder, RenderPassDescriptor const& renderPassDescriptor);
        ~RenderPassEncoder();

    public:
        wgpu::RenderPassEncoder& Handle() { return _handle; }
        wgpu::RenderPassEncoder const& Handle() const { return _handle; }

        wgpu::RenderPassEncoder* operator -> ();

    private:
        wgpu::RenderPassEncoder _handle = nullptr;
};

#endif // RENDERPASSENCODER_HPP