#ifndef TEXELCOPYBUFFERLAYOUT_HPP
#define TEXELCOPYBUFFERLAYOUT_HPP

#include <wgpu-native/webgpu.hpp>

struct TexelCopyBufferLayout : public wgpu::TexelCopyBufferLayout {
    TexelCopyBufferLayout() = default;
    TexelCopyBufferLayout(uint32_t bytesPerRow, uint32_t rowsPerImage);
};

#endif // TEXELCOPYBUFFERLAYOUT_HPP