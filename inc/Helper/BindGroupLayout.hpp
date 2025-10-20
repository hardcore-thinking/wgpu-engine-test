#ifndef BINDGROUPLAYOUT_HPP
#define BINDGROUPLAYOUT_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/BindGroupLayoutDescriptor.hpp>

class BindGroupLayout {
public:
	BindGroupLayout() = delete;
	BindGroupLayout(Device& device, BindGroupLayoutDescriptor const& bindGroupLayoutDescriptor);
	~BindGroupLayout();

public:
	wgpu::BindGroupLayout& Handle() {
		return _handle;
	}
	wgpu::BindGroupLayout const& Handle() const {
		return _handle;
	}

	wgpu::BindGroupLayout* operator -> ();
	wgpu::BindGroupLayout const* operator -> () const;

private:
	wgpu::BindGroupLayout _handle = nullptr;
};

#endif // BINDGROUPLAYOUT_HPP
