#include <Helper/BindGroupDescriptor.hpp>

BindGroupDescriptor::BindGroupDescriptor(BindGroupLayout& bindGroupLayout, std::vector<BindGroupEntry> const& bindGroupEntries) {
    layout = bindGroupLayout.Handle();
    entryCount = static_cast<uint32_t>(bindGroupEntries.size());
    entries = bindGroupEntries.data();
    nextInChain = nullptr;
}