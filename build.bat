@echo off
REM ============================================================================
REM Modern Flocking System - Automated Build Script
REM ============================================================================
REM This script provides automated building, cleaning, and running capabilities
REM for the Qt6/OpenGL flocking simulation project.
REM
REM Usage:
REM   build.bat [command]
REM
REM Commands:
REM   (no args) - Full clean, build, and run sequence
REM   clean     - Clean build artifacts only
REM   build     - Build project only
REM   rebuild   - Clean and build (no run)
REM   run       - Run existing executable
REM   help      - Show this help message
REM ============================================================================

setlocal enabledelayedexpansion

REM Color codes for output (feature branch: set to empty for cross-platform compatibility)
set "GREEN="
set "RED="
set "YELLOW="
set "BLUE="
set "NC="

REM Project configuration
set "PROJECT_NAME=flock"
set "PROJECT_FILE=flock.pro"
set "EXECUTABLE_NAME=flock.exe"
set "BIN_DIR=bin"
set "RELEASE_DIR=release"

REM Check if we're in the correct directory
if not exist "%PROJECT_FILE%" (
    echo %RED%Error: %PROJECT_FILE% not found in current directory%NC%
    echo %YELLOW%Please run this script from the project root directory%NC%
    exit /b 1
)

REM Parse command line arguments
set "COMMAND=%~1"
if "%COMMAND%"=="" set "COMMAND=all"

REM Show header
echo %BLUE%============================================================================%NC%
echo %BLUE%Modern Flocking System - Build Script%NC%
echo %BLUE%============================================================================%NC%
echo.

REM Process commands
if /i "%COMMAND%"=="help" goto :show_help
if /i "%COMMAND%"=="clean" goto :clean_only
if /i "%COMMAND%"=="build" goto :build_only
if /i "%COMMAND%"=="rebuild" goto :rebuild_only
if /i "%COMMAND%"=="run" goto :run_only
if /i "%COMMAND%"=="all" goto :build_all
if /i "%COMMAND%"=="release" goto :build_release

echo %RED%Error: Unknown command '%COMMAND%'%NC%
echo %YELLOW%Use 'build.bat help' to see available commands%NC%
exit /b 1

:show_help
echo %GREEN%Available Commands:%NC%
echo.
echo %YELLOW%  build.bat%NC%          - Full build sequence (clean + build + run)
echo %YELLOW%  build.bat clean%NC%    - Clean build artifacts only
echo %YELLOW%  build.bat build%NC%    - Build project only  
echo %YELLOW%  build.bat rebuild%NC%  - Clean and build (no run)
echo %YELLOW%  build.bat run%NC%      - Run existing executable
echo %YELLOW%  build.bat help%NC%     - Show this help message
echo %YELLOW%  build.bat release%NC%  - Build project in release mode
echo.
echo %GREEN%Requirements:%NC%
echo   - Qt6 with MinGW64 toolchain
echo   - GLM headers
echo   - OpenGL 3.3+ support
echo.
goto :end

:clean_only
echo %YELLOW%[CLEAN] Cleaning build artifacts...%NC%
call :clean_artifacts
goto :end

:build_only
echo %YELLOW%[BUILD] Building project...%NC%
call :build_project
if !ERRORLEVEL! neq 0 goto :build_failed
echo %GREEN%[SUCCESS] Build completed successfully%NC%
goto :end

:rebuild_only
echo %YELLOW%[REBUILD] Cleaning and building...%NC%
call :clean_artifacts
call :build_project
if !ERRORLEVEL! neq 0 goto :build_failed
echo %GREEN%[SUCCESS] Rebuild completed successfully%NC%
goto :end

:run_only
echo %YELLOW%[RUN] Starting application...%NC%
call :run_application
goto :end

:build_all
echo %YELLOW%[ALL] Starting full build sequence...%NC%
call :clean_artifacts
call :build_project
if !ERRORLEVEL! neq 0 goto :build_failed
call :run_application
goto :end

:build_release
echo %YELLOW%[RELEASE] Building project in release mode...%NC%
qmake "%PROJECT_FILE%" CONFIG+=release "DESTDIR=release/bin" >build.log 2>&1
if !ERRORLEVEL! neq 0 (
    echo %RED%  qmake (release) failed%NC%
    type build.log
    exit /b 1
)
echo %GREEN%  Makefiles (release) generated%NC%
mingw32-make >>build.log 2>&1
if !ERRORLEVEL! neq 0 (
    echo %RED%  Compilation (release) failed%NC%
    type build.log
    exit /b 1
)
echo %GREEN%  Compilation (release) successful%NC%
REM Clean up log file
if exist build.log del build.log >nul 2>&1
goto :eof

