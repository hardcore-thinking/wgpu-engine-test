#include <Helper/FragmentState.hpp>

FragmentState::FragmentState(wgpu::StringView const& entryPointName, ShaderModule& shaderModule, std::vector<ColorTargetState> const& colorTargets, std::vector<ConstantEntry> const& constantsEntries) {
	constantCount = constantsEntries.size();
	constants = constantsEntries.data();
	entryPoint = entryPointName;
	module = shaderModule.Handle();
	targetCount = colorTargets.size();
	targets = colorTargets.data();
	nextInChain = nullptr;
}
