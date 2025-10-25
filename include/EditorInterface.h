//
// Created by liad on 10/25/25.
//

#ifndef GAMEENGINE_EDITORINTERFACE_H
#define GAMEENGINE_EDITORINTERFACE_H
#include <string>
#include "WindowManager.h"
#include "Renderer.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

class EditorInterface {
    public:
    EditorInterface(const Window* window, const Renderer* renderer) {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO(); (void)io;

        ImGui_ImplSDL2_InitForSDLRenderer(window->get_window(), renderer->get_renderer());
        ImGui_ImplSDLRenderer2_Init(renderer->get_renderer());
    }
    ~EditorInterface() = default;

    static void ImGuiEvent(const SDL_Event &event) {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }

    static void TestFrame() {
        // Start ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // ImGui UI
        ImGui::Begin("Test");
        ImGui::Text("Hello ImGui + SDLRenderer2!");
        ImGui::End();

        ImGui::Begin("Code");
        ImGui::End();

        ImGui::Render();
    };

    void Render() {
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer.get_renderer());
    }

    static void Destroy() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    private:
};
#endif //GAMEENGINE_EDITORINTERFACE_H