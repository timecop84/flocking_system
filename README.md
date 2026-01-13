# Flocking System

## Overview

This project is a real-time, interactive 3D flocking simulation built on modern OpenGL. It is a cleaned-up, GPU-aware evolution of a Bournemouth University NCCA project. Boids flock using the classic cohesion/alignment/separation rules, wrapped in a modular layout that splits simulation, rendering, and input for easier hacking.

- Key features:
  - Real-time OpenGL rendering with UBO-backed shaders
  - CPU and compute-shader GPU simulation paths (toggle with `G`)
  - Instanced boid rendering for large flocks
  - ImGui + GLFW controls for all sim/render knobs
  - Performance/profiler and behavior validator helpers

## Dependencies

- CMake 3.24+
- OpenGL 4.5 capable GPU/driver
- GLM (pulled from `../SandboxGE/external/glm` or `../cloth_solver/external/glm`)
- GLFW (uses `../cloth_solver/external/glfw` if present, otherwise system `glfw3`)
- Dear ImGui (uses `../cloth_solver/external/imgui`)
- glad (bundled in `glad/`)

## Build Instructions

From the project root:

```bash
# Configure + build + run
./build.sh

# Or on Windows
build.bat
```

You can pass `build`, `rebuild`, `run`, or `clean` as the first argument to control the script behaviour. The CMake build is written to `build/` by default.


## Usage

- Launch the executable (`flock`). The ImGui panel lets you tweak flock size, speeds, obstacle properties, bounding box, and shader settings in real time.
- Keyboard shortcuts mirror the legacy app: `G` toggles GPU/CPU, `Space` pauses, `+/-` add or remove boids, `1/2/3/4/0` quick set sizes, `R/T` profiler controls.

## Code map (quick start for contributors)

- `src/GLWindow.cpp` — application wiring, startup, and simulation loop control
- `src/Input.cpp` — GLFW input callbacks (camera orbit/pan/zoom, obstacle drag, hotkeys)
- `modules/graphics/src/Render.cpp` — per-frame rendering and UBO updates
- `src/Flock.cpp` — boid storage, neighbor queries, flock rule application, collisions
- `src/Boid.cpp` — per-boid integration and drawing
- `src/Behaviours.cpp` — cohesion/alignment/separation force calculations
- `modules/graphics/src/GPUFlockingManager.cpp` — compute-shader flocking path

## Contributors and Acknowledgements

- Boid concept and flocking rules from Craig Reynolds
- Original project and modernization: Dennis Toufexis (Bournemouth University, NCCA, 2011)
- Additional contributors and dependencies are noted inline and in project history

## License

This project is provided for educational and research purposes. See the LICENSE file for details.
