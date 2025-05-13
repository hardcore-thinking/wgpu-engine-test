#include <Helper/Sampler.hpp>

Sampler::Sampler(Device& device, SamplerDescriptor const& samplerDescriptor) {
    _handle = device->createSampler(samplerDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create sampler.");
    }

    std::cout << "Created sampler successfully: " << Handle() << std::endl;
}

Sampler::~Sampler() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "Destroyed sampler successfully" << std::endl;
}