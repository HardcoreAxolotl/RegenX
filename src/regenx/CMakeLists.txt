# =========================================
# File: src/regenx/CMakeLists.txt (ENGINE)
# =========================================
cmake_minimum_required(VERSION 3.26)

# ---- Lua target compatibility (some setups don't define Lua::Lua) ----
# FindLua usually sets LUA_LIBRARIES and LUA_INCLUDE_DIR (or *_DIRS).
if (NOT DEFINED LUA_INCLUDE_DIR AND DEFINED LUA_INCLUDE_DIRS)
    set(LUA_INCLUDE_DIR "${LUA_INCLUDE_DIRS}")
endif()

if (NOT DEFINED LUA_LIBRARIES AND DEFINED LUA_LIBRARY)
    set(LUA_LIBRARIES "${LUA_LIBRARY}")
endif()

if (NOT TARGET Lua::Lua)
    add_library(Lua::Lua INTERFACE IMPORTED)
    set_target_properties(Lua::Lua PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES      "${LUA_LIBRARIES}"
    )
endif()

add_library(regenx_engine STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/core/IO.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/Log.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/platform/Window.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/render/Renderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/SpriteBatch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/TextRenderer.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/audio/AudioDevice.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/ecs/World.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/net/NetClient.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/net/NetServer.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/assets/AssetRegistry.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/ui/DebugUI.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/script/lua/LuaVM.cpp
)

target_include_directories(regenx_engine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}          # allows #include "regenx/..."
        ${CMAKE_CURRENT_SOURCE_DIR}/..       # allows #include "regenx/..."
)

# Link only runtime-safe things here (NO Python)
target_link_libraries(regenx_engine
        PUBLIC
        Threads::Threads

        # System deps
        SDL2::SDL2
        OpenAL::OpenAL
        Freetype::Freetype
        Lua::Lua

        # Third party
        bgfx bx bimg
        imgui imguizmo imnodeflow
        entt sol2 stb

        # Networking
        # If SLikeNet created SLikeNet::SLikeNet alias, prefer it
)

# Link SLikeNet target if it exists
if (TARGET SLikeNet::SLikeNet)
    target_link_libraries(regenx_engine PUBLIC SLikeNet::SLikeNet)
elseif (TARGET SLikeNet)
    target_link_libraries(regenx_engine PUBLIC SLikeNet)
endif()

target_compile_features(regenx_engine PUBLIC cxx_std_20)
