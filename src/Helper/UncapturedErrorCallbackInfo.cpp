#include <Helper/UncapturedErrorCallbackInfo.hpp>

UncapturedErrorCallbackInfo::UncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallback const& uncapturedErrorCallback, void* userData1, void* userData2) {
    callback = (WGPUUncapturedErrorCallback)(&uncapturedErrorCallback);
    nextInChain = nullptr;
    userdata1 = userData1;
    userdata2 = userData2;
}