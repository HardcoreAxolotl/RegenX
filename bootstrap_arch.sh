#!/usr/bin/env bash
set -euo pipefail

# RegenX bootstrap + organizer for Arch Linux
# Run from project root:  ./tools/bootstrap_arch.sh
#
# Optional flags:
#   --no-pacman   Skip pacman install
#   --no-clone    Skip cloning third_party repos
#   --no-skeleton Skip generating skeleton files
#   --no-todo     Skip generating TODO list
#
# Safe behavior:
# - Never overwrites existing files
# - Creates directories if missing
# - Clones repos only if not already present

NO_PACMAN=0
NO_CLONE=0
NO_SKELETON=0
NO_TODO=0

for arg in "$@"; do
  case "$arg" in
    --no-pacman)   NO_PACMAN=1 ;;
    --no-clone)    NO_CLONE=1 ;;
    --no-skeleton) NO_SKELETON=1 ;;
    --no-todo)     NO_TODO=1 ;;
    *) echo "Unknown arg: $arg" >&2; exit 1 ;;
  esac
done

ROOT="$(pwd)"
if [[ ! -f "$ROOT/CMakeLists.txt" ]]; then
  echo "ERROR: Run this from the RegenX project root (where CMakeLists.txt is)." >&2
  exit 1
fi

today() { date +"%Y-%m-%d"; }

say() { echo -e "\n==> $*"; }

mkdir -p tools

# ---------------------------
# 1) Install packages (Arch)
# ---------------------------
if [[ $NO_PACMAN -eq 0 ]]; then
  say "Installing Arch packages (pacman)..."

  # Core build tools + pkg-config
  # SDL2 for window/input; OpenAL for audio; FreeType for fonts; Lua/Python/Mono for scripting targets.
  sudo pacman -Syu --needed --noconfirm \
    base-devel git cmake ninja pkgconf \
    sdl2 \
    openal \
    freetype2 \
    lua \
    python python-pip \
    mono \
    glm \
    zlib

  # BGFX backends often need X11 libs on Linux. Wayland libs are optional but useful.
  sudo pacman -S --needed --noconfirm \
    libx11 libxrandr libxi libxcursor libxinerama \
    mesa libglvnd \
    wayland wayland-protocols

  # Vulkan headers are useful if you want BGFX Vulkan backend available at build time.
  sudo pacman -S --needed --noconfirm \
    vulkan-headers

  # Optional helpers (keep them installed, even if you don't use them immediately):
  # - assimp: future 3D model import
  # - box2d: optional physics
  # - pybind11: python bindings helper (header-only)
  sudo pacman -S --needed --noconfirm \
    pybind11 \
    assimp \
    box2d || true

  say "Pacman install done."
else
  say "Skipping pacman install (--no-pacman)."
fi

# ---------------------------------
# 2) Create a clean directory layout
# ---------------------------------
say "Creating/validating project layout..."

# Keep your existing src/regenx/ layout, but expand it to a scalable engine+editor structure.
mkdir -p \
  src/regenx/{core,platform,input,render,audio,net,ecs,assets,ui,scripting} \
  src/regenx/scripting/{lua,python,csharp} \
  editor/{src,include,ui,visual,assets} \
  third_party \
  docs \
  tests

# Keep existing lua tests folder structure, but also create a standard tests root.
mkdir -p src/regenx/script/lua/tests || true

# Add a .gitignore if missing (and ensure build dirs are ignored)
if [[ ! -f .gitignore ]]; then
  cat > .gitignore <<'EOF'
# Build outputs
cmake-build-*/
build/
out/

# IDE
.idea/
.vscode/
*.user
*.suo

# OS
.DS_Store

# Third-party build artifacts (if you build them in-place)
third_party/**/build/
third_party/**/bin/
EOF
else
  # Append common build dirs if not present
  if ! grep -q "cmake-build-" .gitignore; then
    cat >> .gitignore <<'EOF'

# Build outputs
cmake-build-*/
build/
out/
EOF
  fi
fi

