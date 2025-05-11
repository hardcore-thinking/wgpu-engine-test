#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/RenderPipelineDescriptor.hpp>

class RenderPipeline {
public:
    RenderPipeline() = delete;
    RenderPipeline(Device& device, RenderPipelineDescriptor const& renderPipelineDescriptor);
    ~RenderPipeline();

    public:
        wgpu::RenderPipeline& Handle() { return _handle; }
        wgpu::RenderPipeline const& Handle() const { return _handle; }

    private:
        wgpu::RenderPipeline _handle = nullptr;
};

#endif // RENDERPIPELINE_HPP