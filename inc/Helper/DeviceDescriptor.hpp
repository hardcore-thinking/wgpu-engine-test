#ifndef DEVICEDESCRIPTOR_HPP
#define DEVICEDESCRIPTOR_HPP

#include <iostream>
#include <string>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Adapter.hpp>
#include <Helper/Limits.hpp>
#include <Helper/DeviceLostCallbackInfo.hpp>
#include <Helper/UncapturedErrorCallbackInfo.hpp>

struct DeviceDescriptor : public wgpu::DeviceDescriptor {
public:
	DeviceDescriptor(Adapter const& adapter);
	DeviceDescriptor(Adapter const& adapter, Limits const& limits, DeviceLostCallbackInfo const& deviceLostCallbackInfo, UncapturedErrorCallbackInfo const& uncapturedErrorCallbackInfo);

public:
	static int id;
};

#endif // DEVICEDESCRIPTOR_HPP
