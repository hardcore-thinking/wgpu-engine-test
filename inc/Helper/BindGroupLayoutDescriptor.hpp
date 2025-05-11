#ifndef BINDGROUPLAYOUTDESCRIPTOR_HPP
#define BINDGROUPLAYOUTDESCRIPTOR_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

using BindGroupLayoutEntry = wgpu::BindGroupLayoutEntry;

struct BindGroupLayoutDescriptor : public wgpu::BindGroupLayoutDescriptor {
    public:
        BindGroupLayoutDescriptor() = delete;
        BindGroupLayoutDescriptor(std::vector<BindGroupLayoutEntry> const& entries);
};

#endif // BINDGROUPLAYOUTDESCRIPTOR_HPP