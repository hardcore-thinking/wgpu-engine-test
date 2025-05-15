#include <Helper/RenderPipelineDescriptor.hpp>

RenderPipelineDescriptor::RenderPipelineDescriptor(
    DepthStencilState const& depthStencilState,
    PrimitiveState const& primitiveState,
    MultisampleState const& multisampleState,
    VertexState const& vertexState,
    FragmentState const& fragmentState,
    PipelineLayout& pipelineLayout
) {
    label = wgpu::StringView("RenderPipelineDescriptor");
    depthStencil = &depthStencilState;
    primitive = primitiveState;
    multisample = multisampleState;
    vertex = vertexState;
    fragment = &fragmentState;
    layout = pipelineLayout.Handle();
    nextInChain = nullptr;
}
