#ifndef REGENX_ENGINE_H
#define REGENX_ENGINE_H

#include <functional>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "WindowManager.h"
#include "Renderer.h"
#include "Renderer2D.h"

class Engine {
public:
    struct Config {
        std::string window_title{"RegenX"};
        int window_width{1280};
        int window_height{720};
        Uint32 window_flags{SDL_WINDOW_SHOWN};
        int renderer_index{-1};
        Uint32 renderer_flags{SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
        int target_fps{60};
        SDL_Color clear_color{0, 0, 0, 255};
    };

    using UpdateCallback = std::function<void(float)>;
    using RenderCallback = std::function<void(Renderer2D&, float)>;
    using EventCallback = std::function<void(const SDL_Event&)>;

    explicit Engine(const Config& config = {});
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    void run(UpdateCallback update, RenderCallback render, EventCallback handle_event = {});
    void stop();

    [[nodiscard]] Window& window() { return *window_; }
    [[nodiscard]] Renderer& renderer() { return *renderer_; }
    [[nodiscard]] Renderer2D& renderer2d() { return *renderer2d_; }
    [[nodiscard]] const Config& config() const { return config_; }
    void set_clear_color(SDL_Color color);

private:
    void initialize();
    void shutdown();
    void process_events(const EventCallback& handle_event);

    Config config_;
    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Renderer2D> renderer2d_;
    bool running_{false};
    Uint32 target_frame_time_ms_{16};
};

#endif // REGENX_ENGINE_H
