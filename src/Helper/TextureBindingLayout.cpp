#include <Helper/TextureBindingLayout.hpp>

TextureBindingLayout::TextureBindingLayout(uint32_t textureBinding, wgpu::ShaderStage textureVisibility, wgpu::TextureSampleType textureSampleType) {
	binding = textureBinding;
	visibility = textureVisibility;

	texture.multisampled = false;
	texture.sampleType = textureSampleType;
	texture.viewDimension = wgpu::TextureViewDimension::_2D;
	texture.nextInChain = nullptr;

	buffer.type = wgpu::BufferBindingType::BindingNotUsed;
	storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	sampler.type = wgpu::SamplerBindingType::BindingNotUsed;
}

std::ostream& operator<<(std::ostream& out, TextureBindingLayout const& bindGroupLayoutEntry) {
	out << "- " << "Texture binding layout:" << std::endl;
	out << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	out << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	out << "\t- " << "multisampled: " << bindGroupLayoutEntry.texture.multisampled << std::endl;
	out << "\t- " << "sampleType: " << bindGroupLayoutEntry.texture.sampleType << std::endl;
	out << "\t- " << "viewDimension: " << bindGroupLayoutEntry.texture.viewDimension << std::endl;

	return out;
}
