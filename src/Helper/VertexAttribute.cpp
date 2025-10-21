#include <Helper/VertexAttribute.hpp>

VertexAttribute::VertexAttribute(uint32_t attributeLocation, wgpu::VertexFormat attributeFormat, uint32_t attributeOffset) {
	shaderLocation = attributeLocation;
	format = attributeFormat;
	offset = attributeOffset;
}
