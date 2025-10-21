#include <Helper/CompatibleSurface.hpp>

CompatibleSurface::CompatibleSurface(Instance const& instance, Window& window) {
	_handle = SDL_GetWGPUSurface(instance.Handle(), window.Handle());
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create WGPU surface");
	}

	std::cout << "Surface created successfully: " << Handle() << std::endl;
}

CompatibleSurface::~CompatibleSurface() {
	std::cout << "Compatible surface successfully set for destruction" << std::endl;
}

void CompatibleSurface::Configure(Adapter& adapter, Device& device, Window& window) {
	std::cout << _handle << std::endl;

	std::vector<wgpu::TextureFormat> textureFormats {};

	wgpu::SurfaceConfiguration surfaceConfiguration {};
	surfaceConfiguration.alphaMode = wgpu::CompositeAlphaMode::Auto;
	surfaceConfiguration.device = device.Handle();

	wgpu::SurfaceCapabilities surfaceCapabilities;
	_handle.getCapabilities(adapter.Handle(), &surfaceCapabilities);

	// SDL_DisplayMode displayMode = window.DisplayMode();
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(window.Handle(), &width, &height);

	std::cout << "Window size: " << width << "x" << height << std::endl;

	surfaceConfiguration.format = surfaceCapabilities.formats[0];
	surfaceConfiguration.height = height;
	surfaceConfiguration.nextInChain = nullptr;
	surfaceConfiguration.presentMode = wgpu::PresentMode::Fifo;
	surfaceConfiguration.usage = wgpu::TextureUsage::RenderAttachment;
	surfaceConfiguration.viewFormatCount = textureFormats.size();
	surfaceConfiguration.viewFormats = (WGPUTextureFormat*) (textureFormats.data());
	surfaceConfiguration.width = width;
	_handle.configure(surfaceConfiguration);

	// std::cout << "Surface configured successfully" << std::endl;
}
