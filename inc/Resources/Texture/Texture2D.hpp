#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <stdexcept>

#include <stb_image.h>

#include <Helper/Device.hpp>
#include <Helper/Queue.hpp>
#include <Helper/TextureDescriptor.hpp>
#include <Helper/Texture.hpp>
#include <Helper/TextureViewDescriptor.hpp>
#include <Helper/TextureView.hpp>
#include <Helper/Origin3D.hpp>
#include <Helper/Extent3D.hpp>
#include <Helper/TexelCopyTextureInfo.hpp>
#include <Helper/TexelCopyBufferLayout.hpp>

class Texture2D {
public:
	Texture2D() = default;
	Texture2D(std::filesystem::path const& path, Device& device, Queue& queue, TextureDescriptor& textureDescriptor, TextureViewDescriptor const& textureViewDescriptor);
	Texture2D(Texture2D const& texture2D) = delete;
	Texture2D(Texture2D&& other);
	~Texture2D() = default;

	Texture2D& operator=(Texture2D const& texture2D) = delete;
	Texture2D& operator=(Texture2D&& other) = default;

	Texture const& Handle() const {
		return _texture;
	}
	TextureView const& View() const {
		return _textureView;
	}

	Texture& Handle() {
		return _texture;
	}
	TextureView& View() {
		return _textureView;
	}

private:
	Texture _texture;
	TextureView _textureView;
};

#endif // TEXTURE2D_HPP
