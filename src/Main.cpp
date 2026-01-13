/**
 * @file main.cpp
 * @brief GLFW/ImGui entry point for the flocking simulation.
 */

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLWindow.h"
#include "Colour.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <chrono>
#include <iostream>

struct UIState {
    int boidCount = 200;
    float boidSize = 1.0f;
    ImVec4 boidColour = {1.0f, 1.0f, 1.0f, 1.0f};
    bool flockWireframe = false;
    float speedMultiplier = 1.0f;

    bool obstacleEnabled = false;  // Start disabled to prevent collision issues at spawn
    ImVec4 obstacleColour = {1.0f, 0.8f, 0.4f, 1.0f};
    ImVec4 obstacleSpecular = {1.0f, 1.0f, 1.0f, 1.0f};
    ImVec4 obstacleDiffuse = {1.0f, 0.8f, 0.4f, 1.0f};
    float obstacleSize = 4.0f;
    float obstaclePos[3] = {0.0f, 0.0f, 0.0f};
    bool obstacleWireframe = false;
    float avoidanceScale = 3.0f;
    float collisionScale = 1.3f;
    float repulsionForce = 0.45f;

    float simDistance = 20.0f;
    float flockDistance = 5.0f;
    float cohesion = 3.0f;       // Moderate cohesion for grouping
    float separation = 5.0f;     // Balanced separation
    float alignment = 2.0f;      // Low alignment for natural movement

    float bboxSize = 120.0f;
    ImVec4 backgroundColour = {0.6f, 0.6f, 0.6f, 1.0f};
};

static void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

