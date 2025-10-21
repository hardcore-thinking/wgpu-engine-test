#ifndef RENDERPASSDEPTHSTENCILATTACHMENT_HPP
#define RENDERPASSDEPTHSTENCILATTACHMENT_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/TextureView.hpp>

struct RenderPassDepthStencilAttachment : public wgpu::RenderPassDepthStencilAttachment {
public:
	RenderPassDepthStencilAttachment() = default;
	RenderPassDepthStencilAttachment(TextureView const& textureView);
};

#endif // RENDERPASSDEPTHSTENCILATTACHMENT_HPP
