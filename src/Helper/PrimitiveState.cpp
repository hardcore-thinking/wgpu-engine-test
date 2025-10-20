#include <Helper/PrimitiveState.hpp>

PrimitiveState::PrimitiveState() {
	cullMode = wgpu::CullMode::Back;
	frontFace = wgpu::FrontFace::CCW;
	stripIndexFormat = wgpu::IndexFormat::Undefined;
	topology = wgpu::PrimitiveTopology::TriangleList;
	unclippedDepth = false;
	nextInChain = nullptr;
}
