#ifndef TEXTUREVIEWDESCRIPTOR_HPP
#define TEXTUREVIEWDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

struct TextureViewDescriptor : public wgpu::TextureViewDescriptor {
public:
	TextureViewDescriptor(wgpu::TextureAspect aspect, wgpu::TextureFormat format);
};

#endif // TEXTUREVIEWDESCRIPTOR_HPP
