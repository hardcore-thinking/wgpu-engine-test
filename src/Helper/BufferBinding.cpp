#include <Helper/BufferBinding.hpp>

wgpu::BindGroupEntry CreateBufferBinding(uint32_t binding, wgpu::Buffer& buffer, uint32_t size, uint32_t offset) {
	wgpu::BindGroupEntry bindGroupEntry {};
	
	bindGroupEntry.binding = binding;
	bindGroupEntry.buffer = buffer;
	bindGroupEntry.size = size;
	bindGroupEntry.offset = offset;

	return bindGroupEntry;
}

void EditBufferBinding(wgpu::BindGroupEntry& bindGroupEntry, uint32_t binding, wgpu::Buffer& buffer, uint32_t size, uint32_t offset) {
	bindGroupEntry.binding = binding;

	bindGroupEntry.buffer = buffer;
	bindGroupEntry.size = size;
	bindGroupEntry.offset = offset;
}

void DisplayBufferBinding(wgpu::BindGroupEntry const& bindGroupEntry) {
	std::cout << "- " << "Buffer binding:" << std::endl;
	std::cout << "\t- " << "binding: " << bindGroupEntry.binding << std::endl;
	std::cout << "\t- " << "buffer: " << bindGroupEntry.buffer << std::endl;
	std::cout << "\t- " << "size: " << bindGroupEntry.size << std::endl;
	std::cout << "\t- " << "offset: " << bindGroupEntry.offset << std::endl;
}

