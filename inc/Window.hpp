#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <string>
#include <stdexcept>

#include <SDL3/SDL.h>

struct WindowCreationInfo
{
    std::string title = "SDL window";
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = 640;
    int h = 480;
    Uint32 flags = 0;
};

class Window
{
public:
    Window();
    Window(WindowCreationInfo const &info);
    ~Window();

public:
    SDL_Window *Handle() { return _handle; }

private:
    SDL_Window *_handle = nullptr;
};

#endif // WINDOW_HPP