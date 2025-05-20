#include <Helper/TexelCopyTextureInfo.hpp>

TexelCopyTextureInfo::TexelCopyTextureInfo(Texture texture) {
    this->aspect = wgpu::TextureAspect::All;
    this->mipLevel = 1;
    this->origin = { 0, 0, 0 };
    this->texture = texture.Handle();
}