QT += opengl gui core


INCLUDEPATH += include

OBJECTS_DIR = obj/
MOC_DIR = moc/
UI_HEADERS_DIR = ui/


TARGET = bin/flock

CONFIG += console

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/GLWindow.cpp \
    src/boid.cpp \
    src/flock.cpp \
    src/obstacle.cpp \
    src/Behaviours.cpp

HEADERS += \
    include/mainwindow.h \
    include/GLWindow.h \
    include/boid.h \
    include/flock.h \
    include/obstacle.h \
    include/Behaviours.h

FORMS += \
    ui/mainwindow.ui

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
macx:INCLUDEPATH+=/usr/local/boost/
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

# define the _DEBUG flag for the graphics lib
DEFINES +=NGL_DEBUG


LIBS += -L/usr/local/lib
LIBS +=  -L/$(HOME)/NGL/lib -l NGL
INCLUDEPATH += $$(HOME)/NGL/include/

# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++ {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
DEPENDPATH+=include
# if we are on a mac define DARWIN
macx:DEFINES += DARWIN

# this is where to look for includes
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH += $$(HOME)/boost-trunk/
win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+=-I c:/boost

        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib
        LIBS+= -lmingw32
        DEFINES += WIN32
        DEFINES += USING_GLEW
        DEFINES +=GLEW_STATIC
        DEFINES+=_WIN32
        SOURCES+=C:/NGL/Support/glew/glew.c
        INCLUDEPATH+=C:/NGL/Support/glew/
}

