#include <Helper/PipelineLayoutDescriptor.hpp>

PipelineLayoutDescriptor::PipelineLayoutDescriptor(std::vector<BindGroupLayout> const& bindGroupLayoutsArray) {
    bindGroupLayoutCount = static_cast<uint32_t>(bindGroupLayoutsArray.size());
    bindGroupLayouts = (WGPUBindGroupLayout*)(bindGroupLayoutsArray.data());
    nextInChain = nullptr;
}