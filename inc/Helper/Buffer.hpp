#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/BufferDescriptor.hpp>

class Buffer {
    public:
        Buffer() = delete;
        Buffer(Device& device, BufferDescriptor const& bufferDescriptor);
        ~Buffer();

    public:
        wgpu::Buffer& Handle() { return _handle; }
        wgpu::Buffer const& Handle() const { return _handle; }

        wgpu::Buffer* operator->();
        wgpu::Buffer const* operator->() const;

    private:
        wgpu::Buffer _handle = nullptr;
};

#endif // BUFFER_HPP