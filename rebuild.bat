@echo off
echo ====================================
echo Rebuilding Flocking System
echo ====================================

:: Delete old build artifacts
echo Cleaning up old files...
if exist debug\bin\flock.exe del debug\bin\flock.exe
if exist release\bin\flock.exe del release\bin\flock.exe
if exist obj\*.o del obj\*.o

:: Run make
echo Compiling...
mingw32-make clean
mingw32-make -j4

:: Check if build was successful
if exist release\bin\flock.exe (
    echo ====================================
    echo Build successful!
    echo ====================================
    echo Starting application...
    cd release\bin
    flock.exe
) else (
    echo ====================================
    echo Build failed!
    echo ====================================
)
