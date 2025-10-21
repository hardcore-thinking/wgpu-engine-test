#include <Helper/VertexBufferLayout.hpp>

VertexBufferLayout::VertexBufferLayout(uint32_t bufferArrayStride, std::vector<VertexAttribute> const& attributesArray) {
	attributeCount = attributesArray.size();
	attributes = (WGPUVertexAttribute*) (attributesArray.data());
	arrayStride = bufferArrayStride;
	stepMode = wgpu::VertexStepMode::Vertex;
}
