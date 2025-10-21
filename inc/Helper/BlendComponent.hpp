#ifndef BLENDCOMPONENT_HPP
#define BLENDCOMPONENT_HPP

#include <wgpu-native/webgpu.hpp>

struct BlendComponent : public wgpu::BlendComponent {
public:
	BlendComponent() = default;
	BlendComponent(wgpu::BlendFactor srcBlendFactor, wgpu::BlendFactor dstBlendFactor, wgpu::BlendOperation blendOperation);
};

#endif // BLENDCOMPONENT_HPP
