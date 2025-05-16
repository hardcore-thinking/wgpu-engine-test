#include <Helper/Queue.hpp>

Queue::Queue(Device const& device) {
    _handle = device->getQueue();
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to get queue");
    }

    //std::cout << "Queue created successfully: " << Handle() << std::endl;
}

Queue::~Queue() {
    if (_handle) {
        _handle.release();
        _handle = nullptr;
    }

    //std::cout << "Queue destroyed successfully" << std::endl;
}

wgpu::Queue* Queue::operator-> () {
    return &_handle;
}

wgpu::Queue const* Queue::operator-> () const {
    return &_handle;
}
