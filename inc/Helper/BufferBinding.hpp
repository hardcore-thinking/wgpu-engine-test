#ifndef BUFFERBINDING_HPP
#define BUFFERBINDING_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Buffer.hpp>

struct BufferBinding : public wgpu::BindGroupEntry {
public:
	BufferBinding() = delete;
	BufferBinding(uint32_t bufferBinding, Buffer& bufferHandle, uint32_t bufferSize, uint32_t bufferOffset);

public:
	friend std::ostream& operator<<(std::ostream& out, BufferBinding const& bufferBinding);
};

#endif // BUFFERBINDING_HPP
