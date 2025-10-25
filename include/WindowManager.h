//
// Created by liad on 10/24/25.
//

#ifndef GAMEENGINE_WINDOWMANAGER_H
#define GAMEENGINE_WINDOWMANAGER_H
#include <SDL2/SDL.h>

class Window {
    public:
    Window(const char* title, int width, int height, int flags = 0) :
    title(title), width(width), height(height), flags(flags)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    }
    ~Window() {
        if (window) SDL_DestroyWindow(window);
    }

    // encapsulation
    [[nodiscard]] SDL_Window* get_window() const {
        return window;
    }

    [[nodiscard]]const char* get_title() const {
        return title;
    }

    [[nodiscard]]int get_width() const {
        return width;
    }

    [[nodiscard]]int get_height() const {
        return height;
    }

    // window properties or something IDK
    // fullscreen
    void set_fullscreen(const bool fullscreen) {
        if (fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        else {
            // Exit fullscreen
            SDL_SetWindowFullscreen(window, 0);
            flags &= ~SDL_WINDOW_FULLSCREEN;
        }
    }

    [[nodiscard]] bool get_fullscreen() const {
        return (flags & SDL_WINDOW_FULLSCREEN) != 0;
    }

    // resizable
    void set_resizable(const bool resizable) {
        if (resizable) {
            SDL_SetWindowResizable(window, SDL_TRUE);
            flags |= SDL_WINDOW_RESIZABLE;
        }
        else {
            // Exit fullscreen
            SDL_SetWindowResizable(window, SDL_FALSE);
            flags &= ~SDL_WINDOW_RESIZABLE;
        }
    }

    [[nodiscard]] bool get_resizable() const {
        return (flags & SDL_WINDOW_RESIZABLE) != 0;
    }


private:
    SDL_Window* window;
    const char* title;
    int width;
    int height;
    Uint32 flags;
};

#endif //GAMEENGINE_WINDOWMANAGER_H