#include <Helper/SamplerBindingLayout.hpp>

wgpu::BindGroupLayoutEntry CreateSamplerBindingLayout(uint32_t binding, wgpu::ShaderStage visibility, wgpu::SamplerBindingType type, wgpu::ChainedStruct const* nextInChain) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;
	
	bindGroupLayoutEntry.sampler.type = type;
	bindGroupLayoutEntry.sampler.nextInChain = nextInChain;

	bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	bindGroupLayoutEntry.texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;

	return bindGroupLayoutEntry;
}

void EditSamplerBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStage visibility, wgpu::SamplerBindingType type, wgpu::ChainedStruct const* nextInChain) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.sampler.type = type;
	bindGroupLayoutEntry.sampler.nextInChain = nextInChain;

	bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	bindGroupLayoutEntry.texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
}

void DisplaySamplerBindingLayout(wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry) {
	std::cout << "- " << "Sampler binding layout:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	std::cout << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	std::cout << "\t- " << "type: " << bindGroupLayoutEntry.sampler.type << std::endl;
}
