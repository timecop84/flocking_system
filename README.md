# Flocking System

## Overview

This project is a real-time, interactive 3D flocking simulation, modernized to use current OpenGL standards and practices. The codebase is a significant update of an original project developed at Bournemouth University (MSc Computer Animation and Visual Effects, NCCA). The system models the collective behavior of autonomous agents ("boids") using classic flocking algorithms, with a focus on modularity, extensibility, and real-time performance.

Key features:

- Real-time rendering of boids and obstacles using OpenGL shaders and uniform buffer objects (UBOs)
- GPU-accelerated simulation. (Nvidia CUDA only)
- Multithreaded simulation for improved performance
- Support for instanced rendering to efficiently display large numbers of boids
- Modular architecture with separate components for simulation, rendering, and user interface
- Interactive Qt-based user interface for adjusting simulation and rendering parameters
- Performance monitoring and validation tools

## Contributors and Acknowledgements

- The boid concept and flocking algorithm are based on the seminal work of Craig Reynolds.
- Original project and modernization: Dennis Toufexis (Bournemouth University, NCCA, 2011)
- Additional contributions and dependencies are listed in the codebase and documentation.

## Dependencies

- Qt 5 or 6 (for GUI and OpenGL integration)
- OpenGL 3.3+
- glad2 (OpenGL Extension Loader)
- GLM (OpenGL Mathematics)
- [vcpkg](https://github.com/microsoft/vcpkg)
- [Chocolatey](https://chocolatey.org/)
- MinGW (for Windows builds)


## Installation

### Using vcpkg (Recommended)

1. Follow the [official vcpkg installation guide](https://github.com/microsoft/vcpkg#quick-start) to set up vcpkg on your system.

2. Install the required libraries:
   ```powershell
   vcpkg install qt5-base glm
   ```
   Note: glad2 is included in the project as source files, so no separate installation is needed.

3. For integration with CMake, follow the [vcpkg CMake integration instructions](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration).

## Build Instructions

### Using MinGW and `mingw32-make`

1. Open a terminal in the project root directory.
2. Run:

   ```bash
   mingw32-make
   ```

   or use the provided batch file:

   ```bash
   build.bat (-help for more details)


## Usage

- Launch the application. The main window provides controls for simulation parameters, rendering options, and obstacle properties.
- Use the UI to adjust flock size, boid and obstacle appearance, and simulation dynamics in real time.
- Keyboard shortcuts and additional controls are documented in the application and code comments.

## License

This project is provided for educational and research purposes. See the LICENSE file for details.
