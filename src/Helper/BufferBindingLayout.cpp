#include <Helper/BufferBindingLayout.hpp>

BufferBindingLayout::BufferBindingLayout(uint32_t bufferBinding, wgpu::ShaderStage bufferVisibility, wgpu::BufferBindingType bufferType, uint32_t bufferMinBindingSize) {
	binding = bufferBinding;
	visibility = bufferVisibility;

	buffer.hasDynamicOffset = false;
	buffer.minBindingSize = bufferMinBindingSize;
	buffer.type = bufferType;
	buffer.nextInChain = nullptr;

	texture.sampleType = wgpu::TextureSampleType::BindingNotUsed;
	storageTexture.access = wgpu::StorageTextureAccess::BindingNotUsed;
	sampler.type = wgpu::SamplerBindingType::BindingNotUsed;

	nextInChain = nullptr;
}

std::ostream& operator<<(std::ostream& out, BufferBindingLayout const& bindGroupLayoutEntry) {
	std::cout << "- " << "Buffer binding layout:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupLayoutEntry.binding << std::endl;
	std::cout << "\t- " << "visibility: " << bindGroupLayoutEntry.visibility << std::endl;
	std::cout << "\t- " << "hasDynamicOffset: " << bindGroupLayoutEntry.buffer.hasDynamicOffset << std::endl;
	std::cout << "\t- " << "minBindingSize: " << bindGroupLayoutEntry.buffer.minBindingSize << std::endl;
	std::cout << "\t- " << "type: " << bindGroupLayoutEntry.buffer.type << std::endl;

	return out;
}
