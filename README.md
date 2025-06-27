# Qt6/MinGW Flocking Simulation

A modernized C++/Qt6/OpenGL project simulating flocking behavior using the Boids algorithm. Originally built with NGL/Qt4, this project has been successfully ported to Qt 6.9.1 with MinGW on Windows, featuring a GLM-based compatibility layer and modern orbital camera controls.

## Features

- **Boid Simulation:** Implements classic flocking rules (separation, alignment, cohesion)
- **Obstacle Avoidance:** Interactive spherical obstacle with real-time position/size controls
- **Modern Orbital Camera:** Smooth camera controls with azimuth/elevation/distance-based movement
- **Interactive Visualization:** Real-time OpenGL rendering with immediate mode fallbacks
- **Modular Design:** Core logic separated into `Boid`, `Flock`, `Behaviours`, and `Obstacle` classes
- **Customizable Parameters:** Real-time adjustment of all simulation parameters via UI
- **Qt6 Modern UI:** Updated interface with color pickers and parameter sliders
- **Cross-Platform Build:** MinGW/Qt6 compatibility maintained

## Project Structure

```
flocking_system/
├── flock.pro                 # Qt6 project file
├── build.bat                 # Windows build script
├── .gitignore                # Git ignore rules
├── Doxyfile                  # Doxygen configuration
├── README.md                 # This file
├── include/                  # Header files
│   ├── ngl_compat/          # GLM-based NGL compatibility layer
│   │   ├── Camera.h         # Orbital camera implementation
│   │   ├── Vector.h         # GLM vector wrapper
│   │   ├── Matrix.h         # GLM matrix wrapper
│   │   ├── TransformStack.h # Transform stack for OpenGL
│   │   ├── VAOPrimitives.h  # Immediate mode sphere rendering
│   │   ├── Colour.h         # Color utilities
│   │   ├── BBox.h          # Bounding box wireframe
│   │   └── ...             # Other compatibility headers
│   ├── mainwindow.h         # Main Qt window
│   ├── GLWindow.h           # OpenGL rendering widget
│   ├── boid.h               # Individual boid agent
│   ├── flock.h              # Flock management
│   ├── Behaviours.h         # Flocking behaviors
│   └── obstacle.h           # Obstacle avoidance
├── src/                     # Source files
│   ├── ngl_compat/         # Compatibility layer implementation
│   ├── main.cpp            # Application entry point
│   ├── mainwindow.cpp      # UI event handling
│   ├── GLWindow.cpp        # OpenGL rendering logic
│   ├── boid.cpp            # Boid implementation
│   ├── flock.cpp           # Flock management
│   ├── Behaviours.cpp      # Behavior algorithms
│   └── obstacle.cpp        # Obstacle implementation
├── ui/                     # Qt UI files
└── shaders/                # GLSL shaders (legacy)
```

## Prerequisites

### Required Software

