QT += opengl gui core widgets openglwidgets

INCLUDEPATH += include
INCLUDEPATH += modules/math/include
INCLUDEPATH += modules/graphics/include
INCLUDEPATH += modules/utils/include
INCLUDEPATH += C:/Users/detou/vcpkg/installed/x64-windows/include

OBJECTS_DIR = obj/
MOC_DIR = moc/
UI_HEADERS_DIR = ui/

TARGET = bin/flock

# Console application - will run as GUI but have console available for debugging
CONFIG += console

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/GLWindow.cpp \
    src/boid.cpp \
    src/flock.cpp \
    src/obstacle.cpp \
    src/SphereGeometry.cpp \
    src/Behaviours.cpp \
    src/BehaviorValidator.cpp \
    src/SpatialHashGrid.cpp \
    modules/math/src/MathUtils.cpp \
    modules/graphics/src/Camera.cpp \
    modules/graphics/src/TransformStack.cpp \
    modules/graphics/src/Colour.cpp \
    modules/graphics/src/Material.cpp \
    modules/graphics/src/Light.cpp \
    modules/graphics/src/ShaderLib.cpp \
    modules/graphics/src/BBox.cpp \
    modules/graphics/src/RenderManager.cpp \
    modules/graphics/src/UBOCache.cpp \
    modules/graphics/src/GeometryFactory.cpp \
    modules/graphics/src/SmartShaderManager.cpp \
    modules/graphics/src/TransformBatcher.cpp \
    modules/graphics/src/FrameCoordinator.cpp \
    modules/graphics/src/InstancedBoidRenderer.cpp

HEADERS += \
    include/mainwindow.h \
    include/GLWindow.h \
    include/boid.h \
    include/flock.h \
    include/obstacle.h \
    include/SphereGeometry.h \
    include/Behaviours.h \
    include/FlockTypes.h \
    include/ModernExample.h \
    include/BehaviorValidator.h \
    include/SpatialHashGrid.h \
    include/PerformanceProfiler.h \
    modules/math/include/Vector.h \
    modules/math/include/Matrix.h \
    modules/math/include/MathUtils.h \
    modules/graphics/include/Camera.h \
    modules/graphics/include/TransformStack.h \
    modules/graphics/include/Colour.h \
    modules/graphics/include/Material.h \
    modules/graphics/include/Light.h \
    modules/graphics/include/ShaderLib.h \
    modules/graphics/include/BBox.h \
    modules/graphics/include/UBOStructures.h \
    modules/graphics/include/RenderManager.h \
    modules/graphics/include/UBOCache.h \
    modules/graphics/include/GeometryFactory.h \
    modules/graphics/include/SmartShaderManager.h \
    modules/graphics/include/TransformBatcher.h \
    modules/graphics/include/FrameCoordinator.h \
    modules/graphics/include/InstancedBoidRenderer.h \
    modules/utils/include/PerformanceMonitor.h

FORMS += \
    ui/mainwindow.ui

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# define debug flag for development
DEFINES += DEBUG_BUILD

# Link with OpenGL
LIBS += -lopengl32

dependencyPath += include
macx:DEFINES += DARWIN

# MinGW/Windows setup
win32-g++: {
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtOpenGLWidgets
    LIBS += -lopengl32 -lglu32
    # Exclude Qt6 Entry Point and define our own
    LIBS -= -lQt6EntryPoint
    DEFINES += QT_NO_ENTRYPOINT
}

