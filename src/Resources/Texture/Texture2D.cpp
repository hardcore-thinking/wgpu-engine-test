#include <Resources/Texture/Texture2D.hpp>

Texture2D::Texture2D(std::filesystem::path const& path, Device& device, Queue& queue, TextureDescriptor& textureDescriptor, TextureViewDescriptor const& textureViewDescriptor) {
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* data = nullptr;
	try {
		data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
		if (data == nullptr) {
			throw std::runtime_error("Failed to load texture: " + path.string());
		}

		_texture = std::move(Texture(device, textureDescriptor));
		_textureView = std::move(TextureView(_texture, textureViewDescriptor));

		TexelCopyTextureInfo copyTextureInfo(_texture);
		TexelCopyBufferLayout copyBufferLayout(4 * width, height);

		Extent3D writeSize(width, height, 1);
		queue->writeTexture(copyTextureInfo, data, 4 * width * height, copyBufferLayout, writeSize);
		//WriteMipMaps(device, texture, textureDescriptor.size, textureDescriptor.mipLevelCount, data);

		stbi_image_free(data);
	}

	catch (std::exception const& e) {
		std::cerr << "Failed to create texture: " << e.what() << std::endl;
		stbi_image_free(data);
		throw std::runtime_error("Failed to create texture or view");
	}
}

Texture2D::Texture2D(Texture2D&& other) : _texture(std::move(other._texture)), _textureView(std::move(other._textureView)) {}
