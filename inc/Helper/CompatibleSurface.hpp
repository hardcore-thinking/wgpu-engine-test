#ifndef COMPATIBLESURFACE_HPP
#define COMPATIBLESURFACE_HPP

#include <iostream>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <SDL2/SDL.h>
#include <sdl2webgpu.h>

#include <Window.hpp>
#include <Helper/Instance.hpp>
#include <Helper/Adapter.hpp>
#include <Helper/Device.hpp>
#include <Helper/Surface.hpp>

class CompatibleSurface : public Surface {
    public:
        CompatibleSurface() = default;
        CompatibleSurface(Instance const& instance, Window& window);
        ~CompatibleSurface() = default;

    public:
        void Configure(Adapter& adapter, Device& device, Window& window);
};

#endif // COMPATIBLESURFACE_HPP