#ifndef BUFFERDESCRIPTOR_HPP
#define BUFFERDESCRIPTOR_HPP

#include <iostream>
#include <string>

#include <wgpu-native/webgpu.hpp>

struct BufferDescriptor : public wgpu::BufferDescriptor {
    public:
        BufferDescriptor() = default;
        BufferDescriptor(size_t size, wgpu::BufferUsage usage);

    public:
        static int counter;
        std::string newLabel = "buffer";
};

#endif // BUFFERDESCRIPTOR_HPP