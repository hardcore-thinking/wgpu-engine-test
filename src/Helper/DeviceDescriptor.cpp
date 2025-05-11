#include <Helper/DeviceDescriptor.hpp>

DeviceDescriptor::DeviceDescriptor(Adapter const& adapter) : DeviceDescriptor(adapter, Limits(adapter)) {}

DeviceDescriptor::DeviceDescriptor(Adapter const& adapter, wgpu::Limits const& limits) {
    int did = id++;
    wgpu::QueueDescriptor defaultQueueDescriptor {};
        defaultQueueDescriptor.label = wgpu::StringView("default_queue_" + std::to_string(did));
        defaultQueueDescriptor.nextInChain = nullptr;

        std::vector<wgpu::FeatureName> requiredFeatures {};
        
        wgpu::DeviceDescriptor deviceDescriptor = wgpu::Default;
        deviceDescriptor.defaultQueue = defaultQueueDescriptor;
        //deviceDescriptor.uncapturedErrorCallbackInfo = DeviceLostCallback;
        deviceDescriptor.deviceLostCallbackInfo = wgpu::DeviceLostCallbackInfo {};
        deviceDescriptor.label = wgpu::StringView("device_" + std::to_string(did));
        deviceDescriptor.nextInChain = nullptr;
        deviceDescriptor.requiredFeatureCount = requiredFeatures.size();
        deviceDescriptor.requiredFeatures = (WGPUFeatureName*)(requiredFeatures.data());
        deviceDescriptor.requiredLimits = &limits;
}

int DeviceDescriptor::id = 0;