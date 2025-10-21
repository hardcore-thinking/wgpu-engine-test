#ifndef PIPELINELAYOUT_HPP
#define PIPELINELAYOUT_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/PipelineLayoutDescriptor.hpp>

class PipelineLayout {
public:
	PipelineLayout() = delete;
	PipelineLayout(Device& device, PipelineLayoutDescriptor const& pipelineLayoutDescriptor);
	~PipelineLayout();

	wgpu::PipelineLayout& Handle() {
		return _handle;
	}
	wgpu::PipelineLayout const& Handle() const {
		return _handle;
	}

	wgpu::PipelineLayout* operator->();

private:
	wgpu::PipelineLayout _handle = nullptr;
};

#endif // PIPELINELAYOUT_HPP
