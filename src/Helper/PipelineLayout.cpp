#include <Helper/PipelineLayout.hpp>

PipelineLayout::PipelineLayout(Device& device, PipelineLayoutDescriptor const& pipelineLayoutDescriptor) {
    _handle = device.Handle().createPipelineLayout(pipelineLayoutDescriptor);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create pipeline layout");
    }

    std::cout << "PipelineLayout created: " << Handle() << std::endl;
}

PipelineLayout::~PipelineLayout() {
    if (_handle != nullptr) {
        _handle.release();
        _handle = nullptr;
    }

    std::cout << "PipelineLayout destroyed." << std::endl;
}