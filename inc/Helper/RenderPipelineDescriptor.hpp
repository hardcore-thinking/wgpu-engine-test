#ifndef RENDERPIPELINEDESCRIPTOR_HPP
#define RENDERPIPELINEDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/DepthStencilState.hpp>
#include <Helper/PrimitiveState.hpp>
#include <Helper/MultisampleState.hpp>
#include <Helper/VertexState.hpp>
#include <Helper/FragmentState.hpp>
#include <Helper/PipelineLayout.hpp>

struct RenderPipelineDescriptor : public wgpu::RenderPipelineDescriptor {
    public:
        RenderPipelineDescriptor(
            DepthStencilState const& depthStencilState,
            PrimitiveState const& primitiveState,
            MultisampleState const& multisampleState,
            VertexState const& vertexState,
            FragmentState const& fragmentState,
            PipelineLayout& pipelineLayout
        );
};

#endif // RENDERPIPELINEDESCRIPTOR_HPP