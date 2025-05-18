#include <Helper/BindGroupLayoutDescriptor.hpp>

BindGroupLayoutDescriptor::BindGroupLayoutDescriptor(std::vector<BindGroupLayoutEntry> const& bindGroupLayoutEntries) {
    /*
    for (auto& entry : bindGroupLayoutEntries) {
        std::cout << "BindGroupLayoutEntry #" << entry.binding << std::endl;

        if (entry.buffer.type != wgpu::BufferBindingType::BindingNotUsed) {
            std::cout << "  buffer.hasBindingOffset: " << entry.buffer.hasDynamicOffset << std::endl;
            std::cout << "  buffer.minBindingSize: " << entry.buffer.minBindingSize << std::endl;
            std::cout << "  buffer.offset: " << entry.buffer.type << std::endl;
        }

        else if (entry.texture.sampleType != wgpu::TextureSampleType::BindingNotUsed) {
            std::cout << "  texture.multisampled: " << entry.texture.multisampled << std::endl;
            std::cout << "  texture.sampleType: " << entry.texture.sampleType << std::endl;
            std::cout << "  texture.viewDimension: " << entry.texture.viewDimension << std::endl;
        }

        else if (entry.sampler.type != wgpu::SamplerBindingType::BindingNotUsed) {
            std::cout << "  sampler.type: " << entry.sampler.type << std::endl;
        }

        std::cout << std::endl;
    }
    */

    entryCount = static_cast<uint32_t>(bindGroupLayoutEntries.size());
    entries = bindGroupLayoutEntries.data();
    nextInChain = nullptr;
}