#ifndef RENDERPASSCOLORATTACHMENT_HPP
#define RENDERPASSCOLORATTACHMENT_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/TextureView.hpp>

struct RenderPassColorAttachment : public wgpu::RenderPassColorAttachment {
public:
	RenderPassColorAttachment() = delete;
	RenderPassColorAttachment(TextureView const& textureView);
};

#endif // RENDERPASSCOLORATTACHMENT_HPP
