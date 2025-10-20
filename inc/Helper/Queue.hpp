#ifndef QUEUE_CPP
#define QUEUE_CPP

#include <wgpu-native/webgpu.hpp>

#include <Helper/Device.hpp>

class Queue {
public:
	Queue() = delete;
	Queue(Device const& device);
	~Queue();

public:
	wgpu::Queue& Handle() {
		return _handle;
	}
	wgpu::Queue const& Handle() const {
		return _handle;
	}

	wgpu::Queue* operator-> ();
	wgpu::Queue const* operator-> () const;

private:
	wgpu::Queue _handle = nullptr;
};

#endif // QUEUE_CPP
