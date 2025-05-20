#ifndef EXTENT3D_HPP
#define EXTENT3D_HPP

#include <wgpu-native/webgpu.hpp>

struct Extent3D : public wgpu::Extent3D {
    Extent3D(uint32_t width, uint32_t height, uint32_t depth);
};

#endif // EXTENT3D_HPP