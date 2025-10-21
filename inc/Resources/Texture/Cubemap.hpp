#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <array>

#include <wgpu-native/webgpu.hpp>

#include <Resources/Texture/Texture2D.hpp>
#include <Helper/Device.hpp>
#include <Helper/Queue.hpp>
#include <Helper/TextureDescriptor.hpp>
#include <Helper/TextureViewDescriptor.hpp>
#include <Helper/Texture.hpp>
#include <Helper/TextureView.hpp>
#include <Helper/Origin3D.hpp>
#include <Helper/Extent3D.hpp>

class Cubemap {
public:
	Cubemap(std::array<std::filesystem::path, 6> const& texturePaths, Device& device, Queue& queue, TextureDescriptor& textureDescriptor, TextureViewDescriptor const& textureViewDescriptor);

public:
	Texture2D* operator[](size_t index);
	Texture2D const* operator[](size_t index) const;

	TextureView const& View() const {
		return _textureView;
	}

	TextureView& View() {
		return _textureView;
	}

private:
	std::array<Texture2D, 6> _textures {};
	Texture _texture {};
	TextureView _textureView {};
};

#endif // CUBEMAP_HPP
