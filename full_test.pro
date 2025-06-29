TEMPLATE = app
CONFIG += console
CONFIG += qt

QT += opengl gui core widgets openglwidgets

SOURCES += \
    full_test.cpp \
    modules/math/src/MathUtils.cpp \
    modules/graphics/src/ShaderManager.cpp

INCLUDEPATH += ./
INCLUDEPATH += include/
INCLUDEPATH += modules
INCLUDEPATH += modules/math/include
INCLUDEPATH += modules/graphics/include
INCLUDEPATH += modules/core/include
INCLUDEPATH += C:/Users/detou/vcpkg/installed/x64-windows/include

LIBS += -lopengl32 -lglu32

# Destination directory for the executable
DESTDIR = ./release/bin
