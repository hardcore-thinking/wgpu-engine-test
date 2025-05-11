#ifndef TEXTUREDESCRIPTOR_HPP
#define TEXTUREDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

struct TextureDescriptor : public wgpu::TextureDescriptor {
    public:
        TextureDescriptor(wgpu::TextureFormat textureFormat, wgpu::TextureUsage textureUsage, wgpu::Extent3D const& textureSize, std::vector<wgpu::TextureFormat> const& textureViewFormats);
};

#endif // TEXTUREDESCRIPTOR_HPP