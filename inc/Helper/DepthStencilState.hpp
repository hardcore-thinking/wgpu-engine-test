#ifndef DEPTHSTENCILSTATE_HPP
#define DEPTHSTENCILSTATE_HPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/StencilFaceState.hpp>

struct DepthStencilState : public wgpu::DepthStencilState {
    public:
        DepthStencilState();
        DepthStencilState(StencilFaceState const& stencilFrontState, StencilFaceState const& stencilBackState, wgpu::TextureFormat depthTextureFormat);
};

#endif // DEPTHSTENCILSTATE_HPP