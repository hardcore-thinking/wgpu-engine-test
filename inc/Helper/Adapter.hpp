#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include <iostream>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <SDL2/SDL.h>
#include <sdl2webgpu.h>

#include <Window.hpp>
#include <Helper/Instance.hpp>
#include <Helper/Surface.hpp>

class Adapter {
    public:
        Adapter() = delete;
        Adapter(Instance const& instance, Window const& window, Surface& surface);
        Adapter(Instance const& instance, Window const& window, wgpu::RequestAdapterOptions const& options);
        ~Adapter();

    public:
        wgpu::Adapter& Handle() { return _handle; }
        wgpu::SupportedFeatures const& Features() const { return _features; }
        wgpu::AdapterInfo const& Infos() const { return _infos; }
        wgpu::Limits const& Limits() const { return _limits; }
        void DisplayFeatures() const;
        void DisplayInfos() const;
        void DisplayLimits() const;

    private:
        wgpu::Adapter _handle = nullptr;
        wgpu::SupportedFeatures _features {};
        wgpu::AdapterInfo _infos {};
        wgpu::Limits _limits {};
};

#endif // ADAPTER_HPP