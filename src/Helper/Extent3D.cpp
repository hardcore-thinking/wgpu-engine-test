#include <Helper/Extent3D.hpp>

Extent3D::Extent3D(uint32_t width, uint32_t height, uint32_t depthOrArrayLayers) {
    this->width = width;
    this->height = height;
    this->depthOrArrayLayers = depthOrArrayLayers;
}