#include <Helper/BindGroupLayout.hpp>

BindGroupLayout::BindGroupLayout(Device& device, BindGroupLayoutDescriptor const& bindGroupLayoutDescriptor) {
    _handle = device->createBindGroupLayout(bindGroupLayoutDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create bind group layout");
    }

    std::cout << "BindGroupLayout created successfully: " << Handle() << std::endl;
}

BindGroupLayout::~BindGroupLayout() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "BindGroupLayout destroyed successfully" << std::endl;
}

wgpu::BindGroupLayout* BindGroupLayout::operator->() {
    return &_handle;
}