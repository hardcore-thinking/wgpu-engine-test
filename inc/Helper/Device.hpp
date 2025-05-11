#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Adapter.hpp>
#include <Helper/DeviceDescriptor.hpp>

class Device {
    public:
        Device() = default;
        Device(Adapter& adapter, wgpu::DeviceDescriptor const& descriptor);
        Device(Adapter& adapter, DeviceDescriptor const& descriptor);
        ~Device() = default;

    public:
        wgpu::Device& Handle() { return _handle; }
        wgpu::Limits const& Limits() const { return _limits; }
        void DisplayLimits() const;

    private:
        wgpu::Device _handle = nullptr;
        wgpu::Limits _limits {};
    };

#endif // DEVICE_HPP