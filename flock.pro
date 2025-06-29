QT += opengl gui core widgets openglwidgets

INCLUDEPATH += include
INCLUDEPATH += modules/math/include
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
    src/Behaviours.cpp \
    src/Camera.cpp \
    src/Light.cpp \
    src/TransformStack.cpp \
    src/Vector.cpp \
    src/Colour.cpp \
    src/ShaderLib.cpp \
    src/BBox.cpp \
    src/NGLInit.cpp \
    src/VAOPrimitives.cpp \
    src/Material.cpp \
    src/LegacyRandom.cpp \
    modules/math/src/MathUtils.cpp \
    src/BehaviorValidator.cpp

HEADERS += \
    include/mainwindow.h \
    include/GLWindow.h \
    include/boid.h \
    include/flock.h \
    include/obstacle.h \
    include/Behaviours.h \
    include/Camera.h \
    include/Light.h \
    include/TransformStack.h \
    include/Vector.h \
    include/Colour.h \
    include/ShaderLib.h \
    include/BBox.h \
    include/NGLInit.h \
    include/VAOPrimitives.h \
    include/Material.h \
    include/Matrix.h \
    include/LegacyRandom.h \
    modules/math/include/MathUtils.h \
    include/FlockTypes.h \
    include/ModernRandom.h \
    include/ModernExample.h \
    include/PerformanceMonitor.h \
    include/BehaviorValidator.h

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

