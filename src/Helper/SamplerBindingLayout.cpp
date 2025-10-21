#include <Helper/SamplerBindingLayout.hpp>

SamplerBindingLayout::SamplerBindingLayout(uint32_t samplerBinding, wgpu::ShaderStage samplerVisibility, wgpu::SamplerBindingType samplerType) {
	binding = samplerBinding;
	visibility = samplerVisibility;

	sampler.type = samplerType;
	sampler.nextInChain = nullptr;

	buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
}

std::ostream& operator<<(std::ostream& out, SamplerBindingLayout const& bindGroupLayoutEntry) {
	out << "- " << "Sampler binding layout:" << std::endl;
	out << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	out << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	out << "\t- " << "type: " << bindGroupLayoutEntry.sampler.type << std::endl;

	return out;
}
