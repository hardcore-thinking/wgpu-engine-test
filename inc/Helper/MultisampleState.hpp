#ifndef MULTISAMPLESTATE_HPP
#define MULTISAMPLESTATE_HPP

#include <wgpu-native/webgpu.hpp>

struct MultisampleState : public wgpu::MultisampleState {
    public:
        MultisampleState();
};

#endif // MULTISAMPLESTATE_HPP