static void setup_imgui_style() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowRounding = 6.0f;

    ImVec4 accent = ImVec4(0.98f, 0.55f, 0.22f, 1.0f);
    style.Colors[ImGuiCol_Text]           = ImVec4(0.93f, 0.95f, 0.96f, 1.0f);
    style.Colors[ImGuiCol_WindowBg]       = ImVec4(0.11f, 0.12f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive]  = ImVec4(0.14f, 0.15f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_Header]         = accent;
    style.Colors[ImGuiCol_HeaderHovered]  = ImVec4(1.0f, 0.63f, 0.30f, 1.0f);
    style.Colors[ImGuiCol_Button]         = accent;
    style.Colors[ImGuiCol_ButtonHovered]  = ImVec4(1.0f, 0.63f, 0.30f, 0.9f);
    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.16f, 0.18f, 0.22f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab]     = ImVec4(1.0f, 0.63f, 0.30f, 1.0f);
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Flocking System (ImGui)", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLWindow app;
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    app.initialize(fbWidth, fbHeight);

    UIState ui;
    Colour bg; bg.set(ui.backgroundColour.x, ui.backgroundColour.y, ui.backgroundColour.z);
    app.applyFlock(ui.boidCount);
    app.setBoidSize(ui.boidSize);
    app.setBoidColor(glm::vec3(ui.boidColour.x, ui.boidColour.y, ui.boidColour.z));
    app.setFlockWireframe(ui.flockWireframe);
    app.setFlockSpeedMultiplier(ui.speedMultiplier);
    app.setObstacleEnabled(ui.obstacleEnabled);
    app.setObstacleColour(glm::vec3(ui.obstacleColour.x, ui.obstacleColour.y, ui.obstacleColour.z));
    app.setObstacleSpecular(ui.obstacleSpecular.x, ui.obstacleSpecular.y, ui.obstacleSpecular.z);
    app.setObstacleDiffuse(ui.obstacleDiffuse.x, ui.obstacleDiffuse.y, ui.obstacleDiffuse.z);
    app.setObstacleSize(ui.obstacleSize);
    app.setObstaclePosition(glm::vec3(ui.obstaclePos[0], ui.obstaclePos[1], ui.obstaclePos[2]));
    app.setObstacleWireframe(ui.obstacleWireframe);
    app.setObstacleAvoidanceRadiusScale(ui.avoidanceScale);
    app.setObstacleCollisionRadiusScale(ui.collisionScale);
    app.setObstacleRepulsionForce(ui.repulsionForce);
    app.setSimDistance(ui.simDistance);
    app.setSimFlockDistance(ui.flockDistance);
    app.setSimCohesion(ui.cohesion);
    app.setSimSeparation(ui.separation);
    app.setSimAlignment(ui.alignment);
    app.setBackgroundColour(bg);
    app.setBBoxSize(glm::vec3(ui.bboxSize));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    setup_imgui_style();

    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 450");

    glfwSetWindowUserPointer(window, &app);
    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods){
        ImGui_ImplGlfw_MouseButtonCallback(win, button, action, mods);
        auto* appPtr = static_cast<GLWindow*>(glfwGetWindowUserPointer(win));
        if (!appPtr) return;
        if (ImGui::GetIO().WantCaptureMouse) return;
        double x, y;
        glfwGetCursorPos(win, &x, &y);
        appPtr->onMouseButton(x, y, button, action, mods);
    });
    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset){
        ImGui_ImplGlfw_ScrollCallback(win, xoffset, yoffset);
        auto* appPtr = static_cast<GLWindow*>(glfwGetWindowUserPointer(win));
        if (!appPtr) return;
        if (ImGui::GetIO().WantCaptureMouse) return;
        appPtr->onScroll(xoffset, yoffset);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y){
        ImGui_ImplGlfw_CursorPosCallback(win, x, y);
        auto* appPtr = static_cast<GLWindow*>(glfwGetWindowUserPointer(win));
        if (!appPtr) return;
        if (ImGui::GetIO().WantCaptureMouse) return;
        bool left = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        bool right = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
        bool middle = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
        bool shift = glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        bool ctrl = glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
        appPtr->onMouseMove(x, y, left, right, middle, shift, ctrl);
    });
    glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods){
        ImGui_ImplGlfw_KeyCallback(win, key, scancode, action, mods);
        auto* appPtr = static_cast<GLWindow*>(glfwGetWindowUserPointer(win));
        if (!appPtr) return;
        if (ImGui::GetIO().WantCaptureKeyboard) return;
        appPtr->onKey(key, action);
    });

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        int newFbW, newFbH;
        glfwGetFramebufferSize(window, &newFbW, &newFbH);
        if (newFbW != fbWidth || newFbH != fbHeight) {
            fbWidth = newFbW;
            fbHeight = newFbH;
            app.resize(fbWidth, fbHeight);
        }

        auto now = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        app.updateSimulation(deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Flocking Controls");
        ImGui::Text("FPS: %.1f | Boids: %d | Mode: %s", app.getCurrentFPS(), app.getCurrentBoidSize(),
                    app.isGPUModeEnabled() ? "GPU" : "CPU");
        ImGui::Text("Threads: %d | GPU: %s", app.getThreadCount(), app.getGPUName().c_str());

        {
            auto stats = app.getBoidStats();
            ImGui::Text("Pos min: (%.2f, %.2f, %.2f)", stats.min.x, stats.min.y, stats.min.z);
            ImGui::Text("Pos max: (%.2f, %.2f, %.2f)", stats.max.x, stats.max.y, stats.max.z);
            ImGui::Text("Pos avg: (%.2f, %.2f, %.2f) | NaNs: %d", stats.avg.x, stats.avg.y, stats.avg.z, stats.nanCount);
        }
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Flock", ImGuiTreeNodeFlags_DefaultOpen)) {
            // RESET BUTTON - prominent at top
            if (ImGui::Button("RESET FLOCK", ImVec2(-1, 30))) {
                app.resetFlock();
                ui.boidCount = app.getCurrentBoidSize();
            }
            ImGui::Spacing();
            
            ImGui::SliderInt("Boid Count", &ui.boidCount, 20, 4000);
            if (ImGui::Button("Apply Count")) {
                app.applyFlock(ui.boidCount);
            }
            ImGui::SameLine();
            if (ImGui::Button("+10")) {
                app.addBoidsToFlock();
                ui.boidCount = app.getCurrentBoidSize();
            }
            ImGui::SameLine();
            if (ImGui::Button("-10")) {
                app.removeBoidsFromFlock();
                ui.boidCount = app.getCurrentBoidSize();
            }
            if (ImGui::SliderFloat("Boid Size", &ui.boidSize, 0.5f, 10.0f, "%.2f")) {
                app.setBoidSize(ui.boidSize);
            }
            if (ImGui::ColorEdit3("Boid Colour", &ui.boidColour.x)) {
                app.setBoidColor(glm::vec3(ui.boidColour.x, ui.boidColour.y, ui.boidColour.z));
            }
            if (ImGui::Checkbox("Flock Wireframe", &ui.flockWireframe)) {
                app.setFlockWireframe(ui.flockWireframe);
            }
            if (ImGui::SliderFloat("Flock Speed", &ui.speedMultiplier, 0.1f, 3.0f, "%.2fx")) {
                app.setFlockSpeedMultiplier(ui.speedMultiplier);
            }
        }

        if (ImGui::CollapsingHeader("Obstacle", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Enable Obstacle", &ui.obstacleEnabled)) {
                app.setObstacleEnabled(ui.obstacleEnabled);
            }
            if (ImGui::SliderFloat3("Position", ui.obstaclePos, -99.0f, 99.0f, "%.1f")) {
                app.setObstaclePosition(glm::vec3(ui.obstaclePos[0], ui.obstaclePos[1], ui.obstaclePos[2]));
            }
            if (ImGui::SliderFloat("Size", &ui.obstacleSize, 1.0f, 10.0f, "%.2f")) {
                app.setObstacleSize(ui.obstacleSize);
            }
            if (ImGui::ColorEdit3("Colour", &ui.obstacleColour.x)) {
                app.setObstacleColour(glm::vec3(ui.obstacleColour.x, ui.obstacleColour.y, ui.obstacleColour.z));
            }
            if (ImGui::ColorEdit3("Specular", &ui.obstacleSpecular.x)) {
                app.setObstacleSpecular(ui.obstacleSpecular.x, ui.obstacleSpecular.y, ui.obstacleSpecular.z);
            }
            if (ImGui::ColorEdit3("Diffuse", &ui.obstacleDiffuse.x)) {
                app.setObstacleDiffuse(ui.obstacleDiffuse.x, ui.obstacleDiffuse.y, ui.obstacleDiffuse.z);
            }
            if (ImGui::Checkbox("Wireframe", &ui.obstacleWireframe)) {
                app.setObstacleWireframe(ui.obstacleWireframe);
            }
            if (ImGui::SliderFloat("Avoidance Radius", &ui.avoidanceScale, 1.0f, 10.0f, "%.2f")) {
                app.setObstacleAvoidanceRadiusScale(ui.avoidanceScale);
            }
            if (ImGui::SliderFloat("Collision Radius", &ui.collisionScale, 1.0f, 6.0f, "%.2f")) {
                app.setObstacleCollisionRadiusScale(ui.collisionScale);
            }
            if (ImGui::SliderFloat("Repulsion Force", &ui.repulsionForce, 0.1f, 2.0f, "%.2f")) {
                app.setObstacleRepulsionForce(ui.repulsionForce);
            }
        }

        if (ImGui::CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::SliderFloat("Neighbourhood Distance", &ui.simDistance, 10.0f, 100.0f, "%.1f")) {
                app.setSimDistance(ui.simDistance);
            }
            if (ImGui::SliderFloat("Flock Distance", &ui.flockDistance, 1.0f, 10.0f, "%.1f")) {
                app.setSimFlockDistance(ui.flockDistance);
            }
            if (ImGui::SliderFloat("Cohesion", &ui.cohesion, 0.0f, 10.0f, "%.1f")) {
                app.setSimCohesion(ui.cohesion);
            }
            if (ImGui::SliderFloat("Separation", &ui.separation, 0.0f, 20.0f, "%.1f")) {
                app.setSimSeparation(ui.separation);
            }
            if (ImGui::SliderFloat("Alignment", &ui.alignment, 0.0f, 20.0f, "%.1f")) {
                app.setSimAlignment(ui.alignment);
            }
        }

        if (ImGui::CollapsingHeader("Environment", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::SliderFloat("Bounding Box", &ui.bboxSize, 50.0f, 220.0f, "%.1f")) {
                app.setBBoxSize(glm::vec3(ui.bboxSize));
            }
            if (ImGui::ColorEdit3("Background", &ui.backgroundColour.x)) {
                Colour c; c.set(ui.backgroundColour.x, ui.backgroundColour.y, ui.backgroundColour.z);
                app.setBackgroundColour(c);
            }
        }

        ImGui::End();

        ImGui::Render();

        app.render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
