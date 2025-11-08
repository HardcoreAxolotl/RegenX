#include "Engine.h"

#include <stdexcept>

namespace {
constexpr float MS_TO_SECONDS = 1.0f / 1000.0f;
}

Engine::Engine(const Config& config) : config_(config) {
    initialize();
}

Engine::~Engine() {
    shutdown();
}

void Engine::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        SDL_Quit();
        throw std::runtime_error(std::string("IMG_Init failed: ") + IMG_GetError());
    }

    window_ = std::make_unique<Window>(config_.window_title.c_str(), config_.window_width, config_.window_height,
                                       static_cast<int>(config_.window_flags));
    if (!window_->get_window()) {
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Failed to create SDL_Window");
    }

    renderer_ = std::make_unique<Renderer>(window_.get(), config_.renderer_index, static_cast<int>(config_.renderer_flags));
    if (!renderer_->get_renderer()) {
        renderer_.reset();
        window_.reset();
        IMG_Quit();
        SDL_Quit();
        throw std::runtime_error("Failed to create SDL_Renderer");
    }

    renderer2d_ = std::make_unique<Renderer2D>(*renderer_);

    if (config_.target_fps > 0) {
        target_frame_time_ms_ = static_cast<Uint32>(1000.0f / static_cast<float>(config_.target_fps));
    } else {
        target_frame_time_ms_ = 0;
    }
}

void Engine::shutdown() {
    renderer2d_.reset();
    renderer_.reset();
    window_.reset();

    IMG_Quit();
    SDL_Quit();
}

void Engine::run(UpdateCallback update, RenderCallback render, EventCallback handle_event) {
    running_ = true;
    Uint32 previous_ticks = SDL_GetTicks();

    while (running_) {
        Uint32 frame_start = SDL_GetTicks();
        process_events(handle_event);

        Uint32 current_ticks = SDL_GetTicks();
        float delta_time = static_cast<float>(current_ticks - previous_ticks) * MS_TO_SECONDS;
        previous_ticks = current_ticks;

        if (update) {
            update(delta_time);
        }

        if (render) {
            renderer2d_->clear(config_.clear_color);
            render(*renderer2d_, delta_time);
            renderer2d_->present();
        }

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (target_frame_time_ms_ > 0 && frame_time < target_frame_time_ms_) {
            SDL_Delay(target_frame_time_ms_ - frame_time);
        }
    }

    running_ = false;
}

void Engine::process_events(const EventCallback& handle_event) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (handle_event) {
            handle_event(event);
        }

        if (event.type == SDL_QUIT) {
            running_ = false;
        }
    }
}

void Engine::stop() {
    running_ = false;
}

void Engine::set_clear_color(SDL_Color color) {
    config_.clear_color = color;
}
