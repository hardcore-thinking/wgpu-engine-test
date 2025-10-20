#ifndef BINDGROUPDESCRIPTOR_HPP
#define BINDGROUPDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/BindGroupLayout.hpp>

using BindGroupEntry = wgpu::BindGroupEntry;

struct BindGroupDescriptor : wgpu::BindGroupDescriptor {
public:
	BindGroupDescriptor() = delete;
	BindGroupDescriptor(BindGroupLayout& bindGroupLayout, std::vector<BindGroupEntry> const& entries);

public:
	static int counter;
};

#endif // BINDGROUPDESCRIPTOR_HPP
