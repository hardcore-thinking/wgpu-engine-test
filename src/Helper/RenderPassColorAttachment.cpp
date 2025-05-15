#include <Helper/RenderPassColorAttachment.hpp>

RenderPassColorAttachment::RenderPassColorAttachment(TextureView const& textureView) {
    clearValue = wgpu::Color{ 0.05f, 0.05f, 0.05f, 1.0f };
    depthSlice = 0;
    loadOp = wgpu::LoadOp::Clear;
    resolveTarget = nullptr;
    storeOp = wgpu::StoreOp::Store;
    view = textureView.Handle();
	nextInChain = nullptr;
}