# ---------------------------------------
# 3) Clone third_party (BGFX + tool stack)
# ---------------------------------------
if [[ $NO_CLONE -eq 0 ]]; then
  say "Cloning/updating third_party libraries..."

  cd third_party

  # BGFX stack: bgfx + bx + bimg side-by-side is the normal setup.
  # Use --recursive to fetch submodules where needed.
  if [[ ! -d bx ]]; then
    git clone --depth 1 https://github.com/bkaradzic/bx.git
  fi
  if [[ ! -d bimg ]]; then
    git clone --depth 1 https://github.com/bkaradzic/bimg.git
  fi
  if [[ ! -d bgfx ]]; then
    git clone --depth 1 https://github.com/bkaradzic/bgfx.git
  fi

  # Dear ImGui
  if [[ ! -d imgui ]]; then
    git clone --depth 1 https://github.com/ocornut/imgui.git
  fi

  # ImGuizmo
  if [[ ! -d ImGuizmo ]]; then
    git clone --depth 1 https://github.com/CedricGuillemet/ImGuizmo.git
  fi

  # ImNodeFlow (RegenXVisual)
  if [[ ! -d ImNodeFlow ]]; then
    git clone --depth 1 https://github.com/Fattorino/ImNodeFlow.git
  fi

  # sol2 (Lua bindings)
  if [[ ! -d sol2 ]]; then
    git clone --depth 1 https://github.com/ThePhD/sol2.git
  fi

  # EnTT (ECS)
  if [[ ! -d entt ]]; then
    git clone --depth 1 https://github.com/skypjack/entt.git
  fi

  # SLikeNet (TCP+UDP)
  if [[ ! -d SLikeNet ]]; then
    git clone --depth 1 https://github.com/SLikeSoft/SLikeNet.git
  fi

  # stb headers (image + write) – grab the few files you actually need
  mkdir -p stb
  if [[ ! -f stb/stb_image.h ]]; then
    curl -L --fail -o stb/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
  fi
  if [[ ! -f stb/stb_image_write.h ]]; then
    curl -L --fail -o stb/stb_image_write.h https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h
  fi

  cd "$ROOT"
  say "third_party ready."
else
  say "Skipping cloning third_party (--no-clone)."
fi

# ---------------------------------------------------
# 4) Generate module skeletons (no overwrites)
# ---------------------------------------------------
make_hpp() {
  local path="$1"
  local guard_note="$2"
  if [[ -f "$path" ]]; then
    return 0
  fi
  mkdir -p "$(dirname "$path")"
  cat > "$path" <<EOF
// $guard_note
// Created: $(today)
// RegenX

#pragma once

namespace regenx {

} // namespace regenx
EOF
}

make_cpp() {
  local path="$1"
  local note="$2"
  if [[ -f "$path" ]]; then
    return 0
  fi
  mkdir -p "$(dirname "$path")"
  cat > "$path" <<EOF
// $note
// Created: $(today)
// RegenX

#include "$(basename "${path%.*}").hpp"

namespace regenx {

} // namespace regenx
EOF
}

if [[ $NO_SKELETON -eq 0 ]]; then
  say "Generating skeleton files (only missing ones)..."

  # Engine runtime modules (bgfx-friendly approach)
  make_hpp src/regenx/render/Renderer.hpp "Render system interface (BGFX-backed)"
  make_cpp src/regenx/render/Renderer.cpp "Render system implementation (BGFX-backed)"

  make_hpp src/regenx/render/SpriteBatch.hpp "Sprite batching (2D now, extendable to 3D)"
  make_cpp src/regenx/render/SpriteBatch.cpp "Sprite batching implementation"

  make_hpp src/regenx/render/TextRenderer.hpp "Text rendering (FreeType + atlas)"
  make_cpp src/regenx/render/TextRenderer.cpp "Text rendering implementation"

  make_hpp src/regenx/audio/AudioDevice.hpp "Audio device abstraction (OpenAL Soft)"
  make_cpp src/regenx/audio/AudioDevice.cpp "Audio device implementation"

  make_hpp src/regenx/net/NetClient.hpp "Networking client (TCP+UDP via SLikeNet)"
  make_cpp src/regenx/net/NetClient.cpp "Networking client implementation"

  make_hpp src/regenx/net/NetServer.hpp "Networking server (TCP+UDP via SLikeNet)"
  make_cpp src/regenx/net/NetServer.cpp "Networking server implementation"

  make_hpp src/regenx/assets/AssetRegistry.hpp "Asset registry & caching"
  make_cpp src/regenx/assets/AssetRegistry.cpp "Asset registry implementation"

  make_hpp src/regenx/ui/DebugUI.hpp "Runtime debug UI (ImGui, optional in runtime)"
  make_cpp src/regenx/ui/DebugUI.cpp "Runtime debug UI implementation"

  make_hpp src/regenx/ecs/World.hpp "ECS world wrapper (EnTT registry wrapper)"
  make_cpp src/regenx/ecs/World.cpp "ECS world implementation"

  # Editor modules
  make_hpp editor/include/RegenXEditor.hpp "RegenX Editor entry API"
  make_cpp editor/src/RegenXEditor.cpp "RegenX Editor implementation"

  make_hpp editor/visual/VisualGraph.hpp "RegenXVisual (ImNodeFlow wrapper)"
  make_cpp editor/visual/VisualGraph.cpp "RegenXVisual implementation"

  say "Skeleton generation done."
else
  say "Skipping skeleton generation (--no-skeleton)."
fi

