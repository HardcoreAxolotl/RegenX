# =========================================
# File: editor/CMakeLists.txt (EDITOR)
# =========================================
cmake_minimum_required(VERSION 3.26)

add_library(regenx_editor STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/RegenXEditor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/visual/VisualGraph.cpp
)

target_include_directories(regenx_editor
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Editor links engine + tooling-only deps
target_link_libraries(regenx_editor
        PRIVATE
        regenx_engine

        # Python for editor scripting/tools
        Python3::Python

        # ImGui stack for editor UI
        imgui imguizmo imnodeflow
)
