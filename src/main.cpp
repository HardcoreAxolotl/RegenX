#include <iostream>
#include <SDL2/SDL.h>
#include "RegenX/RegenXPlatform.hpp"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
    }

    Window window;
    window.window_create("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, -1);
    return 0;
}
