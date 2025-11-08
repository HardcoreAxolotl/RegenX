#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <Engine.h>
#include <Sprite.h>

int main() {
    try {
        Engine::Config config;
        config.window_title = "RegenX Demo";
        config.window_width = 1280;
        config.window_height = 720;
        config.clear_color = {30, 30, 36, 255};
        config.target_fps = 60;

        Engine engine(config);

        Sprite sprite(&engine.renderer(), "../assets/example.png");
        sprite.set_alignment(static_cast<float>(sprite.get_width()) / 2.0f,
                             static_cast<float>(sprite.get_height()) / 2.0f);

        float sprite_position_x = static_cast<float>(config.window_width) / 2.0f;
        const float sprite_speed = 120.0f;
        const float sprite_y = static_cast<float>(config.window_height) / 2.0f;

        engine.run(
            [&](float delta_time) {
                sprite_position_x += sprite_speed * delta_time;
                if (sprite_position_x > static_cast<float>(config.window_width)) {
                    sprite_position_x = 0.0f;
                }
            },
            [&](Renderer2D& renderer2d, float) {
                renderer2d.draw_sprite(sprite, static_cast<int>(sprite_position_x), static_cast<int>(sprite_y));
            },
            [&](const SDL_Event& event) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    engine.stop();
                }
            }
        );
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}
