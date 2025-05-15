#ifndef QUERYSETDESCRIPTOR_HPP
#define QUERYSETDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

struct QuerySetDescriptor : public wgpu::QuerySetDescriptor {
    public:
        QuerySetDescriptor() = delete;
        QuerySetDescriptor(uint32_t count, wgpu::QueryType type);
};

#endif // QUERYSETDESCRIPTOR_HPP