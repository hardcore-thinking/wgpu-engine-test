#include <Helper/CommandBuffer.hpp>

CommandBuffer::CommandBuffer(CommandEncoder& commandEncoder) {
	_handle = commandEncoder->finish();
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create command buffer");
	}

	//std::cout << "Command buffer created successfully: " << Handle() << std::endl;
}

CommandBuffer::~CommandBuffer() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Command buffer destroyed successfully" << std::endl;
}
