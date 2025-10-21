#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <iostream>
#include <string>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Adapter.hpp>

struct Limits : public wgpu::Limits {
public:
	Limits(Adapter const& adapter);
};

#endif // LIMITS_HPP
