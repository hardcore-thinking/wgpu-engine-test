#ifndef SAMPLERDESCRIPTOR_HPP
#define SAMPLERDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

struct SamplerDescriptor : public wgpu::SamplerDescriptor {
    public:
        SamplerDescriptor() = delete;
        SamplerDescriptor(float samplerLodMinClamp, float samplerLodMaxClamp);
};

#endif // SAMPLERDESCRIPTOR_HPP