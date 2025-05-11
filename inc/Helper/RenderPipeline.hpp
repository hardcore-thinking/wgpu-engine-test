#ifndef RENDERPIPELINE_HPP
#define RENDERPIPELINE_HPP

#include <iostream>
#include <string>
#include <cassert>

#include <wgpu-native/webgpu.hpp>

wgpu::RenderPipelineDescriptor CreateRenderPipelineDescriptor(wgpu::PipelineLayout pipelineLayout, wgpu::ShaderModule vertexShaderModule, wgpu::ShaderModule fragmentShaderModule);

#endif // RENDERPIPELINE_HPP