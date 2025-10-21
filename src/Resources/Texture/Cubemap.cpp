#include <Resources/Texture/Cubemap.hpp>

Cubemap::Cubemap(std::array<std::filesystem::path, 6> const& texturePaths, Device& device, Queue& queue, TextureDescriptor& textureDescriptor, TextureViewDescriptor const& textureViewDescriptor) {
	std::array<uint8_t*, 6> pixelData;
	try {
		Extent3D cubemapSize = { 0, 0, 6 };
		for (uint32_t layer = 0; layer < 6; ++layer) {
			int width = 0;
			int height = 0;
			int channels = 0;
			pixelData[layer] = stbi_load(texturePaths[layer].string().c_str(), &width, &height, &channels, 4);
			if (pixelData[layer] == nullptr) {
				throw std::runtime_error("Failed to load texture: " + texturePaths[layer].string());
			}

			if (layer == 0) {
				cubemapSize.width = (uint32_t) (width);
				cubemapSize.height = (uint32_t) (height);
			}

			else {
				if (cubemapSize.width != (uint32_t) (width) || cubemapSize.height != (uint32_t) (height)) {
					throw std::runtime_error("All cubemap faces must have the same size!");
				}
			}
		}

		_texture = std::move(Texture(device, textureDescriptor));

		Extent3D cubemapLayerSize = { cubemapSize.width, cubemapSize.height, 1 };
		for (uint32_t layer = 0; layer < 6; ++layer) {
			Origin3D origin = { 0, 0, layer };

			size_t dataSize = 4 * cubemapLayerSize.width * cubemapLayerSize.height;
			TexelCopyTextureInfo copyTextureInfo(_texture);
			copyTextureInfo.origin = origin;
			TexelCopyBufferLayout copyBufferLayout(4 * cubemapLayerSize.width, cubemapLayerSize.height);

			Extent3D writeSize(cubemapLayerSize.width, cubemapLayerSize.height, 1);
			queue->writeTexture(copyTextureInfo, pixelData[layer], dataSize, copyBufferLayout, writeSize);

			stbi_image_free(pixelData[layer]);
		}

		_textureView = std::move(TextureView(_texture, textureViewDescriptor));
	}

	catch (std::exception const& e) {
		std::cerr << "Failed to create texture: " << e.what() << std::endl;
		for (auto& data : pixelData) {
			if (data != nullptr) {
				stbi_image_free(data);
			}
		}

		throw std::runtime_error("Failed to create texture or view");
	}
}
