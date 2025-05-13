#ifndef SAMPLERBINDING_HPP
#define SAMPLERBINDING_HPP

#include <iostream>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Sampler.hpp>

struct SamplerBinding : public wgpu::BindGroupEntry {
    public:
        SamplerBinding() = delete;
        SamplerBinding(uint32_t binding, Sampler& sampler);

    public:
        friend std::ostream& operator<<(std::ostream& out, SamplerBinding const& samplerBinding);
};

#endif // SAMPLERBINDING_HPP