#include <Window.hpp>

Window::Window() {
    WindowCreationInfo info;
    _handle = SDL_CreateWindow(info.title.c_str(), info.x, info.y, info.w, info.h, info.flags);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create SDL window");
    }

    //std::cout << "Window created successfully: <SDL_Window " << _handle << ">" << std::endl;
}

Window::Window(WindowCreationInfo const& info) {
    _handle = SDL_CreateWindow(info.title.c_str(), info.x, info.y, info.w, info.h, info.flags);
    if (_handle == nullptr) {
        throw std::runtime_error("Failed to create SDL window");
    }

    //std::cout << "Window created successfully: <SDL_Window " << _handle << ">" << std::endl;
}

SDL_DisplayMode Window::DisplayMode() {
    SDL_DisplayMode mode {};
    SDL_GetWindowDisplayMode(_handle, &mode);
    return mode;
}

Window::~Window() {
    if (_handle != nullptr) {
        SDL_DestroyWindow(_handle);
        _handle = nullptr;
    }

    //std::cout << "Window destroyed successfully" << std::endl;
}