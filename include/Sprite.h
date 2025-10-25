//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_SPRITE_H
#define GAMEENGINE_SPRITE_H
#include <iostream>
#include <SDL2/SDL_image.h>
#include <EngineTypes.h>

class Sprite {
public:
    Sprite(const Renderer* renderer, const char* fl) : filename(fl) {
        image = IMG_Load(fl);
        if (!image) {
            std::cerr << "IMG_Load Error: " << IMG_GetError() << "\n";
        }
        this->width = image->w;
        this->height = image->h;
        this->size = image->w * image->h * image->format->BytesPerPixel;
        texture = IMG_LoadTexture(renderer->get_renderer(), filename);
        SDL_QueryTexture(texture, NULL, NULL, &image->w, &image->h);
        sprite.x = static_cast<int>(position.x - alignment.x);
        sprite.y = static_cast<int>(position.y - alignment.y);
        sprite.w = width;
        sprite.h = height;
    }

    ~Sprite() {
        if(image) SDL_FreeSurface(image);
        if(texture) SDL_DestroyTexture(texture);
    }

    [[nodiscard]] int get_width() const {
        return width;
    }
    [[nodiscard]] int get_height() const {
        return height;
    }
    [[nodiscard]] int get_size() const {
        return size;
    }
    [[nodiscard]] SDL_Surface* get_image() const {
        return image;
    }
    [[nodiscard]] SDL_Texture* get_texture() const {
        return texture;
    }
    [[nodiscard]] const SDL_Rect* get_sprite() const {
        return &sprite;
    }
    void set_alignment(const float _x, const float _y) {
        alignment.x = _x;
        alignment.y = _y;
        sprite.x = static_cast<int>(position.x - alignment.x);
        sprite.y = static_cast<int>(position.y - alignment.y);
    }
    void set_position(const float _x, const float _y) {
        position.x = _x;
        position.y = _y;
        sprite.x = static_cast<int>(position.x - alignment.x);
        sprite.y = static_cast<int>(position.y - alignment.y);
    }

private:
    const char* filename{};
    SDL_Surface* image;
    SDL_Texture* texture;
    SDL_Rect sprite{};
    int width;
    int height;
    int size;
    RGNX2D::Vector2 alignment;
    RGNX2D::Vector2 position;
};
#endif //GAMEENGINE_SPRITE_H