cmake_minimum_required(VERSION 3.26)

# ============================================================
# RegenX Third-Party Libraries
# ============================================================

# ------------------------------------------------------------
# Threads (always required early)
# ------------------------------------------------------------
find_package(Threads REQUIRED)

# ------------------------------------------------------------
# Helper: unified dependency target
# ------------------------------------------------------------
add_library(regenx_deps INTERFACE)
target_link_libraries(regenx_deps INTERFACE Threads::Threads)

# ============================================================
# SDL2
# ============================================================
find_package(SDL2 REQUIRED)

if (TARGET SDL2::SDL2)
    target_link_libraries(regenx_deps INTERFACE SDL2::SDL2)
else()
    target_include_directories(regenx_deps INTERFACE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(regenx_deps INTERFACE ${SDL2_LIBRARIES})
endif()

# ============================================================
# OpenAL
# ============================================================
find_package(OpenAL REQUIRED)

if (TARGET OpenAL::OpenAL)
    target_link_libraries(regenx_deps INTERFACE OpenAL::OpenAL)
else()
    target_include_directories(regenx_deps INTERFACE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(regenx_deps INTERFACE ${OPENAL_LIBRARY})
endif()

# ============================================================
# FreeType
# ============================================================
find_package(Freetype REQUIRED)

if (TARGET Freetype::Freetype)
    target_link_libraries(regenx_deps INTERFACE Freetype::Freetype)
else()
    target_include_directories(regenx_deps INTERFACE ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(regenx_deps INTERFACE ${FREETYPE_LIBRARIES})
endif()

# ============================================================
# Lua (DO NOT assume Lua::Lua exists)
# ============================================================
find_package(Lua REQUIRED)

target_include_directories(regenx_deps INTERFACE ${LUA_INCLUDE_DIR})
target_link_libraries(regenx_deps INTERFACE ${LUA_LIBRARIES})

# ============================================================
# stb (header-only)
# ============================================================
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/stb
)

# ============================================================
# EnTT (header-only)
# ============================================================
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/entt/single_include
)

# ============================================================
# sol2 (header-only)
# ============================================================
add_library(sol2 INTERFACE)
target_include_directories(sol2 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/sol2/include
)

# ============================================================
# ImGui (static)
# ============================================================
add_library(imgui STATIC
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/imgui
)

# IMPORTANT: enable math operators globally for ImGui extensions
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

# ============================================================
# ImGuizmo
# ============================================================
add_library(imguizmo STATIC
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImGuizmo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImCurveEdit.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/ImSequencer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo/GraphEditor.cpp
)

target_include_directories(imguizmo PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/ImGuizmo
)

target_link_libraries(imguizmo PUBLIC imgui)

# ============================================================
# ImNodeFlow (FIXED)
# ============================================================
file(GLOB IMNODEFLOW_SRC
        ${CMAKE_CURRENT_LIST_DIR}/ImNodeFlow/src/*.cpp
)

add_library(imnodeflow STATIC ${IMNODEFLOW_SRC})

target_include_directories(imnodeflow PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/ImNodeFlow/include
)

target_link_libraries(imnodeflow PUBLIC imgui)

# REQUIRED or ImVec2 operators WILL NOT EXIST
target_compile_definitions(imnodeflow PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

# ============================================================
# SLikeNet (CRYPTO DISABLED – OpenSSL 3 SAFE)
# ============================================================
set(SLIKENET_ENABLE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_DLL OFF CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_STATIC ON CACHE BOOL "" FORCE)
set(SLIKENET_ENABLE_CRYPTO OFF CACHE BOOL "" FORCE)

add_subdirectory(SLikeNet)

# ---- FIX SLikeNet on OpenSSL 3 (Arch): remove crypto sources ----
if (TARGET SLikeNetLibStatic)
    get_target_property(_slikenet_srcs SLikeNetLibStatic SOURCES)
    if (_slikenet_srcs)
        list(FILTER _slikenet_srcs EXCLUDE REGEX ".*/crypto/.*")
        set_property(TARGET SLikeNetLibStatic PROPERTY SOURCES "${_slikenet_srcs}")
    endif()

    # Optional: make it explicit to your codebase
    target_compile_definitions(SLikeNetLibStatic PUBLIC SLIKENET_NO_CRYPTO=1)

    add_library(SLikeNet::SLikeNet ALIAS SLikeNetLibStatic)
endif()

# ============================================================
# BGFX STACK (IMPORTED – PREBUILT)
# ============================================================
set(BGFX_BUILD_DIR
        ${CMAKE_CURRENT_LIST_DIR}/bgfx/.build/linux64_gcc/bin
)

function(import_bgfx name file)
    add_library(${name} STATIC IMPORTED GLOBAL)
    set_target_properties(${name} PROPERTIES
            IMPORTED_LOCATION "${BGFX_BUILD_DIR}/${file}"
    )
endfunction()

# Verify actual filenames with:
# ls third_party/bgfx/.build/linux64_gcc/bin
import_bgfx(bx   libbxRelease.a)
import_bgfx(bimg libbimgRelease.a)
import_bgfx(bgfx libbgfxRelease.a)

target_include_directories(bx INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/bx/include
)
target_include_directories(bimg INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/bimg/include
)
target_include_directories(bgfx INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/bgfx/include
)

# Linux-specific requirements
target_link_libraries(bgfx INTERFACE
        dl
        pthread
)
