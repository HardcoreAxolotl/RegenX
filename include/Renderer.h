//
// Created by liad on 10/24/25.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H
#include <SDL2/SDL.h>
#include "WindowManager.h"

class Renderer {
public:
    Renderer(Window *window, int index, int flags) {
        renderer = SDL_CreateRenderer(window->get_window(), index, flags);
    }
    ~Renderer() {
        SDL_DestroyRenderer(renderer);
    }

    [[nodiscard]] SDL_Renderer *get_renderer() const {
        return renderer;
    }


private:
    SDL_Renderer *renderer;
};

#endif //GAMEENGINE_RENDERER_H