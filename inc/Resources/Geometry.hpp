#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <filesystem>
#include <fstream>
#include <vector>
#include <cstring>

#include <tiny_obj_loader.h>

#include <stb_image.h>

#include <Helper/Device.hpp>
#include <Helper/Queue.hpp>
#include <Helper/CompatibleSurface.hpp>
#include <Helper/TextureDescriptor.hpp>
#include <Helper/Texture.hpp>
#include <Helper/TextureView.hpp>
#include <Helper/VertexAttribute.hpp>

#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>

struct VertexAttributes {
	Math::Vector3 position {};
	Math::Vector3 normal {};
	Math::Vector3 color {};
	Math::Vector2 uv {};
};

void WriteMipMaps(wgpu::Device device, wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount, unsigned char* const data);

wgpu::Texture LoadTexture(std::filesystem::path const& path, wgpu::Device device, wgpu::TextureView* pTextureView = nullptr);

bool LoadGeometry(std::filesystem::path const& path, std::vector<float>& pointData, std::vector<uint16_t>& indexData, int dimensions);
bool LoadGeometryFromOBJ(std::filesystem::path const& path, std::vector<VertexAttributes>& vertexData);

#endif // GEOMETRY_HPP