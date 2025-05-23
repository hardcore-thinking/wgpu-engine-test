#ifndef TEXTUREDESCRIPTOR_HPP
#define TEXTUREDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/Extent3D.hpp>

struct TextureDescriptor : public wgpu::TextureDescriptor {
    public:
        TextureDescriptor(wgpu::TextureFormat textureFormat, wgpu::TextureUsage textureUsage, Extent3D const& textureSize, std::vector<wgpu::TextureFormat> const& textureViewFormats = {});
};

#endif // TEXTUREDESCRIPTOR_HPP