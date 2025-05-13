#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Instance.hpp>

class Surface {
    public:
        Surface() = default;
        Surface(Instance const& instance);
        Surface(Instance const& instance, wgpu::SurfaceDescriptor const& descriptor);
        ~Surface();

    public:
        wgpu::Surface& Handle() { return _handle; }
        wgpu::Surface const& Handle() const { return _handle; }

        wgpu::Surface* operator->();
        wgpu::Surface const* operator->() const;

    protected:
        static int _id;
        wgpu::Surface _handle = nullptr;
};

#endif // SURFACE_HPP