REM ============================================================================
REM Helper Functions
REM ============================================================================

:clean_artifacts
echo %BLUE%  - Cleaning previous build artifacts...%NC%

REM Clean Qt-generated files
if exist Makefile (
    echo %BLUE%    * Running make clean...%NC%
    mingw32-make clean >nul 2>&1
)
if exist Makefile.Debug (
    echo %BLUE%    * Cleaning debug build...%NC%
    mingw32-make -f Makefile.Debug clean >nul 2>&1
)
if exist Makefile.Release (
    echo %BLUE%    * Cleaning release build...%NC%
    mingw32-make -f Makefile.Release clean >nul 2>&1
)

REM Remove generated files and directories
for %%d in (debug release %BIN_DIR% obj moc build) do (
    if exist "%%d" (
        echo %BLUE%    * Removing %%d directory...%NC%
        rmdir /s /q "%%d" >nul 2>&1
    )
)

REM Remove generated files
for %%f in (Makefile Makefile.Debug Makefile.Release *.o *.obj ui_*.h moc_*.cpp moc_*.h qrc_*.cpp) do (
    if exist "%%f" (
        echo %BLUE%    * Removing %%f...%NC%
        del /q "%%f" >nul 2>&1
    )
)
echo Clean completed - [OK]%NC%
goto :eof

:build_project
echo Checking Qt6 installation...%NC%
qmake --version >nul 2>&1
if !ERRORLEVEL! neq 0 (
    echo Qt6 qmake not found in PATH - [FAIL]%NC%
    echo Please ensure Qt6 MinGW64 is installed and in PATH%NC%
    exit /b 1
)
echo Qt6 found - [OK]%NC%
echo Generating Makefiles...%NC%
qmake "%PROJECT_FILE%" >build.log 2>&1
if !ERRORLEVEL! neq 0 (
    echo qmake failed - [FAIL]%NC%
    type build.log
    exit /b 1
)
echo Makefiles generated - [OK]%NC%
echo Compiling project...%NC%
mingw32-make >>build.log 2>&1
if !ERRORLEVEL! neq 0 (
    echo Compilation failed - [FAIL]%NC%
    type build.log
    exit /b 1
)
echo Compilation successful - [OK]%NC%
REM Clean up log file
if exist build.log del build.log >nul 2>&1
goto :eof

:run_application
echo Locating executable...%NC%

REM Try multiple possible locations for the executable
set "FOUND_EXE="
for %%d in (%BIN_DIR% %RELEASE_DIR% release/bin debug .) do (
    if exist "%%d\%EXECUTABLE_NAME%" (
        set "FOUND_EXE=%%d\%EXECUTABLE_NAME%"
        goto :found_exe
    )
)

echo Executable not found - [FAIL]%NC%
echo Tried locations: %BIN_DIR%\, %RELEASE_DIR%\, release/bin\, debug\, .\%NC%
echo Please build the project first using 'build.bat build'%NC%
exit /b 1

:found_exe
echo Found executable: !FOUND_EXE! - [OK]%NC%
echo Starting application...%NC%
echo.
echo ======================================%NC%
echo   Starting Flocking Simulation...%NC%
echo ======================================%NC%
echo.

REM Start the application
start "" "!FOUND_EXE!"

REM Wait a moment to see if it starts successfully
timeout /t 2 >nul 2>&1

echo Application launched (if no error appeared) - [OK]%NC%
goto :eof

:build_failed
echo.
echo %RED%============================================================================%NC%
echo %RED%BUILD FAILED%NC%
echo %RED%============================================================================%NC%
echo.
echo %YELLOW%Common solutions:%NC%
echo %YELLOW%  1. Ensure Qt6 with MinGW64 is installed and in PATH%NC%
echo %YELLOW%  2. Check that GLM headers are available%NC%
echo %YELLOW%  3. Verify OpenGL development libraries are installed%NC%
echo %YELLOW%  4. Try running 'build.bat clean' then 'build.bat build'%NC%
echo.
echo %YELLOW%For detailed error information, check the output above.%NC%
echo.
exit /b 1

:end
echo.
echo %BLUE%============================================================================%NC%
echo %GREEN%Build script completed%NC%
echo %BLUE%============================================================================%NC%
endlocal
