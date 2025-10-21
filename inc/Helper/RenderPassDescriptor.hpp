#ifndef RENDERPASSDESCRIPTOR_HPP
#define RENDERPASSDESCRIPTOR_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

#include <Helper/RenderPassColorAttachment.hpp>
#include <Helper/RenderPassDepthStencilAttachment.hpp>
#include <Helper/QuerySet.hpp>

struct RenderPassDescriptor : public wgpu::RenderPassDescriptor {
public:
	RenderPassDescriptor() = delete;
	RenderPassDescriptor(std::vector<RenderPassColorAttachment> const& colorAttachments, RenderPassDepthStencilAttachment const& depthStencilAttachment);
};

#endif // RENDERPASSDESCRIPTOR_HPP
