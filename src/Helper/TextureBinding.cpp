#include <Helper/TextureBinding.hpp>

wgpu::BindGroupEntry CreateTextureBinding(uint32_t binding, wgpu::TextureView& textureView) {
	wgpu::BindGroupEntry bindGroupEntry {};
	
	bindGroupEntry.binding = binding;
	bindGroupEntry.textureView = textureView;

	return bindGroupEntry;
}

void EditTextureBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::TextureView& textureView) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.textureView = textureView;
}

void DisplayTextureBinding(wgpu::BindGroupEntry const& bindGroupEntry) {
	std::cout << "- " << "Texture binding:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupEntry.binding << std::endl;
	std::cout << "\t- " << "textureView: " << bindGroupEntry.textureView << std::endl;
}
