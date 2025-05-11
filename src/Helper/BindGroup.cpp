#include <Helper/BindGroup.hpp>

BindGroup::BindGroup(Device& device, BindGroupDescriptor& bindGroupDescriptor) {
    _handle = device.Handle().createBindGroup(bindGroupDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create bind group.");
    }

    std::cout << "Bind group created successfully: " << Handle() << std::endl;
}

BindGroup::~BindGroup() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "Bind group destroyed successfully" << std::endl;
}