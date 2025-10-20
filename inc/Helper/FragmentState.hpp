#ifndef FRAGMENTSTATE_HPP
#define FRAGMENTSTATE_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

#include <Helper/ShaderModule.hpp>
#include <Helper/ColorTargetState.hpp>
#include <Helper/ConstantEntry.hpp>

struct FragmentState : public wgpu::FragmentState {
public:
	FragmentState() = delete;
	FragmentState(wgpu::StringView const& entryPointName, ShaderModule& shaderModule, std::vector<ColorTargetState> const& colorTargets, std::vector<ConstantEntry> const& constantsEntries);
};

#endif // FRAGMENTSTATE_HPP
