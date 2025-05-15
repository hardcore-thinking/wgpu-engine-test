#include <Helper/RenderPassDepthStencilAttachment.hpp>

RenderPassDepthStencilAttachment::RenderPassDepthStencilAttachment(TextureView const& textureView) {
	depthClearValue = 1.0f;
    depthLoadOp = wgpu::LoadOp::Clear;
    depthReadOnly = false;
    depthStoreOp = wgpu::StoreOp::Store;
    stencilClearValue = 0;
    stencilLoadOp = wgpu::LoadOp::Clear;
    stencilReadOnly = true;
    stencilStoreOp = wgpu::StoreOp::Store;
    view = textureView.Handle();
}