# Modular Graphics and Simulation Library

A simplified, modular C++ library providing reusable components for 3D graphics, mathematical operations, and utility functions. Originally developed as part of a flocking simulation system, these modules are designed to be easily integrated into other graphics and simulation projects.

## ✅ Modernization Status

**COMPLETED**: Full modernization and simplification of the flocking system:
- ✅ Removed all legacy and obsolete code (LegacyRandom, NGLInit, VAOPrimitives, ShaderManager stubs)
- ✅ Migrated reusable components to modular structure under `modules/`
- ✅ **SIMPLIFIED INTERFACES**: Eliminated redundant wrapper files (FlockLib.h, FlockMath.h)
- ✅ **CONSOLIDATED MATH MODULE**: All math utilities now in single `MathUtils.h` with unified namespace
- ✅ Modernized random number generation with thread-safe singleton pattern
- ✅ Added comprehensive documentation and usage examples
- ✅ Updated build system and verified successful compilation and execution
- ✅ **STREAMLINED STRUCTURE**: Direct module includes, no wrapper layers

## 🏗️ Simplified Architecture

The library now uses a **direct inclusion model** with no wrapper layers:

```
modules/
├── math/
│   ├── include/
│   │   ├── Vector.h         # 3D vector operations (header only)
│   │   ├── Matrix.h         # 4x4 matrix transformations (header only)
│   │   └── MathUtils.h      # ALL math utilities + constants + type aliases
│   └── src/
│       └── MathUtils.cpp    # Complete math implementation (Vector + utilities)
├── graphics/
│   ├── include/
│   │   ├── Camera.h         # 3D camera with projection/view matrices
│   │   ├── Colour.h         # RGBA color class
│   │   ├── Material.h       # Material properties for rendering
│   │   ├── TransformStack.h # Hierarchical transform management
│   │   ├── Light.h          # Lighting system
│   │   ├── ShaderLib.h      # Shader program management
│   │   └── BBox.h           # Axis-aligned bounding box with OpenGL drawing
│   └── src/
│       ├── Camera.cpp
│       ├── Colour.cpp
│       ├── Material.cpp
│       ├── TransformStack.cpp
│       ├── Light.cpp
│       ├── ShaderLib.cpp
│       └── BBox.cpp
└── utils/
    ├── include/
    │   └── PerformanceMonitor.h # Performance monitoring utilities
    └── src/
        └── PerformanceMonitor.cpp
```



## 📦 Modules

### Math Module (`modules/math/`)
**Unified and Consolidated**:
- **Vector & Matrix**: 3D vector and 4x4 matrix classes (interface in headers)
- **Single Implementation**: All math code consolidated in `MathUtils.cpp`
- **Type Aliases**: `math::Vec3`, `math::Mat4` for convenience
- **Constants**: `math::PI`, `math::TWO_PI`, etc.
- **Utilities**: `math::utils::clamp()`, `lerp()`, `distance()`, `randomVector()`, etc.
- **All in one include**: `#include "MathUtils.h"` (via include path)

**Benefits of Consolidation**:
- Single compilation unit for all math operations
- Reduced build complexity and faster compilation
- Logical grouping of related functionality
- Easier maintenance and debugging

### Graphics Module (`modules/graphics/`)
- **Camera**: 3D camera class with perspective/orthographic projection
- **Colour**: RGB/RGBA color class with various formats
- **Material**: Material properties for rendering
- **TransformStack**: Hierarchical transformation matrix stack
- **Light**: Lighting system with position, color, and attenuation
- **ShaderLib**: GLSL shader program management and uniform handling
- **BBox**: Axis-aligned bounding box with OpenGL drawing and collision detection
- **Core functionality**: Camera, color management, materials, transforms, lighting, shaders, bounding boxes
- **Direct includes**: Individual header files for specific functionality

### Utils Module (`modules/utils/`)
- **PerformanceMonitor**: Performance timing and profiling utilities
- **Random**: Thread-safe modern random number generation
- **Core functionality**: Performance monitoring utilities
- **Direct includes**: `PerformanceMonitor.h` for performance tracking

## 🚀 Usage (Simplified)

### Recommended: Direct module includes
```cpp
// Include only what you need
#include "modules/math/include/MathUtils.h"        // Math utilities and random functions
#include "modules/graphics/include/Camera.h"       // Camera functionality
#include "modules/graphics/include/Colour.h"       // Color management
#include "modules/utils/include/PerformanceMonitor.h" // Performance monitoring

// Use simplified namespaces
math::Vec3 position(0, 0, 0);
math::utils::clamp(value, 0.0f, 1.0f);
Camera camera;
math::utils::getRandomFloat();
```

