#include <Helper/BufferBindingLayout.hpp>

wgpu::BindGroupLayoutEntry CreateBufferBindingLayout(uint32_t binding, wgpu::ShaderStage visibility, wgpu::BufferBindingType type, uint32_t size, uint32_t minBindingSize, bool hasDynamicOffset, wgpu::ChainedStruct const* nextInChain) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.buffer.hasDynamicOffset = hasDynamicOffset;
	bindGroupLayoutEntry.buffer.minBindingSize = minBindingSize;
	bindGroupLayoutEntry.buffer.type = type;
	bindGroupLayoutEntry.buffer.nextInChain = nextInChain;

	bindGroupLayoutEntry.texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	bindGroupLayoutEntry.sampler.type = wgpu::SamplerBindingType::BindingNotUsed;

	return bindGroupLayoutEntry;
}

void EditBufferBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStage visibility, wgpu::BufferBindingType type, uint32_t size, uint32_t minBindingSize, bool hasDynamicOffset, wgpu::ChainedStruct const* nextInChain) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.buffer.hasDynamicOffset = hasDynamicOffset;
	bindGroupLayoutEntry.buffer.minBindingSize = minBindingSize;
	bindGroupLayoutEntry.buffer.type = type;
	bindGroupLayoutEntry.buffer.nextInChain = nextInChain;

	bindGroupLayoutEntry.texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	bindGroupLayoutEntry.sampler.type = wgpu::SamplerBindingType::BindingNotUsed;
}

void DisplayBufferBindingLayout(wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry) {
	std::cout << "- " << "Buffer binding layout:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	std::cout << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	std::cout << "\t- " << "hasDynamicOffset: " << bindGroupLayoutEntry.buffer.hasDynamicOffset << std::endl;
	std::cout << "\t- " << "minBindingSize: " << bindGroupLayoutEntry.buffer.minBindingSize << std::endl;
	std::cout << "\t- " << "type: " << bindGroupLayoutEntry.buffer.type << std::endl;
}
