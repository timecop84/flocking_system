# Build Path Troubleshooting for Flocking System

## 1. Qt Include Errors
If you see errors like `cannot open source file "QOpenGLWidget"` or `QSurfaceFormat`, make sure your Qt installation is correct and your `QTDIR`/`PATH` environment variables are set. For qmake builds, the following should be true:
- `qmake` is in your `PATH`.
- The correct `INCLUDEPATH` for Qt is set. Usually, this is handled by `QT += widgets opengl openglwidgets` in your `.pro` file.
- If using a custom Qt install, add:
  ```
  INCLUDEPATH += C:/Qt/6.x.x/msvc2019_64/include
  INCLUDEPATH += C:/Qt/6.x.x/msvc2019_64/include/QtWidgets
  INCLUDEPATH += C:/Qt/6.x.x/msvc2019_64/include/QtOpenGLWidgets
  ```
  (Adjust for your Qt version and compiler.)

## 2. GLM Include Errors
If you see errors like `cannot open source file "glm/glm.hpp"`:
- Make sure GLM is installed (e.g., via vcpkg or manually).
- If using vcpkg, you already have:
  ```
  INCLUDEPATH += C:/Users/detou/vcpkg/installed/x64-windows/include
  ```
  If GLM is not found, check that `C:/Users/detou/vcpkg/installed/x64-windows/include/glm` exists.
- If not, install GLM:
  ```
  vcpkg install glm
  ```

## 3. General Advice
- Run `qmake` again after editing `.pro` files.
- Clean and rebuild the project.
- If using CMake, ensure `find_package(Qt6 COMPONENTS Widgets OpenGL OpenGLWidgets REQUIRED)` and `find_package(glm REQUIRED)` are present.

## 4. Example Fixes
If you still get errors, add these to your `.pro` file:
```
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtOpenGLWidgets
```
Or, for CMake, add:
```
target_include_directories(flocking_system PRIVATE ${Qt6Widgets_INCLUDE_DIRS} ${Qt6OpenGLWidgets_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
```

## 5. Environment Variables
- Make sure `QTDIR` and `PATH` include your Qt bin directory.
- For vcpkg, run `vcpkg integrate install` if not already done.

---
If you need more help, provide your Qt and vcpkg install locations and the full error message.
