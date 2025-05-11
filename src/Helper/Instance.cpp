#include <Helper/Instance.hpp>

Instance::Instance() {
    wgpu::InstanceDescriptor descriptor {};
    descriptor.nextInChain = nullptr;
    descriptor.features.timedWaitAnyEnable = false;
    descriptor.features.timedWaitAnyMaxCount = 0;
    descriptor.features.nextInChain = nullptr;

    _handle = wgpu::createInstance(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU instance");
    }

    std::cout << "Instance created successfully: " << Handle() << std::endl;
}

Instance::Instance(wgpu::InstanceDescriptor const& descriptor) {
    _handle = wgpu::createInstance(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU instance");
    }

    std::cout << "Instance created successfully: " << Handle() << std::endl;
}

Instance::~Instance() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "Instance destroyed successfully" << std::endl;
}