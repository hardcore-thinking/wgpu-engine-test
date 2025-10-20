#include <Helper/RenderPipeline.hpp>

RenderPipeline::RenderPipeline(Device& device, RenderPipelineDescriptor const& renderPipelineDescriptor) {
	_handle = device->createRenderPipeline(renderPipelineDescriptor);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create render pipeline");
	}

	//std::cout << "Render pipeline created successfully: " << Handle() << std::endl;
}

RenderPipeline::~RenderPipeline() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Render pipeline released successfully" << std::endl;
}

wgpu::RenderPipeline* RenderPipeline::operator->() {
	return &_handle;
}
