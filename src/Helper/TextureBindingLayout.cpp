#include <Helper/TextureBindingLayout.hpp>

wgpu::BindGroupLayoutEntry CreateTextureBindingLayout(uint32_t binding, wgpu::ShaderStage visibility, wgpu::TextureSampleType sampleType, wgpu::TextureViewDimension viewDimension, bool multisampled, wgpu::ChainedStruct const* nextInChain) {
	wgpu::BindGroupLayoutEntry bindGroupLayoutEntry {};

	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.texture.multisampled = multisampled;
	bindGroupLayoutEntry.texture.sampleType = sampleType;
	bindGroupLayoutEntry.texture.viewDimension = viewDimension;
	bindGroupLayoutEntry.texture.nextInChain = nextInChain;

	bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	bindGroupLayoutEntry.sampler.type = wgpu::SamplerBindingType::BindingNotUsed;
	
	return bindGroupLayoutEntry;
}

void EditTextureBindingLayout(wgpu::BindGroupLayoutEntry& bindGroupLayoutEntry, uint32_t binding, wgpu::ShaderStage visibility, wgpu::TextureSampleType sampleType, wgpu::TextureViewDimension viewDimension, bool multisampled, wgpu::ChainedStruct const* nextInChain) {
	bindGroupLayoutEntry.binding = binding;
	bindGroupLayoutEntry.visibility = visibility;

	bindGroupLayoutEntry.texture.multisampled = multisampled;
	bindGroupLayoutEntry.texture.sampleType = sampleType;
	bindGroupLayoutEntry.texture.viewDimension = viewDimension;
	bindGroupLayoutEntry.texture.nextInChain = nextInChain;

	bindGroupLayoutEntry.buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	bindGroupLayoutEntry.storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	bindGroupLayoutEntry.sampler.type = wgpu::SamplerBindingType::BindingNotUsed;
}

void DisplayTextureBindingLayout(wgpu::BindGroupLayoutEntry const& bindGroupLayoutEntry) {
	std::cout << "- " << "Texture binding layout:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	std::cout << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	std::cout << "\t- " << "multisampled: " << bindGroupLayoutEntry.texture.multisampled << std::endl;
	std::cout << "\t- " << "sampleType: " << bindGroupLayoutEntry.texture.sampleType << std::endl;
	std::cout << "\t- " << "viewDimension: " << bindGroupLayoutEntry.texture.viewDimension << std::endl;
}
