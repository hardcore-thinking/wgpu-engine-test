#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

class Instance {
    public:
        Instance();
        Instance(wgpu::InstanceDescriptor const& descriptor);
        ~Instance();

    public:
        wgpu::Instance Handle() const { return _handle; }

    private:
        wgpu::Instance _handle = nullptr;
};

#endif // INSTANCE_HPP