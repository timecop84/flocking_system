# Flocking System

## Overview

This project is a real-time, interactive 3D flocking simulation, modernized to use current OpenGL standards and practices. The codebase is a significant update of an original project developed at Bournemouth University (MSc Computer Animation and Visual Effects, NCCA). The system models the collective behavior of autonomous agents ("boids") using classic flocking algorithms, with a focus on modularity, extensibility, and real-time performance.

Key features:
- Real-time rendering of boids and obstacles using OpenGL shaders and uniform buffer objects (UBOs)
- Interactive Qt-based user interface for adjusting simulation and rendering parameters
- Support for wireframe and solid rendering modes
- Modular architecture separating simulation, rendering, and UI logic
- Performance monitoring and validation tools

## Contributors and Acknowledgements

- The boid concept and flocking algorithm are based on the seminal work of Craig Reynolds.
- Original project and modernization: [Your Name] (Bournemouth University, NCCA)
- Additional contributions and dependencies are listed in the codebase and documentation.

## Dependencies

- Qt 5 or 6 (for GUI and OpenGL integration)
- OpenGL 3.3 or higher
- GLEW (OpenGL Extension Wrangler)
- GLM (OpenGL Mathematics)
- [vcpkg](https://github.com/microsoft/vcpkg) (recommended for C++ dependency management)
- [Chocolatey](https://chocolatey.org/) (optional, for Windows package management)
- MinGW (for Windows builds)
- CMake (if using CMake-based build)

## Installation

### Using vcpkg (Recommended)

1. Follow the [official vcpkg installation guide](https://github.com/microsoft/vcpkg#quick-start) to set up vcpkg on your system.

2. Install the required libraries:
   ```powershell
   vcpkg install qt5-base glew glm
   ```

3. For integration with CMake, follow the [vcpkg CMake integration instructions](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration).

### Using Chocolatey (Windows)

1. Install [Chocolatey](https://chocolatey.org/install) if not already present.

2. Install dependencies:
   ```powershell
   choco install qt5 glew glm
   ```

3. Ensure MinGW is installed:
   ```powershell
   choco install mingw
   ```

4. (Optional) Install CMake:
   ```powershell
   choco install cmake
   ```

## Build Instructions

### Using MinGW and `mingw32-make`

1. Open a terminal in the project root directory.
2. Run:
   ```bash
   mingw32-make
   ```
   or use the provided batch file:
   ```bash
   build.bat
   ```

### Using CMake (if CMakeLists.txt is provided)

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Configure the project:
   ```bash
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
   ```
3. Build:
   ```bash
   cmake --build .
   ```

## Usage

- Launch the application. The main window provides controls for simulation parameters, rendering options, and obstacle properties.
- Use the UI to adjust flock size, boid and obstacle appearance, and simulation dynamics in real time.
- Keyboard shortcuts and additional controls are documented in the application and code comments.

## License

This project is provided for educational and research purposes. See the LICENSE file for details.
