#include <Helper/BindGroupDescriptor.hpp>

BindGroupDescriptor::BindGroupDescriptor(BindGroupLayout& bindGroupLayout, std::vector<BindGroupEntry> const& bindGroupEntries) {
    for (auto& entry : bindGroupEntries) {
        std::cout << "BindGroupEntry #" << entry.binding << std::endl;

        if (entry.buffer) {
            std::cout << "  buffer: " << entry.buffer << std::endl;
            std::cout << "  size: " << entry.size << std::endl;
            std::cout << "  offset: " << entry.offset << std::endl;
        }

        else if (entry.textureView) {
            std::cout << "  texture: " << entry.textureView << std::endl;
        }

        else if (entry.sampler) {
            std::cout << "  sampler: " << entry.sampler << std::endl;
        }
    }

    layout = bindGroupLayout.Handle();
    entryCount = static_cast<uint32_t>(bindGroupEntries.size());
    entries = bindGroupEntries.data();
    nextInChain = nullptr;
}