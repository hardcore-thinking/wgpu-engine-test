#include <Helper/Surface.hpp>

Surface::Surface(Instance const& instance) {
    wgpu::SurfaceDescriptor descriptor {};
    descriptor.nextInChain = nullptr;
    descriptor.label = wgpu::StringView("surface" + std::to_string(_id++));

    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU surface");
    }
    
    std::cout << "Surface created successfully: " << Handle() << std::endl;
}

Surface::Surface(Instance const& instance, wgpu::SurfaceDescriptor const& descriptor) {
    _handle = instance.Handle().createSurface(descriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create WGPU surface");
    }
    
    std::cout << "Surface created successfully: " << Handle() << std::endl;
}

Surface::~Surface() {
    if (_handle != nullptr) {
        _handle.unconfigure();
        _handle.release();
        _handle = nullptr;
    }
}

int Surface::_id = 0;