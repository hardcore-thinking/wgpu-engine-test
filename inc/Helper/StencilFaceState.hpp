#ifndef STENCILFACESTATE_HPP
#define STENCILFACESTATE_HPP

#include <wgpu-native/webgpu.hpp>

struct StencilFaceState : public wgpu::StencilFaceState {
    public:
        StencilFaceState();
};

#endif // STENCILFACESTATE_HPP