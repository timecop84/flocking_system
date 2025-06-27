@echo off
REM Build script for Qt6/MinGW Flocking Simulation
REM This script provides easy clean and build commands

echo ======================================
echo    Qt6/MinGW Flocking Simulation
echo         Build Script
echo ======================================
echo.

if "%1"=="clean" goto clean
if "%1"=="build" goto build
if "%1"=="rebuild" goto rebuild
if "%1"=="run" goto run
goto help

:clean
echo Cleaning build artifacts...
mingw32-make clean
if exist "ui_mainwindow.h" del "ui_mainwindow.h"
if exist "moc\" rmdir /s /q "moc"
if exist "obj\" rmdir /s /q "obj"
if exist "release\" rmdir /s /q "release"
if exist "debug\" rmdir /s /q "debug"
if exist ".qmake.stash" del ".qmake.stash"
if exist "Makefile" del "Makefile"
if exist "Makefile.Debug" del "Makefile.Debug"
if exist "Makefile.Release" del "Makefile.Release"
echo Clean completed!
goto end

:build
echo Building project...
qmake flock.pro
if errorlevel 1 (
    echo ERROR: qmake failed!
    goto end
)
mingw32-make
if errorlevel 1 (
    echo ERROR: Build failed!
    goto end
)
echo Build completed successfully!
echo Executable: release\bin\flock.exe
goto end

:rebuild
echo Rebuilding project (clean + build)...
call :clean
call :build
goto end

:run
echo Starting flocking simulation...
if exist "release\bin\flock.exe" (
    start "" "release\bin\flock.exe"
) else (
    echo ERROR: Executable not found! Please build first.
    echo Run: build.bat build
)
goto end

:help
echo Usage: build.bat [command]
echo.
echo Commands:
echo   clean    - Clean all build artifacts
echo   build    - Build the project
echo   rebuild  - Clean and build
echo   run      - Run the simulation
echo.
echo Examples:
echo   build.bat clean
echo   build.bat build
echo   build.bat rebuild
echo   build.bat run
echo.

:end
echo.
pause
