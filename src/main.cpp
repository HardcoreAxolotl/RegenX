#include <WindowManager.h>
#include <Renderer.h>
#include <Renderer2D.h>
#include <Input.h>

#include <iostream>
#include <SDL2/SDL_image.h>

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    Window window("Test", 1280, 720);
    Renderer renderer(&window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer.get_renderer()) {
        std::cerr << "Renderer creation failed!" << std::endl;
        return -1;
    }

    Renderer2D renderer_2d(renderer);

    Sprite test(&renderer, "../assets/example.png");

    test.set_alignment(test.get_width()/2, test.get_height()/2);

    float x = 0;

    SDL_Event event;
    Input::Input input;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        x++;

        renderer_2d.clear({255, 128, 255, 255});
        renderer_2d.draw_sprite(test, x, 0);
        renderer_2d.present();

        SDL_Delay(1000/60);
    }

    // Cleanup
    SDL_Quit();
    return 0;
}
