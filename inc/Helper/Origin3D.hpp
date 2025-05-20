#ifndef ORIGIN3D_HPP
#define ORIGIN3D_HPP

#include <wgpu-native/webgpu.hpp>

struct Origin3D : public wgpu::Origin3D {
    Origin3D(uint32_t x, uint32_t y, uint32_t z);
};

#endif // ORIGIN3D_HPP