#ifndef PIPELINELAYOUTDESCRIPTOR_HPP
#define PIPELINELAYOUTDESCRIPTOR_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

#include <Helper/BindGroupLayout.hpp>

struct PipelineLayoutDescriptor : public wgpu::PipelineLayoutDescriptor {
    public:    
        PipelineLayoutDescriptor() = delete;
        PipelineLayoutDescriptor(std::vector<BindGroupLayout> const& bindGroupLayouts);
};

#endif // PIPELINELAYOUTDESCRIPTOR_HPP