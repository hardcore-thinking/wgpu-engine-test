#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

class Instance {
    public:
        Instance();
        Instance(wgpu::InstanceDescriptor const& descriptor);
        ~Instance();

    public:
        wgpu::Instance& Handle() { return _handle; }
        wgpu::Instance const& Handle() const { return _handle; }

        wgpu::Instance* operator->();
        wgpu::Instance const* operator->() const;

    private:
        wgpu::Instance _handle = nullptr;
};

#endif // INSTANCE_HPP