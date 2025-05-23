#include <Helper/TexelCopyTextureInfo.hpp>

TexelCopyTextureInfo::TexelCopyTextureInfo(Texture const& texture) {
    this->aspect = wgpu::TextureAspect::All;
    this->mipLevel = 0;
    this->origin = { 0, 0, 0 };
    this->texture = texture.Handle();
}