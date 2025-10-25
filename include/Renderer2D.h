//
// Created by liad on 10/24/25.
//

#ifndef GAMEENGINE_RENDERER2D_H
#define GAMEENGINE_RENDERER2D_H

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include "Renderer.h"
#include "Sprite.h"

class Renderer2D {
public:
    explicit Renderer2D(Renderer& renderer)
            : renderer(renderer.get_renderer()) {}

    void clear(const SDL_Color& color = {0, 0, 0, 255}) const {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
    }

    void present() const {
        SDL_RenderPresent(renderer);
    }

    void draw_point(int x, int y, const SDL_Color& color = {255, 255, 255, 255}) const {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void draw_line(int x1, int y1, int x2, int y2, const SDL_Color& color = {255, 255, 255, 255}) const {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(renderer, x1,y1, x2,y2);
    }

    void draw_sprite(Sprite sprite, int x, int y) {
        sprite.set_position(x,y);
        SDL_RenderCopy(renderer, sprite.get_texture(), NULL,sprite.get_sprite());
    }

private:
    SDL_Renderer* renderer;
};

#endif //GAMEENGINE_RENDERER2D_H
