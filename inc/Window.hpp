#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

struct WindowCreationInfo {
	std::string title = "SDL window";
	int x = SDL_WINDOWPOS_CENTERED;
	int y = SDL_WINDOWPOS_CENTERED;
	int w = 1920;
	int h = 1080;
	Uint32 flags = SDL_WINDOW_SHOWN;
};

class Window {
    public:
        Window();
        Window(WindowCreationInfo const& info);
        ~Window();

    public:
        SDL_Window* Handle() { return _handle; }
        SDL_DisplayMode DisplayMode();

    private:
        SDL_Window* _handle = nullptr;
};

#endif // WINDOW_HPP