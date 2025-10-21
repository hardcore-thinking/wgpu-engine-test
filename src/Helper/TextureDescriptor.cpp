#include <Helper/TextureDescriptor.hpp>

TextureDescriptor::TextureDescriptor(wgpu::TextureFormat textureFormat, wgpu::TextureUsage textureUsage, Extent3D const& textureSize, std::vector<wgpu::TextureFormat> const& textureViewFormats) {
	(void) textureViewFormats;

	format = textureFormat;
	usage = textureUsage;
	size = textureSize;
	mipLevelCount = 1;
	sampleCount = 1;
	dimension = wgpu::TextureDimension::_2D;
	viewFormatCount = 0;
	viewFormats = nullptr;
	// viewFormatCount = static_cast<uint32_t>(textureViewFormats.size());
	// viewFormats = (WGPUTextureFormat*)(textureViewFormats.data());
	nextInChain = nullptr;
}