1. **Qt 6.9.1** (or compatible Qt6 version)
   - Download from [Qt Official Website](https://www.qt.io/download)
   - Install with MinGW 64-bit compiler
   - Required modules: Core, Widgets, OpenGL, OpenGLWidgets

2. **MinGW 64-bit** (usually included with Qt installer)
   - GCC compiler with C++17 support
   - Make utilities (mingw32-make)

3. **GLM (OpenGL Mathematics)**
   - Download from [GLM GitHub](https://github.com/g-truc/glm)
   - Extract to `C:\libs\glm\` (or adjust path in `flock.pro`)

4. **OpenGL**
   - OpenGL 3.3+ compatible GPU
   - Compatibility Profile support required

### Environment Setup

1. **Add Qt to PATH:**
   ```bash
   C:\Qt\6.9.1\mingw_64\bin
   C:\Qt\Tools\mingw1120_64\bin
   ```

2. **Verify installations:**
   ```bash
   qmake --version
   mingw32-make --version
   g++ --version
   ```

## Building and Running

### Method 1: Using Custom Makefile (Cross-Platform)

1. **Clone/Download the project:**
   ```bash
   git clone <your-repo-url>
   cd flocking_system
   ```

2. **Build the project:**
   ```bash
   make -f Makefile.custom build
   ```

3. **Run the simulation:**
   ```bash
   make -f Makefile.custom run
   ```

### Method 2: Using Build Script (Windows)

1. **Clone/Download the project:**
   ```bash
   git clone <your-repo-url>
   cd flocking_system
   ```

2. **Build the project:**
   ```bash
   # In PowerShell (default Windows terminal):
   .\build.bat build
   
   # In Command Prompt (cmd):
   build.bat build
   ```

3. **Run the simulation:**
   ```bash
   # In PowerShell:
   .\build.bat run
   
   # In Command Prompt:
   build.bat run
   ```

### Method 3: Manual Build

1. **Generate Makefiles:**
   ```bash
   qmake flock.pro
   ```

2. **Compile:**
   ```bash
   mingw32-make
   ```

3. **Run:**
   ```bash
   release\bin\flock.exe
   ```

### Custom Makefile Commands

- `make -f Makefile.custom build` - Build the project
- `make -f Makefile.custom clean` - Clean build artifacts
- `make -f Makefile.custom rebuild` - Clean and build
- `make -f Makefile.custom run` - Run the simulation
- `make -f Makefile.custom check` - Verify build environment
- `make -f Makefile.custom docs` - Generate documentation

### Build Script Commands (Windows)

**PowerShell (recommended):**
- `.\build.bat clean` - Clean all build artifacts
- `.\build.bat build` - Build the project
- `.\build.bat rebuild` - Clean and build
- `.\build.bat run` - Run the simulation

**Command Prompt (cmd):**
- `build.bat clean` - Clean all build artifacts
- `build.bat build` - Build the project
- `build.bat rebuild` - Clean and build
- `build.bat run` - Run the simulation

### Qt Creator (Alternative)

1. Open `flock.pro` in Qt Creator
2. Configure with MinGW kit
3. Build and run (Ctrl+R)

## Usage

### Camera Controls

- **Left Mouse:** Orbital rotation (azimuth/elevation)
- **Right Mouse:** Pan camera position
- **Middle Mouse:** Pan camera position (alternative)
- **Mouse Wheel:** Zoom in/out (distance-based)

### UI Controls

- **Flock Parameters:**
  - Boid count, size, and color
  - Separation, cohesion, and alignment weights
  - Simulation distance and flock distance

- **Obstacle Controls:**
  - Position (X, Y, Z sliders)
  - Size adjustment
  - Color picker
  - Wireframe toggle

- **Visualization:**
  - Background color picker
  - Bounding box size
  - Wireframe mode toggle

### Keyboard Shortcuts

- Camera controls are fully mouse-driven
- All parameters accessible via UI sliders and buttons

## Dependencies

### Runtime Dependencies

- **Qt6 Core** (6.9.1+)
- **Qt6 Widgets** (6.9.1+)
- **Qt6 OpenGL** (6.9.1+)
- **Qt6 OpenGLWidgets** (6.9.1+)
- **OpenGL 3.3+** (Compatibility Profile)
- **MinGW Runtime** (included with MinGW)

### Development Dependencies

- **GLM** (Header-only library)
- **MinGW GCC** (C++17 support)
- **Qt6 Development Tools** (qmake, moc, uic)

## Troubleshooting

### Build Issues

1. **"build.bat not recognized" (PowerShell)**
   - Use `.\build.bat` instead of `build.bat` in PowerShell
   - Alternative: Use Command Prompt (cmd) where `build.bat` works directly

2. **"qmake not found"**
   - Add Qt bin directory to PATH
   - Verify Qt installation

3. **"GLM headers not found"**
   - Check GLM path in `flock.pro`
   - Ensure GLM is extracted to correct location

4. **"OpenGL context creation failed"**
   - Update graphics drivers
   - Verify OpenGL 3.3+ support

### Runtime Issues

1. **Qt6 DLL errors**
   - Ensure Qt bin directory in PATH
   - Copy required DLLs to executable directory

2. **Black screen/no rendering**
   - Check OpenGL context creation
   - Verify graphics driver compatibility

## Architecture

### NGL Compatibility Layer

The project uses a custom compatibility layer (`ngl_compat/`) that provides GLM-based implementations of NGL classes:

- **Vector/Matrix:** GLM wrappers for mathematical operations
- **Camera:** Modern orbital camera with smooth controls
- **TransformStack:** OpenGL matrix stack management
- **VAOPrimitives:** Immediate mode rendering for spheres
- **BBox:** Wireframe bounding box visualization

### Main Classes

- **Boid:** Individual agent with position, velocity, and flocking behavior
- **Flock:** Collection manager for boids with update logic
- **Behaviours:** Implementation of separation, alignment, and cohesion rules
- **Obstacle:** Spherical obstacle with avoidance behavior
- **GLWindow:** OpenGL rendering widget with camera and drawing logic
- **MainWindow:** Qt main window with UI controls and event handling

## Documentation

Generate Doxygen documentation:
```bash
doxygen Doxyfile
```
Open `html/index.html` for complete API documentation.

## Credits

- **Original Algorithm:** Based on Boids by Craig Reynolds
- **Original NGL Version:** NCCA/Bournemouth University
- **Qt6/MinGW Port:** Modernized for current development environments
- **GLM Integration:** OpenGL Mathematics library for math operations

## License

See source files for license information.

---

*Successfully ported from legacy NGL/Qt4 to modern Qt6.9.1/MinGW with full feature compatibility.*
