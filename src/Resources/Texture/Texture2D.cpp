#include <Resources/Texture/Texture2D.hpp>

Texture2D::Texture2D(std::filesystem::path const& path, Device& device, Queue& queue, TextureDescriptor const& textureDescriptor, TextureViewDescriptor const& textureViewDescriptor) {
    int width = 0;
	int height = 0;
	int channels = 0;
    unsigned char* data = nullptr;
	try {
        unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
	    if (data == nullptr) {
            throw std::runtime_error("Failed to load texture: " + path.string());
        }

	    _texture = Texture(device, textureDescriptor);
        _textureView = TextureView(_texture, textureViewDescriptor);
    
        TexelCopyTextureInfo copyTextureInfo(_texture);
        TexelCopyBufferLayout copyBufferLayout(4 * width, height);

        queue->writeTexture(copyTextureInfo, data, textureDescriptor.size.width * textureDescriptor.size.height * 4, copyBufferLayout, textureDescriptor.size);
	    //WriteMipMaps(device, texture, textureDescriptor.size, textureDescriptor.mipLevelCount, data);

	    stbi_image_free(data);
    }

    catch (std::exception& e) {
        std::cerr << "Failed to create texture: " << e.what() << std::endl;
        stbi_image_free(data);
        throw std::runtime_error("Failed to create texture or view");
    }
}