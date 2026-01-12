@echo off
setlocal

set BUILD_DIR=%BUILD_DIR%
if "%BUILD_DIR%"=="" set BUILD_DIR=build

set CONFIG=%CONFIG%
if "%CONFIG%"=="" set CONFIG=Release

set COMMAND=%1
if "%COMMAND%"=="" set COMMAND=run

if /I "%COMMAND%"=="clean" goto :clean
if /I "%COMMAND%"=="build" goto :build
if /I "%COMMAND%"=="rebuild" goto :rebuild
if /I "%COMMAND%"=="run" goto :run
if /I "%COMMAND%"=="help" goto :help

echo Unknown command: %COMMAND%
echo Use build.bat [clean|build|rebuild|run|help]
exit /b 1

:help
echo Usage: build.bat [clean^|build^|rebuild^|run^|help]
echo   clean   - remove build directory
echo   build   - configure and build
echo   rebuild - clean then build
echo   run     - configure, build, and run (default)
exit /b 0

:clean
if exist "%BUILD_DIR%" (
    echo Removing %BUILD_DIR%
    rmdir /s /q "%BUILD_DIR%"
)
exit /b 0

:build
cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 exit /b 1
cmake --build "%BUILD_DIR%" --config %CONFIG%
exit /b %errorlevel%

:rebuild
call "%~f0" clean
call "%~f0" build
exit /b %errorlevel%

:run
call "%~f0" build

set EXE=""
if exist "%BUILD_DIR%\%CONFIG%\flock.exe" set EXE=%BUILD_DIR%\%CONFIG%\flock.exe
if exist "%BUILD_DIR%\flock.exe" set EXE=%BUILD_DIR%\flock.exe
if exist "%BUILD_DIR%\%CONFIG%\flock" set EXE=%BUILD_DIR%\%CONFIG%\flock
if exist "%BUILD_DIR%\flock" set EXE=%BUILD_DIR%\flock

if "%EXE%"=="" (
    echo Executable not found after build.
    exit /b 1
)

echo Running %EXE%
"%EXE%"
exit /b %errorlevel%
