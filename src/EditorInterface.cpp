#include "EditorInterface.h"

#include "Renderer.h"
#include "WindowManager.h"

#include <SDL2/SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_node_editor.h>

EditorInterface::EditorInterface(Window& window, Renderer& renderer)
    : renderer_(&renderer), node_editor_context_(nullptr) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window.get_window(), renderer.get_renderer());
    ImGui_ImplSDLRenderer2_Init(renderer.get_renderer());

    ax::NodeEditor::Config config;
    config.SettingsFile = "regenx_node_editor.json";
    node_editor_context_ = ax::NodeEditor::CreateEditor(&config);
}

EditorInterface::~EditorInterface() {
    if (node_editor_context_) {
        ax::NodeEditor::DestroyEditor(node_editor_context_);
        node_editor_context_ = nullptr;
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void EditorInterface::process_event(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void EditorInterface::begin_frame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void EditorInterface::draw() {
    if (!node_editor_context_) {
        return;
    }

    ax::NodeEditor::SetCurrentEditor(node_editor_context_);

    ImGui::Begin("RegenX Tooling");
    ImGui::TextUnformatted("SDL2 + ImGui node editor integration ready.");
    ImGui::TextUnformatted("Use this space to build gameplay graphs.");
    ImGui::End();

    ax::NodeEditor::Begin("Scene Graph");

    ax::NodeEditor::BeginNode(1);
    ImGui::TextUnformatted("Sprite Loader");
    ax::NodeEditor::BeginPin(2, ax::NodeEditor::PinKind::Output);
    ImGui::TextUnformatted("Texture");
    ax::NodeEditor::EndPin();
    ax::NodeEditor::EndNode();

    ax::NodeEditor::BeginNode(3);
    ImGui::TextUnformatted("Renderer");
    ax::NodeEditor::BeginPin(4, ax::NodeEditor::PinKind::Input);
    ImGui::TextUnformatted("Texture");
    ax::NodeEditor::EndPin();
    ax::NodeEditor::EndNode();

    if (!demo_link_created_) {
        ax::NodeEditor::Link(5, 2, 4);
        demo_link_created_ = true;
    }

    ax::NodeEditor::MiniMap(0.2f, ax::NodeEditor::MiniMapLocation::BottomRight);

    ax::NodeEditor::End();
    ax::NodeEditor::SetCurrentEditor(nullptr);
}

void EditorInterface::render() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_->get_renderer());
}
