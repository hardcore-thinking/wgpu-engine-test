#include <Helper/RenderPassDescriptor.hpp>

RenderPassDescriptor::RenderPassDescriptor(std::vector<RenderPassColorAttachment> const& colorAttachmentsArray, RenderPassDepthStencilAttachment const& depthStencilAttachmentHandle) {
	colorAttachmentCount = static_cast<uint32_t>(colorAttachmentsArray.size());
	colorAttachments = colorAttachmentsArray.data();
	depthStencilAttachment = &depthStencilAttachmentHandle;
	occlusionQuerySet = nullptr;
	timestampWrites = nullptr;
	nextInChain = nullptr;
}
