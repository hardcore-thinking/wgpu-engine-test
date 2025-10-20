#include <Window.hpp>

Window::Window() {
	WindowCreationInfo info;
	SDL_PropertiesID propertiesId = SDL_CreateProperties();
	SDL_SetStringProperty(propertiesId, SDL_PROP_WINDOW_CREATE_TITLE_STRING, info.title.c_str());
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_X_NUMBER, info.x);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_Y_NUMBER, info.y);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, info.w);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, info.h);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, info.flags);
	_handle = SDL_CreateWindowWithProperties(propertiesId);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}

	// std::cout << "Window created successfully: <SDL_Window " << _handle << ">" << std::endl;
}

Window::Window(WindowCreationInfo const& info) {
	SDL_PropertiesID propertiesId = SDL_CreateProperties();
	SDL_SetStringProperty(propertiesId, SDL_PROP_WINDOW_CREATE_TITLE_STRING, info.title.c_str());
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_X_NUMBER, info.x);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_Y_NUMBER, info.y);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, info.w);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, info.h);
	SDL_SetNumberProperty(propertiesId, SDL_PROP_WINDOW_CREATE_FLAGS_NUMBER, info.flags);
	_handle = SDL_CreateWindowWithProperties(propertiesId);
	if (_handle == nullptr) {
		throw std::runtime_error("Failed to create SDL window");
	}

	// std::cout << "Window created successfully: <SDL_Window " << _handle << ">" << std::endl;
}

Window::~Window() {
	if (_handle != nullptr) {
		SDL_DestroyWindow(_handle);
		_handle = nullptr;
	}

	// std::cout << "Window destroyed successfully" << std::endl;
}
