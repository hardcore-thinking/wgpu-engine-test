#include <Helper/SamplerDescriptor.hpp>

SamplerDescriptor::SamplerDescriptor(float samplerLodMinClamp, float samplerLodMaxClamp) {
    addressModeU = wgpu::AddressMode::Repeat;
	addressModeV = wgpu::AddressMode::Repeat;
	addressModeW = wgpu::AddressMode::ClampToEdge;
	magFilter = wgpu::FilterMode::Linear;
	minFilter = wgpu::FilterMode::Linear;
	mipmapFilter = wgpu::MipmapFilterMode::Linear;
	lodMinClamp = 0.0f;
	lodMaxClamp = 8.0f;
	compare = wgpu::CompareFunction::Undefined;
	maxAnisotropy = 1;
}