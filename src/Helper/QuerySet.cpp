#include <Helper/QuerySet.hpp>

QuerySet::QuerySet() {
	_handle = nullptr;

	//std::cout << "Query set created successfully: " << Handle() << std::endl;
}

QuerySet::QuerySet(Device& device, QuerySetDescriptor const& descriptor) {
	_handle = device->createQuerySet(descriptor);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create query set");
	}

	//std::cout << "Query set created successfully: " << Handle() << std::endl;
}

QuerySet::~QuerySet() {
	if (_handle != nullptr) {
		_handle.destroy();
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Query set destroyed successfully" << std::endl;
}
