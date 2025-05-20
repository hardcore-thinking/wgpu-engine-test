#ifndef TEXELCOPYTEXTUREINFO_HPP
#define TEXELCOPYTEXTUREINFO_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/Texture.hpp>
#include <Helper/Origin3D.hpp>

struct TexelCopyTextureInfo : public wgpu::TexelCopyTextureInfo {
    TexelCopyTextureInfo() = default;
    TexelCopyTextureInfo(Texture texture);
};

#endif // TEXELCOPYTEXTUREINFO_HPP