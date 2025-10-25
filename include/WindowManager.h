//
// Created by liad on 10/24/25.
//

#ifndef GAMEENGINE_WINDOWMANAGER_H
#define GAMEENGINE_WINDOWMANAGER_H
#include <SDL2/SDL.h>

class Window {
public:
    // constructor
    Window(const char* title, int width, int height, int flags = 0)
        : title(title), width(width), height(height), flags(flags)
    {
        win = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            flags
        );
    }

    // destructor
    ~Window() {
        if (win && SDL_WasInit(SDL_INIT_VIDEO)) {
            SDL_DestroyWindow(win);
        }
    }

    // deleted copy constructor and assignment operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // getters
    [[nodiscard]] SDL_Window* get_window() const { return win; }
    [[nodiscard]] const char* get_title() const { return title; }
    [[nodiscard]] int get_width() const { return width; }
    [[nodiscard]] int get_height() const { return height; }

    // fullscreen
    void set_fullscreen(bool fullscreen) {
        if (fullscreen) {
            SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
            flags |= SDL_WINDOW_FULLSCREEN;
        } else {
            SDL_SetWindowFullscreen(win, 0);
            flags &= ~SDL_WINDOW_FULLSCREEN;
        }
    }

    [[nodiscard]] bool get_fullscreen() const {
        return (flags & SDL_WINDOW_FULLSCREEN) != 0;
    }

    // resizable
    void set_resizable(bool resizable) {
        if (resizable) {
            SDL_SetWindowResizable(win, SDL_TRUE);
            flags |= SDL_WINDOW_RESIZABLE;
        } else {
            SDL_SetWindowResizable(win, SDL_FALSE);
            flags &= ~SDL_WINDOW_RESIZABLE;
        }
    }

    [[nodiscard]] bool get_resizable() const {
        return (flags & SDL_WINDOW_RESIZABLE) != 0;
    }

    // surface operations
    void update() {
        SDL_UpdateWindowSurface(win);
    }

    SDL_Surface* get_surface() {
        return SDL_GetWindowSurface(win);
    }

private:
    SDL_Window* win = nullptr;
    const char* title;
    int width;
    int height;
    Uint32 flags;
};


#endif //GAMEENGINE_WINDOWMANAGER_H