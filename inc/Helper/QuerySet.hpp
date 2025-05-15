#ifndef QUERYSET_HPP
#define QUERYSET_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/QuerySetDescriptor.hpp>

class QuerySet {
    public:
        QuerySet();
        QuerySet(Device& device, QuerySetDescriptor const& descriptor);
        ~QuerySet();

        wgpu::QuerySet& Handle() { return _handle; }
        wgpu::QuerySet const& Handle() const { return _handle; }

        wgpu::QuerySet* operator -> ();

    private:
        wgpu::QuerySet _handle = nullptr;
};

#endif // QUERYSET_HPP
