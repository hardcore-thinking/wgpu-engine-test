#include <Helper/DeviceLostCallbackInfo.hpp>

DeviceLostCallbackInfo::DeviceLostCallbackInfo(wgpu::DeviceLostCallback const& deviceLostCallback, void* userData1, void* userData2) {
	callback = (WGPUDeviceLostCallback) (&deviceLostCallback);
	this->nextInChain = nullptr;
	this->userdata1 = userData1;
	this->userdata2 = userData2;
}
