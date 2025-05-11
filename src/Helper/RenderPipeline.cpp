#include <Helper/RenderPipeline.hpp>

wgpu::RenderPipelineDescriptor CreateRenderPipelineDescriptor(wgpu::PipelineLayout pipelineLayout, wgpu::ShaderModule vertexShaderModule, wgpu::ShaderModule fragmentShaderModule) {
	static int id = 0;

	wgpu::RenderPipelineDescriptor renderPipelineDescriptor {};

	wgpu::DepthStencilState depthStencilState {};
	depthStencilState.depthBias = 0;
	depthStencilState.depthBiasClamp = 0.0f;
	depthStencilState.depthBiasSlopeScale = 0.0f;
	depthStencilState.depthCompare = wgpu::CompareFunction::LessEqual;
	depthStencilState.depthWriteEnabled = wgpu::OptionalBool::True;
	depthStencilState.format = wgpu::TextureFormat::Depth24Plus;
	depthStencilState.nextInChain = nullptr;
	depthStencilState.stencilBack.failOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilBack.depthFailOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilBack.passOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilBack.compare = wgpu::CompareFunction::Always;
	depthStencilState.stencilFront.failOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilFront.depthFailOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilFront.passOp = wgpu::StencilOperation::Keep;
	depthStencilState.stencilFront.compare = wgpu::CompareFunction::Always;
	depthStencilState.stencilReadMask = 0;
	depthStencilState.stencilWriteMask = 0;
	renderPipelineDescriptor.depthStencil = &depthStencilState;

	wgpu::FragmentState fragmentState {};
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;
	fragmentState.entryPoint = wgpu::StringView("frag_main");
	fragmentState.module = fragmentShaderModule;
	fragmentState.nextInChain = nullptr;
	fragmentState.targetCount = 0;
	fragmentState.targets = nullptr;
	renderPipelineDescriptor.fragment = &fragmentState;

	renderPipelineDescriptor.label = wgpu::StringView(("render_pipeline_" + std::to_string(id)).c_str());

	renderPipelineDescriptor.layout = pipelineLayout;

	wgpu::MultisampleState multisampleState {};
	multisampleState.alphaToCoverageEnabled = false;
	multisampleState.count = 1;
	multisampleState.mask = 0xFFFFFFFF;
	multisampleState.nextInChain = nullptr;
	renderPipelineDescriptor.multisample = multisampleState;

	renderPipelineDescriptor.nextInChain = nullptr;

	wgpu::PrimitiveState primitiveState {};
	primitiveState.cullMode = wgpu::CullMode::Back;
	primitiveState.frontFace = wgpu::FrontFace::CCW;
	primitiveState.nextInChain = nullptr;
	primitiveState.stripIndexFormat = wgpu::IndexFormat::Uint16;
	primitiveState.topology = wgpu::PrimitiveTopology::TriangleList;
	renderPipelineDescriptor.primitive = primitiveState;

	wgpu::VertexState vertexState {};
	vertexState.bufferCount = 0;
	vertexState.buffers = nullptr;
	vertexState.constantCount = 0;
	vertexState.constants = nullptr;
	vertexState.entryPoint = wgpu::StringView("vert_main");
	vertexState.module = vertexShaderModule;
	vertexState.nextInChain = nullptr;
	renderPipelineDescriptor.vertex = vertexState;

	return renderPipelineDescriptor;
}

wgpu::RenderPipeline CreateRenderPipeline2(wgpu::Device& device, wgpu::RenderPipelineDescriptor const& descriptor) {
    assert (device != nullptr && "Device is nullptr");
	return device.createRenderPipeline(descriptor);
}