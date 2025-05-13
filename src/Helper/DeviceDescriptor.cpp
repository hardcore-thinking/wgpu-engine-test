#include <Helper/DeviceDescriptor.hpp>

DeviceDescriptor::DeviceDescriptor(Adapter const& adapter) : DeviceDescriptor(adapter, Limits(adapter), {}, {}) {}

DeviceDescriptor::DeviceDescriptor(Adapter const& adapter, Limits const& limits, DeviceLostCallbackInfo const& deviceLostCallbackInfo, UncapturedErrorCallbackInfo const& uncapturedErrorCallbackInfo) {
    int did = id++;
    wgpu::QueueDescriptor defaultQueueDescriptor {};
        defaultQueueDescriptor.label = wgpu::StringView("default_queue_" + std::to_string(did));
        defaultQueueDescriptor.nextInChain = nullptr;

        std::vector<wgpu::FeatureName> requiredFeatures {};
        
        wgpu::DeviceDescriptor deviceDescriptor = wgpu::Default;
        deviceDescriptor.defaultQueue = defaultQueueDescriptor;

        deviceDescriptor.deviceLostCallbackInfo.callback = deviceLostCallbackInfo.callback;
        deviceDescriptor.deviceLostCallbackInfo.nextInChain = deviceLostCallbackInfo.nextInChain;
        deviceDescriptor.deviceLostCallbackInfo.userdata1 = deviceLostCallbackInfo.userdata1;
        deviceDescriptor.deviceLostCallbackInfo.userdata2 = deviceLostCallbackInfo.userdata2;

        deviceDescriptor.label = wgpu::StringView("device_" + std::to_string(did));
        deviceDescriptor.nextInChain = nullptr;
        deviceDescriptor.requiredFeatureCount = requiredFeatures.size();
        deviceDescriptor.requiredFeatures = (WGPUFeatureName*)(requiredFeatures.data());
        deviceDescriptor.requiredLimits = &limits;

        deviceDescriptor.uncapturedErrorCallbackInfo.callback = uncapturedErrorCallbackInfo.callback;
        deviceDescriptor.uncapturedErrorCallbackInfo.nextInChain = uncapturedErrorCallbackInfo.nextInChain;
        deviceDescriptor.uncapturedErrorCallbackInfo.userdata1 = uncapturedErrorCallbackInfo.userdata1;
        deviceDescriptor.uncapturedErrorCallbackInfo.userdata2 = uncapturedErrorCallbackInfo.userdata2;
}

int DeviceDescriptor::id = 0;