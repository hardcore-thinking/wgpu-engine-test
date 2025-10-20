#include <Helper/TextureView.hpp>

TextureView::TextureView(Texture& texture, TextureViewDescriptor const& descriptor) {
	_handle = texture->createView(descriptor);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create WGPU texture view");
	}

	std::cout << "Texture view created successfully: " << Handle() << std::endl;
}

TextureView::TextureView(TextureView&& other) {
	_handle = other._handle;
	other._handle = nullptr;
}

TextureView::TextureView(wgpu::TextureView&& other) {
	_handle = other;
	other = nullptr;
}

TextureView::~TextureView() {
	if (_handle != nullptr) {
		_handle.release();
		_handle = nullptr;
	}

	//std::cout << "Texture view released successfully" << std::endl;
}

TextureView& TextureView::operator = (TextureView&& other) {
	if (this != &other) {
		if (_handle != nullptr) {
			_handle.release();
			_handle = nullptr;
		}
		_handle = other._handle;
		other._handle = nullptr;
	}
	return *this;
}

TextureView& TextureView::operator = (wgpu::TextureView&& other) {
	if (this->Handle() != other) {
		if (_handle != nullptr) {
			_handle.release();
			_handle = nullptr;
		}
		_handle = other;
		other = nullptr;
	}
	return *this;
}

wgpu::TextureView* TextureView::operator->() {
	return &_handle;
}

wgpu::TextureView const* TextureView::operator->() const {
	return &_handle;
}
