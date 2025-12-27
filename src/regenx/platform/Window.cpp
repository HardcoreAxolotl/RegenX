//
// Created by liad on 27/12/2025.
//

#include "Window.hpp"

void Window::window_create(
    const char* title,
    int x,
    int y,
    int width,
    int height,
    Uint32 flags
) {
    window = SDL_CreateWindow(title, x, y, width, height, flags);

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
    }
}
