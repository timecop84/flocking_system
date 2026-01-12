# Flocking System

## Overview

This project is a real-time, interactive 3D flocking simulation, modernized to use current OpenGL standards and practices. The codebase is a significant update of an original project developed at Bournemouth University (MSc Computer Animation and Visual Effects, NCCA). The system models the collective behavior of autonomous agents ("boids") using classic flocking algorithms, with a focus on modularity, extensibility, and real-time performance.

Key features:

- Real-time rendering of boids and obstacles using OpenGL shaders and uniform buffer objects (UBOs)
- GPU-accelerated simulation. (Compute Shader)
- Multithreaded simulation for improved performance
- Support for instanced rendering to efficiently display large numbers of boids
- Modular architecture with separate components for simulation, rendering, and user interface
- ImGui + GLFW interface for adjusting simulation and rendering parameters
- Performance monitoring and validation tools

## Contributors and Acknowledgements

- The boid concept and flocking algorithm are based on the seminal work of Craig Reynolds.
- Original project and modernization: Dennis Toufexis (Bournemouth University, NCCA, 2011)
- Additional contributions and dependencies are listed in the codebase and documentation.

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

## License

This project is provided for educational and research purposes. See the LICENSE file for details.
