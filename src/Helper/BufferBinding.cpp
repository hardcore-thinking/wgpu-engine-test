#include <Helper/BufferBinding.hpp>

BufferBinding::BufferBinding(uint32_t bufferBinding, Buffer& bufferHandle, uint32_t bufferSize, uint32_t bufferOffset) {
	binding = bufferBinding;
	buffer = (WGPUBuffer)(&bufferHandle);
	size = bufferSize;
	offset = bufferOffset;
}

std::ostream& operator<<(std::ostream& out, BufferBinding const& bufferBinding) {
	out << "- " << "Buffer binding:" << std::endl;
	out << "\t- " << "binding: " << bufferBinding.binding << std::endl;
	out << "\t- " << "buffer: " << bufferBinding.buffer << std::endl;
	out << "\t- " << "size: " << bufferBinding.size << std::endl;
	out << "\t- " << "offset: " << bufferBinding.offset << std::endl;

	return out;
}
