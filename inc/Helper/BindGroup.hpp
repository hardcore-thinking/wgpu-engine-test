#ifndef BINDGROUP_HPP
#define BINDGROUP_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/BindGroupDescriptor.hpp>

class BindGroup {
    public:
        BindGroup() = delete;
        BindGroup(Device& device, BindGroupDescriptor& bindGroupDescriptor);
        ~BindGroup();

    public:
        wgpu::BindGroup& Handle() { return _handle; }
        wgpu::BindGroup const& Handle() const { return _handle; }

        wgpu::BindGroup* operator->();

    private:
        wgpu::BindGroup _handle = nullptr;
};

#endif // BINDGROUP_HPP