cmake_minimum_required(VERSION 3.24)

project(flocking_system LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate GLFW (prefer vendored copy from cloth_solver)
set(_GLFW_TARGET "")
set(FLOCKING_GLFW_DIR "${CMAKE_SOURCE_DIR}/../cloth_solver/external/glfw")
if(EXISTS "${FLOCKING_GLFW_DIR}/CMakeLists.txt")
    add_subdirectory(${FLOCKING_GLFW_DIR} ${CMAKE_BINARY_DIR}/glfw)
endif()
if(TARGET glfw)
    set(_GLFW_TARGET glfw)
elseif(TARGET glfw3::glfw)
    set(_GLFW_TARGET glfw3::glfw)
endif()
if(NOT _GLFW_TARGET)
    find_package(glfw3 CONFIG REQUIRED)
    if(TARGET glfw)
        set(_GLFW_TARGET glfw)
    elseif(TARGET glfw3::glfw)
        set(_GLFW_TARGET glfw3::glfw)
    else()
        message(FATAL_ERROR "glfw target not found")
    endif()
endif()

# GLM include directory (re-use SandboxGE or cloth_solver copy)
set(FLOCKING_GLM_DIR "${CMAKE_SOURCE_DIR}/../SandboxGE/external/glm")
if(NOT EXISTS "${FLOCKING_GLM_DIR}/glm/glm.hpp")
    set(FLOCKING_GLM_DIR "${CMAKE_SOURCE_DIR}/../cloth_solver/external/glm")
endif()
if(NOT EXISTS "${FLOCKING_GLM_DIR}/glm/glm.hpp")
    message(FATAL_ERROR "GLM headers not found. Expected at ../SandboxGE/external/glm or ../cloth_solver/external/glm")
endif()

# ImGui sources (re-use cloth_solver copy)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/../cloth_solver/external/imgui")
if(NOT EXISTS "${IMGUI_DIR}/imgui.h")
    message(FATAL_ERROR "Dear ImGui not found. Expected at ../cloth_solver/external/imgui")
endif()
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(FLOCKING_SOURCES
    src/main.cpp
    src/GLWindow.cpp
    src/boid.cpp
    src/flock.cpp
    src/obstacle.cpp
    src/SphereGeometry.cpp
    src/Behaviours.cpp
    src/BehaviorValidator.cpp
    src/SpatialHashGrid.cpp
    glad/src/gl.c
    modules/math/src/MathUtils.cpp
    modules/graphics/src/Camera.cpp
    modules/graphics/src/TransformStack.cpp
    modules/graphics/src/Colour.cpp
    modules/graphics/src/Material.cpp
    modules/graphics/src/Light.cpp
    modules/graphics/src/ShaderLib.cpp
    modules/graphics/src/BBox.cpp
    modules/graphics/src/RenderManager.cpp
    modules/graphics/src/UBOCache.cpp
    modules/graphics/src/GeometryFactory.cpp
    modules/graphics/src/ShaderManager.cpp
    modules/graphics/src/TransformBatcher.cpp
    modules/graphics/src/FrameCoordinator.cpp
    modules/graphics/src/InstancedBoidRenderer.cpp
    modules/graphics/src/BoidRenderer.cpp
    modules/graphics/src/ObstacleRenderer.cpp
    modules/graphics/src/GPUFlockingManager.cpp
)

add_executable(flock ${FLOCKING_SOURCES} ${IMGUI_SOURCES})
target_include_directories(flock PRIVATE
    include
    glad/include
    modules/math/include
    modules/graphics/include
    modules/utils/include
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${FLOCKING_GLM_DIR}
)

target_link_libraries(flock PRIVATE ${_GLFW_TARGET} opengl32)
target_compile_definitions(flock PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(flock PRIVATE OpenMP::OpenMP_CXX)
endif()

add_custom_command(TARGET flock POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:flock>/shaders
)
