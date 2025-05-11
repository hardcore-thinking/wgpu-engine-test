#ifndef VERTEXATTRIBUTE_HPP
#define VERTEXATTRIBUTE_HPP

#include <wgpu-native/webgpu.hpp>

struct VertexAttribute : public wgpu::VertexAttribute {
    public:
        VertexAttribute() = delete;
        VertexAttribute(uint32_t attributeLocation, wgpu::VertexFormat attributeFormat, uint32_t attributeOffset);
};

#endif // VERTEXATTRIBUTE_HPP