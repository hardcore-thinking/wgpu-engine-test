#ifndef PRIMITIVESTATE_HPP
#define PRIMITIVESTATE_HPP

#include <wgpu-native/webgpu.hpp>

struct PrimitiveState : public wgpu::PrimitiveState {
public:
	PrimitiveState();
};

#endif // PRIMITIVESTATE_HPP
