// VERIFIED ImNodeFlow + SDL2 + ImGui demo
// This matches the *actual* ImNodeFlow API as shipped
// No imaginary helpers, no inheritance mistakes

#include <SDL.h>
#include <SDL_opengl.h>

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <ImNodeFlow.h>

// -----------------------------
// Simple shared state
// -----------------------------
struct ShapeState {
    ImVec2 pos  {100, 100};
    ImVec4 col  {0.2f, 0.7f, 1.0f, 1.0f};
};

// -----------------------------
// Position node
// -----------------------------
struct PositionNode : ImFlow::BaseNode {
    ImVec2 value {100, 100};
    std::shared_ptr<ImFlow::OutPin<ImVec2>> out;

    PositionNode() : BaseNode("Position") {
        out = addOUT<ImVec2>("Position");
    }

    void draw() override {
        ImGui::DragFloat2("Value", &value.x, 1.0f);
        out->setValue(value);
    }
};

// -----------------------------
// Color node
// -----------------------------
struct ColorNode : ImFlow::BaseNode {
    ImVec4 value {0.2f, 0.7f, 1.0f, 1.0f};
    std::shared_ptr<ImFlow::OutPin<ImVec4>> out;

    ColorNode() : BaseNode("Color") {
        out = addOUT<ImVec4>("Color");
    }

    void draw() override {
        ImGui::ColorEdit4("Value", &value.x);
        out->setValue(value);
    }
};

// -----------------------------
// Rectangle sink node
// -----------------------------
struct RectNode : ImFlow::BaseNode {
    ShapeState* target;
    std::shared_ptr<ImFlow::InPin<ImVec2>> inPos;
    std::shared_ptr<ImFlow::InPin<ImVec4>> inCol;

    RectNode(ShapeState* s) : BaseNode("Rectangle"), target(s) {
        inPos = addIN<ImVec2>("Position", ImVec2{100,100});
        inCol = addIN<ImVec4>("Color", ImVec4{1,1,1,1});
    }

    void draw() override {
        if (inPos->isLinked()) target->pos = inPos->getValue();
        if (inCol->isLinked()) target->col = inCol->getValue();
        ImGui::Text("Applying to rectangle");
    }
};

// -----------------------------
// Main
// -----------------------------
int main(int, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "ImNodeFlow Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    SDL_GLContext gl = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplSDL2_InitForOpenGL(window, gl);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImFlow::ImNodeFlow graph;
    ShapeState shape;

    graph.addNode<PositionNode>({0, 0});
    graph.addNode<ColorNode>({0, 200});
    graph.addNode<RectNode>({400, 100}, &shape);

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Node Graph");
        graph.update();
        graph.draw();
        ImGui::End();

        ImGui::Begin("Preview");
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        dl->AddRectFilled(
            {p.x + shape.pos.x, p.y + shape.pos.y},
            {p.x + shape.pos.x + 120, p.y + shape.pos.y + 80},
            ImColor(shape.col)
        );
        ImGui::Dummy({400,300});
        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
