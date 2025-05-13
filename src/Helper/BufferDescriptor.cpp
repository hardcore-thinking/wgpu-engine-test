#include <Helper/BufferDescriptor.hpp>

BufferDescriptor::BufferDescriptor(size_t bufferSize, wgpu::BufferUsage bufferUsage) {
    newLabel = "buffer" + std::to_string(counter++);
    label = wgpu::StringView(newLabel);
    size = bufferSize;
    usage = bufferUsage;
    mappedAtCreation = false;
}

int BufferDescriptor::counter = 0;