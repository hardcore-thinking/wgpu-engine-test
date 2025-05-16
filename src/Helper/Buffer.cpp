#include <Helper/Buffer.hpp>

Buffer::Buffer(Device& device, BufferDescriptor const& bufferDescriptor) {
    _handle = device->createBuffer(bufferDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create buffer.");
    }

    //std::cout << "Buffer '" << bufferDescriptor.label.data << "' created successfully: " << Handle() << std::endl;
}

Buffer::~Buffer() {
    if (_handle != nullptr) {
        _handle.destroy();
        _handle.release();
        _handle = nullptr;
    }

    //std::cout << "Buffer destroyed successfully" << std::endl;
}

wgpu::Buffer* Buffer::operator->() {
    return &_handle;
}

wgpu::Buffer const* Buffer::operator->() const {
    return &_handle;
}