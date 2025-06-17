# Flocking System

A C++/Qt/OpenGL project simulating flocking behavior using the Boids algorithm. The system visualizes groups of autonomous agents ("boids") that exhibit realistic flocking, avoidance, and obstacle navigation in a 3D environment.

## Features

- **Boid Simulation:** Implements classic flocking rules (separation, alignment, cohesion).
- **Obstacle Avoidance:** Boids can detect and avoid obstacles in the environment.
- **Interactive Visualization:** Real-time rendering using OpenGL within a Qt application.
- **Modular Design:** Core logic separated into classes such as `Boid`, `Flock`, `Behaviours`, and `Obstacle`.
- **Customizable Parameters:** Easily adjust flock size, behavior weights, and simulation parameters.
- **Doxygen Documentation:** Auto-generated HTML documentation in the `html/` folder.

## Project Structure

DeferredRenderer/ ├── DeferredRendering.pro # Qt project file ├── Doxyfile # Doxygen configuration for documentation 
├── Makefile # Makefile for building the project ├── include/ # Header files │ ├── camera.h │ ├── FrameBufferObjects.h │ 
├── GeometryPass.h │ ├── GLWindow.h │ ├── GroupedObj.h │ ├── LightPass.h │ ├── LightSetup.h │ ├── MainWindow.h │ ├── Maths.h │ ├── Mtl.h │ ├── Quad.h │ ├── ReadFile.h 
│ └── ShaderManager.h ├── moc/ # Qt meta-object compiler output ├── models/ # 3D models and materials ├── obj/ # Build output ├── shaders/ # GLSL shader programs
├── src/ # Source files ├── textures/ # Texture images └── SponzaMesh.bin, sponzaMtl.bin # Preprocessed Sponza scene data


## Prerequisites

- Qt (for windowing and build system)
- OpenGL 3.3+ compatible GPU
- C++ compiler (supporting C++11 or newer)
- CMake or qmake (depending on your build preference)

## Building

### Using qmake (Qt)

1. Open a terminal and navigate to the project directory.
2. Run:
3. qmake DeferredRendering.pro make

4. Or use Qt Creator to open `DeferredRendering.pro` and build the project.

### Using Makefile

1. In the project directory, run:
2. make

3. 
## Running

After building, run the generated executable (e.g., `DeferredRendering` or `DeferredRenderer.exe`). The application will open a window and render the loaded scene using deferred shading.

## Usage

- Use the mouse and keyboard to navigate the scene (see in-app instructions or code for controls).
- Load different models or adjust lighting parameters as needed.

## Documentation

- Doxygen documentation can be generated using the provided `Doxyfile`:
- doxygen Doxyfile

On Windows, use `nmake` or `jom` instead of `make`.

#### Using Qt Creator

1. Open `flock.pro` in Qt Creator.
2. Configure the project and click **Build**.

## Running

After building, run the executable found in `bin/` (e.g., `bin/flock`). The application window will open and display the flocking simulation.

## Usage

- **Camera Controls:** Use mouse and keyboard to navigate the 3D scene (see code for details).
- **Simulation Controls:** Adjust parameters via the UI (if available) or by editing source/constants.
- **Documentation:** Open `html/index.html` in your browser for full API documentation.

## Main Classes

- **Boid:** Represents an individual agent with position, velocity, and update logic.
- **Flock:** Manages a collection of boids and applies flocking rules.
- **Behaviours:** Encapsulates the logic for separation, alignment, and cohesion.
- **Obstacle:** Defines obstacles for boids to avoid.
- **GLWindow:** OpenGL widget for rendering the simulation.
- **MainWindow:** Main Qt window integrating UI and rendering.

## Documentation

- Run `doxygen Doxyfile` to regenerate documentation.
- Open `html/index.html` for the latest docs.

## Credits

- Based on the Boids algorithm by Craig Reynolds.
- Uses [NGL](https://github.com/NCCA/NGL) for math and OpenGL utilities.
- Developed for educational and research purposes.

## License

See source files for license information.

---

*For more details, see the source code and Doxygen documentation in the `html/` directory.*
