#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/SamplerDescriptor.hpp>

class Sampler {
    public:
        Sampler() = delete;
        Sampler(Device& device, SamplerDescriptor const& samplerDescriptor);
        ~Sampler();

    public:
        wgpu::Sampler& Handle() { return _handle; }
        wgpu::Sampler const& Handle() const { return _handle; }

        wgpu::Sampler* operator->();

    private:
        wgpu::Sampler _handle = nullptr;
};

#endif // SAMPLER_HPP