# -----------------------------------------
# 5) Generate TODO list (capabilities map)
# -----------------------------------------
if [[ $NO_TODO -eq 0 ]]; then
  say "Generating docs/TODO_LIBS.md..."

  cat > docs/TODO_LIBS.md <<EOF
# RegenX Capability TODO List (Library-Driven)
Generated: $(today)

This is a **menu** of what your chosen libraries enable.
Nothing here is mandatory. It’s a list you can scan and decide what to implement.

---

## Platform / Window / Input (SDL2)

- [ ] Window creation, resize, fullscreen/windowed
- [ ] Input: keyboard, mouse, controller, hotplug
- [ ] Event loop abstraction (engine-facing)
- [ ] Clipboard, cursors, relative mouse mode (FPS camera)
- [ ] High-res timing (frame delta, fixed timestep)
- [ ] File path helpers (base path / pref path)

---

## Rendering (BGFX)

- [ ] Renderer init/shutdown (BGFX platform attach)
- [ ] Render pipeline: views, passes, framebuffers
- [ ] Resource mgmt: shaders, textures, buffers, uniforms
- [ ] 2D sprite rendering (quads)
- [ ] Batching and sorting (sprites, UI)
- [ ] Offscreen render targets (editor viewport, post-processing)
- [ ] Debug draw (lines, boxes, axes)
- [ ] 3D ready: cameras, meshes, materials (future)

---

## Sprites / Images (stb_image)

- [ ] Load images into CPU memory
- [ ] Upload to BGFX texture
- [ ] Texture atlas support (pack, UV mapping)
- [ ] Sprite animation (frames, timing)
- [ ] Sprite transforms (pos/rot/scale, flip, pivot)

---

## Text Rendering (FreeType + optional stb_truetype)

- [ ] FreeType font load (TTF/OTF)
- [ ] Glyph rasterization + atlas caching
- [ ] Kerning and layout
- [ ] Unicode text rendering
- [ ] Signed Distance Field text (optional upgrade)
- [ ] Debug fonts: use ImGui baked font as fallback

---

## UI (Dear ImGui + ImGuizmo)

- [ ] Runtime debug UI (FPS, memory, logs, scene stats)
- [ ] Docking editor layout (panels like Unity/Unreal)
- [ ] Inspector panel (edit component fields live)
- [ ] Hierarchy/outliner (entity list)
- [ ] Viewport panel (render texture -> ImGui)
- [ ] Transform gizmo in viewport (ImGuizmo)

---

## RegenXVisual (ImNodeFlow)

- [ ] Node graph editor UI
- [ ] Graph serialization format (json/binary)
- [ ] Graph -> IR (intermediate representation)
- [ ] IR -> Lua code generator
- [ ] IR -> C++ code generator
- [ ] IR -> Python tool scripts (optional)
- [ ] Graph validation (types, cycles, missing links)
- [ ] Editor integration (drag-drop assets into nodes)

---

## Audio (OpenAL Soft)

- [ ] Audio device init/shutdown
- [ ] Load/stream audio (you choose codec path later)
- [ ] 2D sounds (UI, SFX)
- [ ] 3D positional audio (listener/source positions)
- [ ] Volume groups (music/sfx/ambient)
- [ ] Spatial attenuation model + doppler (optional)

---

## Networking (SLikeNet - TCP + UDP)

- [ ] Client connect/disconnect
- [ ] Server hosting and discovery (optional)
- [ ] Reliable UDP channels (game state)
- [ ] TCP channel (out-of-band: chat, auth, downloads)
- [ ] Packet serialization layer (bitstreams)
- [ ] Entity replication (opt-in components)
- [ ] Snapshot + interpolation model (typical real-time netcode)

---

## ECS / World Model (EnTT)

- [ ] Entity create/destroy
- [ ] Component add/remove
- [ ] System scheduler (update order)
- [ ] Scene loading/saving (via your serializer)
- [ ] Runtime tags (enabled/disabled, layers)
- [ ] Editor selection and live editing

---

## Scripting

### Lua (Lua 5.4 + sol2)
- [ ] Lua VM wrapper
- [ ] Bind engine API: input, transforms, rendering hooks
- [ ] Script component: Start/Update hooks
- [ ] Hot reload scripts (dev-only)

### Python (Python3 + pybind11)
- [ ] Editor automation scripts (import pipeline)
- [ ] Tool scripts (asset processing, generation)
- [ ] Optional: in-engine dev console commands

### C# (Mono) (optional)
- [ ] Managed runtime host
- [ ] Bindings / glue layer
- [ ] Script compilation pipeline (heavier)

---

EOF

  say "TODO list written to docs/TODO_LIBS.md"
else
  say "Skipping TODO generation (--no-todo)."
fi

say "Done. Next: wire CMake targets (engine vs editor) and link BGFX/ImGui/SLikeNet cleanly."
