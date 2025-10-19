#include <Resources/Geometry/Geometry.hpp>

void WriteMipMaps(wgpu::Device device, wgpu::Texture texture, wgpu::Extent3D textureSize, uint32_t mipLevelCount, unsigned char *const data)
{
	(void)device;

	wgpu::Queue queue = device.getQueue();

	wgpu::TexelCopyTextureInfo destination{};
	destination.texture = texture;
	destination.origin = {0, 0, 0};
	destination.aspect = wgpu::TextureAspect::All;

	wgpu::TexelCopyBufferLayout source{};
	source.offset = 0;

	wgpu::Extent3D mipLevelSize = textureSize;
	std::vector<uint8_t> previousLevelPixels;
	wgpu::Extent3D previousMipLevelSize;
	for (uint32_t level = 0; level < mipLevelCount; ++level)
	{
		std::vector<uint8_t> pixels(4 * mipLevelSize.width * mipLevelSize.height);

		if (level == 0)
		{
			std::memcpy(pixels.data(), data, pixels.size());
		}

		else
		{
			for (uint32_t i = 0; i < mipLevelSize.width; ++i)
			{
				for (uint32_t j = 0; j < mipLevelSize.height; ++j)
				{
					uint8_t *p = &pixels[4 * (j * mipLevelSize.width + i)];

					uint8_t *p00 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 0))];
					uint8_t *p01 = &previousLevelPixels[4 * ((2 * j + 0) * previousMipLevelSize.width + (2 * i + 1))];
					uint8_t *p10 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 0))];
					uint8_t *p11 = &previousLevelPixels[4 * ((2 * j + 1) * previousMipLevelSize.width + (2 * i + 1))];

					p[0] = (p00[0] + p01[0] + p10[0] + p11[0]) / 4;
					p[1] = (p00[1] + p01[1] + p10[1] + p11[1]) / 4;
					p[2] = (p00[2] + p01[2] + p10[2] + p11[2]) / 4;
					p[3] = (p00[3] + p01[3] + p10[3] + p11[3]) / 4;
				}
			}
		}

		destination.mipLevel = level;
		source.bytesPerRow = 4 * mipLevelSize.width;
		source.rowsPerImage = mipLevelSize.height;
		queue.writeTexture(destination, pixels.data(), pixels.size(), source, mipLevelSize);

		previousLevelPixels = std::move(pixels);
		previousMipLevelSize = mipLevelSize;
		mipLevelSize.width /= 2;
		mipLevelSize.height /= 2;
	}

	queue.release();
}

wgpu::Texture LoadTexture(std::filesystem::path const &path, wgpu::Device device, wgpu::TextureView *pTextureView)
{
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char *data = stbi_load(path.string().c_str(), &width, &height, &channels, 4);
	if (data == nullptr)
	{
		std::cerr << "Failed to load texture: " << path << std::endl;
		return nullptr;
	}

	wgpu::TextureDescriptor textureDescriptor{};
	textureDescriptor.dimension = wgpu::TextureDimension::_2D;
	textureDescriptor.format = wgpu::TextureFormat::RGBA8Unorm;
	textureDescriptor.size = {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};
	textureDescriptor.mipLevelCount = std::bit_width(std::max(textureDescriptor.size.width, textureDescriptor.size.height));
	textureDescriptor.sampleCount = 1;
	textureDescriptor.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	textureDescriptor.viewFormatCount = 0;
	textureDescriptor.nextInChain = nullptr;
	wgpu::Texture texture = device.createTexture(textureDescriptor);

	WriteMipMaps(device, texture, textureDescriptor.size, textureDescriptor.mipLevelCount, data);

	stbi_image_free(data);

	if (pTextureView != nullptr)
	{
		wgpu::TextureViewDescriptor textureViewDescriptor{};
		textureViewDescriptor.aspect = wgpu::TextureAspect::All;
		textureViewDescriptor.baseArrayLayer = 0;
		textureViewDescriptor.arrayLayerCount = 1;
		textureViewDescriptor.baseMipLevel = 0;
		textureViewDescriptor.mipLevelCount = textureDescriptor.mipLevelCount;
		textureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
		textureViewDescriptor.format = textureDescriptor.format;
		textureViewDescriptor.nextInChain = nullptr;
		*pTextureView = texture.createView(textureViewDescriptor);
	}

	return texture;
}

