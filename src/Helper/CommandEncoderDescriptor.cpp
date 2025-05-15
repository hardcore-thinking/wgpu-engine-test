#include <Helper/CommandEncoderDescriptor.hpp>

CommandEncoderDescriptor::CommandEncoderDescriptor(std::string const& overrideLabel) {
    newLabel = ((overrideLabel != "") ? overrideLabel : "command_encoder") + "_" + std::to_string(counter++);
    label = wgpu::StringView(newLabel.c_str());
    nextInChain = nullptr;
}

int CommandEncoderDescriptor::counter = 0;