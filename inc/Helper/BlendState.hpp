#ifndef BLENDSTATE_HPP
#define BLENDSTATE_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/BlendComponent.hpp>

struct BlendState : public wgpu::BlendState {
public:
	BlendState() = delete;
	BlendState(BlendComponent const& color, BlendComponent const& alpha);
};

#endif // BLENDSTATE_HPP
