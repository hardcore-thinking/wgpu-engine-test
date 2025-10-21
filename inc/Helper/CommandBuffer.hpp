#ifndef COMMANDBUFFER_HPP
#define COMMANDBUFFER_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/CommandEncoder.hpp>
#include <Helper/CommandBufferDescriptor.hpp>

class CommandBuffer {
public:
	CommandBuffer() = delete;
	CommandBuffer(CommandEncoder& commandEncoder);
	~CommandBuffer();

public:
	wgpu::CommandBuffer& Handle() {
		return _handle;
	}
	wgpu::CommandBuffer const& Handle() const {
		return _handle;
	}

	wgpu::CommandBuffer* operator-> () {
		return &_handle;
	}

private:
	wgpu::CommandBuffer _handle = nullptr;
};

#endif // COMMANDBUFFER_HPP
