cmake_minimum_required(VERSION 3.16)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(NODE_EDITOR_DIR ${CMAKE_SOURCE_DIR}/imgui-node-editor)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

set(IMGUI_NODE_EDITOR_SOURCES
    ${NODE_EDITOR_DIR}/crude_json.cpp
    ${NODE_EDITOR_DIR}/imgui_canvas.cpp
    ${NODE_EDITOR_DIR}/imgui_node_editor.cpp
    ${NODE_EDITOR_DIR}/imgui_node_editor_api.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(GameEngine
    src/main.cpp
    src/Engine.cpp
    src/EditorInterface.cpp
    ${IMGUI_SOURCES}
    ${IMGUI_NODE_EDITOR_SOURCES}
)

target_include_directories(GameEngine PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${NODE_EDITOR_DIR}
    ${NODE_EDITOR_DIR}/external
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(GameEngine
    ${SDL2_LIBRARIES}
    SDL2_image
    GL
)
