#include <Helper/VertexState.hpp>

VertexState::VertexState(wgpu::StringView const& entryPointName, ShaderModule& shaderModule, std::vector<VertexBufferLayout> const& vertexBufferLayouts, std::vector<ConstantEntry> const& constantsEntries) {
    constantCount = constantsEntries.size();
    constants = constantsEntries.data();
    entryPoint = entryPointName;
    module = shaderModule.Handle();
    bufferCount = vertexBufferLayouts.size();
    buffers = vertexBufferLayouts.data();
    nextInChain = nullptr;
} 