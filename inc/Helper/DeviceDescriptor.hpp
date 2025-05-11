#ifndef DEVICEDESCRIPTOR_HPP
#define DEVICEDESCRIPTOR_HPP

#include <iostream>
#include <string>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Adapter.hpp>
#include <Helper/Limits.hpp>

struct DeviceDescriptor : public wgpu::DeviceDescriptor {
    public:
        DeviceDescriptor(Adapter const& adapter);
        DeviceDescriptor(Adapter const& adapter, wgpu::Limits const& limits);
    
    public:
        static int id;
};

#endif // DEVICEDESCRIPTOR_HPP