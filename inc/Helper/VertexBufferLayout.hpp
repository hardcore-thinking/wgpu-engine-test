#ifndef VERTEXBUFFERLAYOUT_HPP
#define VERTEXBUFFERLAYOUT_HPP

#include <vector>

#include <wgpu-native/webgpu.hpp>

#include <Helper/VertexAttribute.hpp>

struct VertexBufferLayout : public wgpu::VertexBufferLayout {
    public:
        VertexBufferLayout() = delete;
        VertexBufferLayout(uint32_t bufferArrayStride, std::vector<VertexAttribute> const& attributesArray);
};

#endif // VERTEXBUFFERLAYOUT_HPP