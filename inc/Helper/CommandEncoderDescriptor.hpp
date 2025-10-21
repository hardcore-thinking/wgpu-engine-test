#ifndef COMMANDENCODERDESCRIPTOR_HPP
#define COMMANDENCODERDESCRIPTOR_HPP

#include <iostream>
#include <string>

#include <wgpu-native/webgpu.hpp>

struct CommandEncoderDescriptor : public wgpu::CommandEncoderDescriptor {
public:
	CommandEncoderDescriptor(std::string const& overrideLabel = "");

public:
	static int counter;
	std::string newLabel = "";
};

#endif // COMMANDENCODERDESCRIPTOR_HPP
