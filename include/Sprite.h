//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_SPRITE_H
#define GAMEENGINE_SPRITE_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <SDL2/SDL_image.h>
#include <EngineTypes.h>
#include <Renderer.h>

class Sprite {
public:
    Sprite(const Renderer* renderer, const char* fl) : filename(fl) {
        if (renderer == nullptr || renderer->get_renderer() == nullptr) {
            throw std::invalid_argument("Renderer must not be null when creating a sprite");
        }

        image = IMG_Load(fl);
        if (!image) {
            throw std::runtime_error(std::string("IMG_Load Error: ") + IMG_GetError());
        }
        width = image->w;
        height = image->h;
        size = image->w * image->h * image->format->BytesPerPixel;

        texture = IMG_LoadTexture(renderer->get_renderer(), filename);
        if (!texture) {
            SDL_FreeSurface(image);
            image = nullptr;
            throw std::runtime_error(std::string("IMG_LoadTexture Error: ") + IMG_GetError());
        }

        if (SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) != 0) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
            SDL_FreeSurface(image);
            image = nullptr;
            throw std::runtime_error(std::string("SDL_QueryTexture Error: ") + SDL_GetError());
        }

        sprite.x = static_cast<int>(position.x - alignment.x);
        sprite.y = static_cast<int>(position.y - alignment.y);
        sprite.w = width;
        sprite.h = height;
    }

    ~Sprite() {
        if (image) {
            SDL_FreeSurface(image);
            image = nullptr;
        }
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
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
    SDL_Surface* image{nullptr};
    SDL_Texture* texture{nullptr};
    SDL_Rect sprite{};
    int width{0};
    int height{0};
    int size{0};
    RGNX2D::Vector2 alignment{};
    RGNX2D::Vector2 position{};
};
#endif //GAMEENGINE_SPRITE_H
