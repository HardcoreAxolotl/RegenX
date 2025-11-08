cmake_minimum_required(VERSION 3.16)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)

# ImGui + ImNodes
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(NODE_EDITOR_DIR ${CMAKE_SOURCE_DIR}/imgui-node-editor)

file(GLOB IMGUI_SRC
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
        # ${NODE_EDITOR_DIR}/*.cpp
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Include directories
include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${CMAKE_SOURCE_DIR}/include
        # ${NODE_EDITOR_DIR}
)

# Executable
add_executable(GameEngine
        src/main.cpp
        src/Engine.cpp
        ${IMGUI_SRC}
)

# Link libraries
target_link_libraries(GameEngine
        ${SDL2_LIBRARIES}
        SDL2_image
        GL
)
