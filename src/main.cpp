#include <SDL2/SDL.h>
#include <gainput/gainput.h>

// Define your user buttons somewhere global
enum Button
{
    ButtonConfirm
};

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    // Setup window
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        "Input",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    // Setting up Gainput
    gainput::InputManager manager;
    const gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
    manager.SetDisplaySize(SCREEN_WIDTH, SCREEN_HEIGHT);
    gainput::InputMap map(manager);
    map.MapBool(ButtonConfirm, mouseId, gainput::MouseButtonLeft);

    SDL_Event event;
    bool game_running = true;
    while (game_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                game_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer ,0,0,0,255);
        SDL_RenderClear(renderer);

        // Call every frame
        manager.Update();
        // May have to call platform-specific event-handling functions here.
        // Check button state
        if (map.GetBoolWasDown(ButtonConfirm))
        {
            SDL_Rect rect = {100, 100, 100, 100};
            SDL_RenderDrawRect(renderer, &rect);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();
}