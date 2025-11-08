#pragma once

#include <cstdint>

struct SDL_Event;

class Window;
class Renderer;

namespace ax {
namespace NodeEditor {
struct EditorContext;
}
}

class EditorInterface {
public:
    EditorInterface(Window& window, Renderer& renderer);
    ~EditorInterface();

    void process_event(const SDL_Event& event);
    void begin_frame();
    void draw();
    void render();

private:
    Renderer* renderer_;
    ax::NodeEditor::EditorContext* node_editor_context_;
    bool demo_link_created_{false};
};
