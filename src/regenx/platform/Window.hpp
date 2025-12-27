//
// Created by liad on 27/12/2025.
//
#pragma once
#include <SDL2/SDL.h>

class Window {
public:
    void window_create(const char *title, int x, int y, int width, int height, Uint32 flags);

private:
    SDL_Window *window = nullptr;
};