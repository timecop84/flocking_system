QT += opengl gui core widgets openglwidgets

INCLUDEPATH += include
INCLUDEPATH += include/ngl_compat
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
    src/ngl_compat/Camera.cpp \
    src/ngl_compat/Light.cpp \
    src/ngl_compat/NGLInit.cpp \
    src/ngl_compat/VAOPrimitives.cpp \
    src/ngl_compat/TransformStack.cpp \
    src/ngl_compat/Random.cpp \
    modules/math/src/MathUtils.cpp

HEADERS += \
    include/mainwindow.h \
    include/GLWindow.h \
    include/boid.h \
    include/flock.h \
    include/obstacle.h \
    include/Behaviours.h \
    include/ngl_compat/Camera.h \
    include/ngl_compat/Light.h \
    include/ngl_compat/NGLInit.h \
    include/ngl_compat/VAOPrimitives.h \
    include/ngl_compat/TransformStack.h \
    include/ngl_compat/Random.h \
    modules/math/include/MathUtils.h \
    include/FlockTypes.h \
    include/ModernRandom.h \
    include/ModernExample.h

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

