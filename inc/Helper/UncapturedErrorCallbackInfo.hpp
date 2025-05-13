#ifndef UNCAPTUREDERRORCALLBACKINFO_HPP
#define UNCAPTUREDERRORCALLBACKINFO_HPP

#include <wgpu-native/webgpu.hpp>

struct UncapturedErrorCallbackInfo : public wgpu::UncapturedErrorCallbackInfo {
    public:
        UncapturedErrorCallbackInfo() = default;
        UncapturedErrorCallbackInfo(wgpu::UncapturedErrorCallback const& uncapturedErrorCallbackInfo, void* userData1, void* userData2);
};

#endif // UNCAPTUREDERRORCALLBACKINFO_HPP