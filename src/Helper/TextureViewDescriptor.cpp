#include <Helper/TextureViewDescriptor.hpp>

TextureViewDescriptor::TextureViewDescriptor(wgpu::TextureAspect textureAspect, wgpu::TextureFormat textureFormat) {
    aspect = textureAspect;
    format = textureFormat;
    dimension = wgpu::TextureViewDimension::_2D;
    baseMipLevel = 0;
    mipLevelCount = 1;
    baseArrayLayer = 0;
    arrayLayerCount = 1;
    nextInChain = nullptr;
}