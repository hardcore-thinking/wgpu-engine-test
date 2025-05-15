#ifndef COMMANDENCODER_HPP
#define COMMANDENCODER_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>
#include <Helper/CommandEncoderDescriptor.hpp>

class CommandEncoder {
    public:
        CommandEncoder() = delete;
        CommandEncoder(Device& device, CommandEncoderDescriptor const& descriptor);
        ~CommandEncoder();

    public:
        wgpu::CommandEncoder& Handle() { return _handle; }
        wgpu::CommandEncoder const& Handle() const { return _handle; }

        wgpu::CommandEncoder* operator -> ();
        wgpu::CommandEncoder const* operator -> () const;

    private:
        wgpu::CommandEncoder _handle = nullptr;
};

#endif // COMMANDENCODER_HPP