#include <Helper/CommandEncoder.hpp>

CommandEncoder::CommandEncoder(Device& device, CommandEncoderDescriptor const& descriptor) {
    _handle = device->createCommandEncoder(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create a command encoder");
    }

    std::cout << "Command encoder created successfully " << Handle() << std::endl;
}

CommandEncoder::~CommandEncoder() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }
    
    std::cout << "Command encoder destroyed successfully " << std::endl;
}

wgpu::CommandEncoder* CommandEncoder::operator -> () {
    return &_handle;
}

wgpu::CommandEncoder const* CommandEncoder::operator -> () const {
    return &_handle;
}