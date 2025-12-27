# RegenX Capability TODO List (Library-Driven)
Generated: 2025-12-27

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