### Alternative: Individual includes
```cpp
// Include specific components
#include "modules/math/include/Vector.h"
#include "modules/graphics/include/Camera.h"
math::Vec3 position(1.0f, 2.0f, 3.0f);
Color red(1.0f, 0.0f, 0.0f);
```

### Option 3: Include specific headers
```cpp
#include "modules/math/include/Vector.h"
#include "modules/graphics/include/Camera.h"

Vector cameraPos(10.0f, 5.0f, 10.0f);
// ... use Vector and Camera directly
```

## 🛠️ Integration

### CMake Integration
```cmake
# Add the modules to your include path
target_include_directories(your_target PRIVATE 
    path/to/flocking_system/modules/math/include
    path/to/flocking_system/modules/graphics/include
    path/to/flocking_system/modules/utils/include
)

# Add source files
target_sources(your_target PRIVATE
    path/to/flocking_system/modules/math/src/MathUtils.cpp
    path/to/flocking_system/modules/graphics/src/Camera.cpp
    path/to/flocking_system/modules/graphics/src/Colour.cpp
    path/to/flocking_system/modules/graphics/src/Material.cpp
    path/to/flocking_system/modules/graphics/src/TransformStack.cpp
    path/to/flocking_system/modules/graphics/src/Light.cpp
    path/to/flocking_system/modules/graphics/src/ShaderLib.cpp
    path/to/flocking_system/modules/graphics/src/BBox.cpp
    path/to/flocking_system/modules/utils/src/PerformanceMonitor.cpp
    # Add other .cpp files as needed
)
```

### QMake Integration
```make
INCLUDEPATH += path/to/flocking_system/modules/math/include
INCLUDEPATH += path/to/flocking_system/modules/graphics/include  
INCLUDEPATH += path/to/flocking_system/modules/utils/include

SOURCES += \
    path/to/flocking_system/modules/math/src/MathUtils.cpp \
    path/to/flocking_system/modules/graphics/src/Camera.cpp \
    path/to/flocking_system/modules/graphics/src/Colour.cpp \
    path/to/flocking_system/modules/graphics/src/Material.cpp \
    path/to/flocking_system/modules/graphics/src/TransformStack.cpp \
    path/to/flocking_system/modules/graphics/src/Light.cpp \
    path/to/flocking_system/modules/graphics/src/ShaderLib.cpp \
    path/to/flocking_system/modules/graphics/src/BBox.cpp \
    path/to/flocking_system/modules/utils/src/PerformanceMonitor.cpp \
    # ... other source files
```

## 📋 Examples

### Basic Math Operations
```cpp
#include "modules/math/include/Vector.h"

Vector a(1, 2, 3);
Vector b(4, 5, 6);
Vector sum = a + b;                    // Vector addition
float dot = a.dot(b);                  // Dot product
Vector normalized = a.normalized();    // Unit vector
```

### Camera Setup
```cpp
#include "modules/graphics/include/Camera.h"

Vector eye(0, 0, 10);
Vector center(0, 0, 0);
Vector up(0, 1, 0);

Camera camera(eye, center, up, PERSPECTIVE);
camera.setShape(45.0f, 16.0f/9.0f, 0.1f, 100.0f, PERSPECTIVE);
```

### Transform Hierarchies
```cpp
#include "modules/graphics/include/TransformStack.h"

TransformStack transforms;
transforms.pushMatrix();           // Save current transform
transforms.translate(10, 0, 0);    // Move 10 units on X
transforms.rotate(45, 0, 1, 0);    // Rotate 45° around Y
// ... render objects with this transform
transforms.popMatrix();            // Restore previous transform
```

## 🔧 Dependencies

- **OpenGL**: For graphics functionality
- **GLM**: For additional math operations (optional)
- **Qt6**: For some utility classes (can be made optional)

## 📝 License

This library is part of the Flocking System project. See the main project README for license information.

## 🤝 Contributing

This modular library is designed to be extensible. To add new modules:

1. Create a new directory under `modules/`
2. Add `include/` and `src/` subdirectories
3. Create a module header (e.g., `YourModule.h`) in `include/`
4. Update `modules/FlockLib.h` to include your module
5. Update build files to include your sources

## 📚 Documentation

For detailed API documentation, see the individual header files. Each class and function is documented with Doxygen-style comments.
