#ifndef VERTEXSTATE_HPP
#define VERTEXSTATE_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

#include <Helper/ShaderModule.hpp>
#include <Helper/VertexBufferLayout.hpp>
#include <Helper/ConstantEntry.hpp>

struct VertexState : public wgpu::VertexState {
public:
	VertexState() = delete;
	VertexState(wgpu::StringView const& entryPointName, ShaderModule& shaderModule, std::vector<VertexBufferLayout> const& vertexBufferLayouts, std::vector<ConstantEntry> const& constantsEntries);
};

#endif // VERTEXSTATE_HPP
