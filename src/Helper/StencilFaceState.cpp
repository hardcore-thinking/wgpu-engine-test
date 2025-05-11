#include <Helper/StencilFaceState.hpp>

StencilFaceState::StencilFaceState() {
    compare = wgpu::CompareFunction::Always;
    depthFailOp = wgpu::StencilOperation::Keep;
    failOp = wgpu::StencilOperation::Keep;
    passOp = wgpu::StencilOperation::Keep;
}