#ifndef DEVICELOSTCALLBACKINFO_HPP
#define DEVICELOSTCALLBACKINFO_HPP

#include <wgpu-native/webgpu.hpp>

struct DeviceLostCallbackInfo : public wgpu::DeviceLostCallbackInfo {
public:
	DeviceLostCallbackInfo() = default;
	DeviceLostCallbackInfo(wgpu::DeviceLostCallback const& deviceLostCallback, void* userData1, void* userData2);
};

#endif // DEVICELOSTCALLBACKINFO_HPP