wgpu::TextureView GetNextTexture(wgpu::Device &device, wgpu::Surface &surface)
{
	(void)device;

	wgpu::SurfaceTexture surfaceTexture{};
	surface.getCurrentTexture(&surfaceTexture);

	// std::cout << "Surface texture: ";
	switch (surfaceTexture.status)
	{
	case wgpu::SurfaceGetCurrentTextureStatus::DeviceLost:
		std::cerr << "Device lost" << std::endl;
		return nullptr;

	case wgpu::SurfaceGetCurrentTextureStatus::Lost:
		std::cerr << "Lost" << std::endl;
		return nullptr;

	case wgpu::SurfaceGetCurrentTextureStatus::Outdated:
		std::cerr << "Outdated" << std::endl;
		return nullptr;

	case wgpu::SurfaceGetCurrentTextureStatus::OutOfMemory:
		std::cerr << "Out of memory" << std::endl;
		return nullptr;

	case wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal:
		// std::cout << "Success" << std::endl;
		break;

	case wgpu::SurfaceGetCurrentTextureStatus::Timeout:
		std::cerr << "Timeout" << std::endl;
		return nullptr;

	default:
		std::cerr << "Unknown" << std::endl;
		return nullptr;
	}

	wgpu::Texture texture = surfaceTexture.texture;

	wgpu::TextureViewDescriptor textureViewDescriptor{};
	textureViewDescriptor.label = wgpu::StringView("current_surface_texture_view");
	textureViewDescriptor.format = texture.getFormat();
	textureViewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
	textureViewDescriptor.baseMipLevel = 0;
	textureViewDescriptor.mipLevelCount = 1;
	textureViewDescriptor.baseArrayLayer = 0;
	textureViewDescriptor.arrayLayerCount = 1;
	textureViewDescriptor.aspect = wgpu::TextureAspect::All;
	textureViewDescriptor.nextInChain = nullptr;
	wgpu::TextureView textureView = texture.createView(textureViewDescriptor);

	return textureView;
}

bool LoadGeometry(std::filesystem::path const &path, std::vector<float> &pointData, std::vector<uint16_t> &indexData, int dimensions)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		return false;
	}

	pointData.clear();
	indexData.clear();

	enum class Section
	{
		None,
		Points,
		Indices,
	};

	Section currentSection = Section::None;

	float value = 0.0;
	uint16_t index = 0;
	std::string line = "";
	while (!file.eof())
	{
		std::getline(file, line);

		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		if (line == "[points]")
		{
			currentSection = Section::Points;
		}

		else if (line == "[indices]")
		{
			currentSection = Section::Indices;
		}

		else if (line[0] == '#' || line.empty())
		{
			// Do nothing, this is a comment
		}

		else if (currentSection == Section::Points)
		{
			std::istringstream iss(line);
			for (int i = 0; i < dimensions + 3; ++i)
			{
				iss >> value;
				pointData.push_back(value);
			}
		}

		else if (currentSection == Section::Indices)
		{
			std::istringstream iss(line);
			for (int i = 0; i < 3; ++i)
			{
				iss >> index;
				indexData.push_back(index);
			}
		}
	}
	return true;
}

bool LoadGeometryFromOBJ(std::filesystem::path const &path, std::vector<VertexAttributes> &vertexData)
{
	tinyobj::attrib_t attrib{};
	std::vector<tinyobj::shape_t> shapes{};
	std::vector<tinyobj::material_t> materials{};

	std::string warn{};
	std::string err{};

	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str());

	if (!warn.empty())
	{
		std::cerr << "Warning: " << warn << std::endl;
	}

	if (!err.empty())
	{
		std::cerr << "Error: " << err << std::endl;
	}

	if (!result)
	{
		return false;
	}

	for (auto const &shape : shapes)
	{
		size_t offset = vertexData.size();
		vertexData.resize(offset + shape.mesh.indices.size());
		for (size_t i = 0; i < shape.mesh.indices.size(); i++)
		{
			tinyobj::index_t idx = shape.mesh.indices[i];

			vertexData[i].position = {
				attrib.vertices[3 * idx.vertex_index + 0],
				-attrib.vertices[3 * idx.vertex_index + 2],
				attrib.vertices[3 * idx.vertex_index + 1]};
		}
	}

	return true;
}