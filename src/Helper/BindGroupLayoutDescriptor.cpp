#include <Helper/BindGroupLayoutDescriptor.hpp>

BindGroupLayoutDescriptor::BindGroupLayoutDescriptor(std::vector<BindGroupLayoutEntry> const& bindGroupLayoutEntries) {
    entryCount = static_cast<uint32_t>(bindGroupLayoutEntries.size());
    entries = bindGroupLayoutEntries.data();
    nextInChain = nullptr;
}