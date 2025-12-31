cmake_minimum_required(VERSION 3.26)

# ============================================================
# RegenX Third-Party Libraries
# ============================================================

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Freetype REQUIRED)
find_package(Lua REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED) # only needed if SLikeNet crypto is ON (you disable it)

# ------------------------------------------------------------
# Unified deps target
# ------------------------------------------------------------
add_library(regenx_deps INTERFACE)
target_link_libraries(regenx_deps INTERFACE Threads::Threads)

# SDL2
if (TARGET SDL2::SDL2)
    target_link_libraries(regenx_deps INTERFACE SDL2::SDL2)
else()
    target_include_directories(regenx_deps INTERFACE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(regenx_deps INTERFACE ${SDL2_LIBRARIES})
endif()

# OpenAL
if (TARGET OpenAL::OpenAL)
    target_link_libraries(regenx_deps INTERFACE OpenAL::OpenAL)
else()
    target_include_directories(regenx_deps INTERFACE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(regenx_deps INTERFACE ${OPENAL_LIBRARY})
endif()

# FreeType
if (TARGET Freetype::Freetype)
    target_link_libraries(regenx_deps INTERFACE Freetype::Freetype)
else()
    target_include_directories(regenx_deps INTERFACE ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(regenx_deps INTERFACE ${FREETYPE_LIBRARIES})
endif()

# Lua
target_include_directories(regenx_deps INTERFACE ${LUA_INCLUDE_DIR})
target_link_libraries(regenx_deps INTERFACE ${LUA_LIBRARIES})

# -----------------------------
# stb (header-only)
# -----------------------------
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb)

# -----------------------------
# EnTT (header-only)
# -----------------------------
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE ${CMAKE_CURRENT_LIST_DIR}/entt/single_include)

# -----------------------------
# sol2 (header-only)
# -----------------------------
add_library(sol2 INTERFACE)
target_include_directories(sol2 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/sol2/include)

# -----------------------------
# ImGui (static + SDL2 + OpenGL backend)
# -----------------------------
add_library(imgui STATIC
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp
        imgui/backends/imgui_impl_sdl2.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/imgui
        ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
)

# Make math operators available for ImVec2 ops used by ImGuizmo/ImNodeFlow
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

# SDL2 linking for backend + OpenGL for renderer backend
if (TARGET SDL2::SDL2)
    target_link_libraries(imgui PUBLIC SDL2::SDL2)
else()
    target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})
endif()
target_link_libraries(imgui PUBLIC OpenGL::GL)

# -----------------------------
# ImGuizmo
# -----------------------------
add_library(imguizmo STATIC
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImGuizmo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImCurveEdit.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImSequencer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/GraphEditor.cpp
)
target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo)
target_link_libraries(imguizmo PUBLIC imgui)

# -----------------------------
# ImNodeFlow
# -----------------------------
file(GLOB IMNODEFLOW_SRC ${CMAKE_CURRENT_LIST_DIR}/ImNodeFlow/src/*.cpp)
add_library(imnodeflow STATIC ${IMNODEFLOW_SRC})
target_include_directories(imnodeflow PUBLIC ${CMAKE_CURRENT_LIST_DIR}/ImNodeFlow/include)
target_link_libraries(imnodeflow PUBLIC imgui)

# -----------------------------
# SLikeNet (crypto OFF for OpenSSL3)
# -----------------------------
set(SLIKENET_ENABLE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_DLL OFF CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_STATIC ON CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_CRYPTO OFF CACHE BOOL "" FORCE)

add_subdirectory(SLikeNet)

if (TARGET SLikeNetLibStatic)
    # Remove crypto sources if they still sneak in
    get_target_property(_slikenet_srcs SLikeNetLibStatic SOURCES)
    if (_slikenet_srcs)
        list(FILTER _slikenet_srcs EXCLUDE REGEX ".*/crypto/.*")
        set_property(TARGET SLikeNetLibStatic PROPERTY SOURCES "${_slikenet_srcs}")
    endif()
    target_compile_definitions(SLikeNetLibStatic PUBLIC SLIKENET_NO_CRYPTO=1)
    add_library(SLikeNet::SLikeNet ALIAS SLikeNetLibStatic)
endif()

# -----------------------------
# BGFX stack (imported prebuilt)
# -----------------------------
set(BGFX_BUILD_DIR ${CMAKE_CURRENT_LIST_DIR}/bgfx/.build/linux64_gcc/bin)

function(import_bgfx name file)
    add_library(${name} STATIC IMPORTED GLOBAL)
    set_target_properties(${name} PROPERTIES IMPORTED_LOCATION "${BGFX_BUILD_DIR}/${file}")
endfunction()

import_bgfx(bx   libbxRelease.a)
import_bgfx(bimg libbimgRelease.a)
import_bgfx(bgfx libbgfxRelease.a)

target_include_directories(bx   INTERFACE ${CMAKE_CURRENT_LIST_DIR}/bx/include)
target_include_directories(bimg INTERFACE ${CMAKE_CURRENT_LIST_DIR}/bimg/include)
target_include_directories(bgfx INTERFACE ${CMAKE_CURRENT_LIST_DIR}/bgfx/include)

target_link_libraries(bgfx INTERFACE dl pthread)
