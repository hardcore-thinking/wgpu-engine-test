#include <Helper/BlendComponent.hpp>

BlendComponent::BlendComponent(wgpu::BlendFactor srcBlendFactor, wgpu::BlendFactor dstBlendFactor, wgpu::BlendOperation blendOperation) {
    srcFactor = srcBlendFactor;
    dstFactor = dstBlendFactor;
    operation = blendOperation;
}