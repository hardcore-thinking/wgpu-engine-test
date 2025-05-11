#include <Helper/Limits.hpp>

Limits::Limits(Adapter const& adapter) {
    wgpu::Limits adapterSupportedLimits = adapter.Limits();
		maxBindGroups = 1;
		maxBindGroupsPlusVertexBuffers = 0;
		maxBindingsPerBindGroup = 3;
		maxBufferSize = 0; // 150000 * sizeof(VertexAttributes);
		maxColorAttachmentBytesPerSample = 0;
		maxColorAttachments = 0;
		maxComputeInvocationsPerWorkgroup = 0;
		maxComputeWorkgroupSizeX = 0;
		maxComputeWorkgroupSizeY = 0;
		maxComputeWorkgroupSizeZ = 0;
		maxComputeWorkgroupsPerDimension = 0;
		maxComputeWorkgroupStorageSize = 0;
		maxDynamicStorageBuffersPerPipelineLayout = 0;
		maxDynamicUniformBuffersPerPipelineLayout = 0;
		maxInterStageShaderVariables = 0;
		maxSampledTexturesPerShaderStage = 3;
		maxSamplersPerShaderStage = 1;
		maxStorageBufferBindingSize = 0;
		maxStorageBuffersPerShaderStage = 0;
		maxStorageTexturesPerShaderStage = 0;
		maxTextureArrayLayers = 1;
		maxTextureDimension1D = 2048;
		maxTextureDimension2D = 2048;
		maxTextureDimension3D = 0;
		maxUniformBufferBindingSize = static_cast<uint64_t>(16ULL * 4 * sizeof(float));
		maxUniformBuffersPerShaderStage = 1;
		maxVertexAttributes = 4;
		maxVertexBufferArrayStride = 0; //sizeof(VertexAttributes);
		maxVertexBuffers = 1;
		minStorageBufferOffsetAlignment = adapterSupportedLimits.minStorageBufferOffsetAlignment;
		minUniformBufferOffsetAlignment = adapterSupportedLimits.minUniformBufferOffsetAlignment;
        nextInChain = nullptr;
}