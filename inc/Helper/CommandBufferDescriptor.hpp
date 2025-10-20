#ifndef COMMANDBUFFERDESCRIPTOR_HPP
#define COMMANDBUFFERDESCRIPTOR_HPP

#include <wgpu-native/webgpu.hpp>

struct CommandBufferDescriptor : public wgpu::CommandBufferDescriptor {
	CommandBufferDescriptor();
};

#endif // COMMANDBUFFERDESCRIPTOR_HPP
