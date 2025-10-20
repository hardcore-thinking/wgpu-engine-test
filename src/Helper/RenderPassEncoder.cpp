#include <Helper/RenderPassEncoder.hpp>

RenderPassEncoder::RenderPassEncoder(CommandEncoder& commandEncoder, RenderPassDescriptor const& renderPassDescriptor) {
	_handle = commandEncoder->beginRenderPass(renderPassDescriptor);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create render pass encoder");
	}

	//std::cout << "Render pass encoder created successfully: " << Handle() << std::endl;
}

RenderPassEncoder::~RenderPassEncoder() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Render pass encoder destroyed successfully" << std::endl;
}

wgpu::RenderPassEncoder* RenderPassEncoder::operator -> () {
	return &_handle;
}
