#ifndef COLORTARGETSTATE_HPP
#define COLORTARGETSTATE_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/Adapter.hpp>
#include <Helper/CompatibleSurface.hpp>
#include <Helper/BlendState.hpp>

struct ColorTargetState : public wgpu::ColorTargetState {
public:
	ColorTargetState() = delete;
	ColorTargetState(Adapter& adapter, CompatibleSurface& compatibleSurface, BlendState const& blendState);
};

#endif // COLORTARGETSTATE_HPP	
