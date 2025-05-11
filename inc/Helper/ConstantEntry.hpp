#ifndef CONSTANTENTRY_HPP
#define CONSTANTENTRY_HPP

#include <wgpu-native/webgpu.hpp>

struct ConstantEntry : public wgpu::ConstantEntry {
    public:
        ConstantEntry() = delete;
        ConstantEntry(wgpu::StringView const& constantKey, double constantValue);
};

#endif // CONSTANTENTRY_HPP