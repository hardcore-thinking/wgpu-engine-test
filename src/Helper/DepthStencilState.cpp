#include <Helper/DepthStencilState.hpp> 

DepthStencilState::DepthStencilState() {
    depthBias = 0;
	depthBiasClamp = 0.0f;
	depthBiasSlopeScale = 0.0f;
    depthCompare = wgpu::CompareFunction::Less;
    depthWriteEnabled = wgpu::OptionalBool::True;
    format = wgpu::TextureFormat::Depth24Plus;
    nextInChain = nullptr;
    stencilFront = StencilFaceState {};
    stencilBack = StencilFaceState {};
    stencilReadMask = 0;
    stencilWriteMask = 0;
    
}

DepthStencilState::DepthStencilState(StencilFaceState const& stencilFrontState, StencilFaceState const& stencilBackState, wgpu::TextureFormat depthTextureFormat) {
    depthBias = 0;
	depthBiasClamp = 0.0f;
	depthBiasSlopeScale = 0.0f;
	depthCompare = wgpu::CompareFunction::Less;
	depthWriteEnabled = wgpu::OptionalBool::True;
	format = depthTextureFormat;
	nextInChain = nullptr;
	stencilBack = stencilBackState;
	stencilFront = stencilFrontState;
	stencilReadMask = 0;
	stencilWriteMask = 0;
}