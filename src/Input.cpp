// Input.cpp
// Handles user input for the ImGui/GLFW app: camera orbit/pan/zoom, obstacle dragging, and hotkeys for flock controls.

#include "GLWindow.h"
#include "PerformanceProfiler.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <limits>
#include <cmath>
#include <GPUFlockingManager.h>

void GLWindow::onMouseMove(double x, double y, bool leftDown, bool rightDown, bool middleDown, bool shift, bool ctrl)
{
    m_shiftDown = shift;
    m_ctrlDown = ctrl;
    m_leftMouseDown = leftDown;
    m_rightMouseDown = rightDown;
    m_middleMouseDown = middleDown;

    // Obstacle dragging has priority
    if (m_obstacleSelected && leftDown) {
        if (shift) {
            m_obstacleDragPlane = ObstacleDragPlane::XZ;
        } else if (ctrl) {
            m_obstacleDragPlane = ObstacleDragPlane::YZ;
        } else {
            m_obstacleDragPlane = ObstacleDragPlane::XY;
        }
        float nx = 2.0f * static_cast<float>(x) / static_cast<float>(m_viewportWidth) - 1.0f;
        float ny = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(m_viewportHeight);
        glm::vec4 rayClip(nx, ny, -1.0f, 1.0f);
        glm::mat4 proj = m_cam->getProjectionMatrix();
        glm::mat4 view = m_cam->getViewMatrix();
        glm::vec4 rayEye = glm::inverse(proj) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
        glm::vec3 camPos = m_cam->getEye();
        glm::vec3 hitPoint{std::numeric_limits<float>::quiet_NaN()};
        if (m_obstacleDragPlane == ObstacleDragPlane::XY) {
            float denom = rayWorld.z;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.z) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::XZ) {
            float denom = rayWorld.y;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.y) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        } else if (m_obstacleDragPlane == ObstacleDragPlane::YZ) {
            float denom = rayWorld.x;
            if (fabs(denom) > 1e-6f) {
                float t = (m_obstacleDragPlaneZ - camPos.x) / denom;
                hitPoint = camPos + t * rayWorld;
            }
        }
        if (!std::isnan(hitPoint.x) && !std::isnan(hitPoint.y) && !std::isnan(hitPoint.z)) {
            obstacle->setSpherePosition(Vector(hitPoint.x, hitPoint.y, hitPoint.z));
            m_needsRedraw = true;
        }
        m_lastMousePos = glm::vec2(x, y);
        return;
    }

    if(m_rotate && leftDown)
    {
        int diffx=static_cast<int>(x)-m_origX;
        int diffy=static_cast<int>(y)-m_origY;
        
        m_cameraAzimuth += diffx * 0.5f;
        m_cameraElevation += diffy * 0.5f;
        
        if (m_cameraElevation > 85.0f) m_cameraElevation = 85.0f;
        if (m_cameraElevation < -85.0f) m_cameraElevation = -85.0f;
        
        if (m_cameraAzimuth > 360.0f) m_cameraAzimuth -= 360.0f;
        if (m_cameraAzimuth < 0.0f) m_cameraAzimuth += 360.0f;
        
        updateCameraPosition();
        
        m_origX = static_cast<int>(x);
        m_origY = static_cast<int>(y);
        m_needsRedraw = true;
    }
    else if(m_translate && rightDown)
    {
        int diffX = static_cast<int>(x) - m_origXPos;
        int diffY = static_cast<int>(y) - m_origYPos;
        
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=static_cast<int>(x);
        m_origYPos=static_cast<int>(y);
        m_needsRedraw = true;
    }
    else if(m_pan && middleDown)
    {
        int diffX = static_cast<int>(x) - m_origXPos;
        int diffY = static_cast<int>(y) - m_origYPos;
        
        float panSpeed = 0.1f;
        m_cameraTarget.m_x += panSpeed * diffX;
        m_cameraTarget.m_y -= panSpeed * diffY;
        
        updateCameraPosition();
        
        m_origXPos=static_cast<int>(x);
        m_origYPos=static_cast<int>(y);
        m_needsRedraw = true;
    }

    m_lastMousePos = glm::vec2(x, y);
}

void GLWindow::onMouseButton(double x, double y, int button, int action, int mods)
{
    m_shiftDown = (mods & GLFW_MOD_SHIFT) != 0;
    m_ctrlDown = (mods & GLFW_MOD_CONTROL) != 0;
    bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        m_leftMouseDown = pressed;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        m_rightMouseDown = pressed;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        m_middleMouseDown = pressed;
    }

    if (pressed && button == GLFW_MOUSE_BUTTON_LEFT) {
        float nx = 2.0f * static_cast<float>(x) / static_cast<float>(m_viewportWidth) - 1.0f;
        float ny = 1.0f - 2.0f * static_cast<float>(y) / static_cast<float>(m_viewportHeight);
        glm::vec4 rayClip(nx, ny, -1.0f, 1.0f);
        glm::mat4 proj = m_cam->getProjectionMatrix();
        glm::mat4 view = m_cam->getViewMatrix();
        glm::vec4 rayEye = glm::inverse(proj) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);
        glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
        glm::vec3 camPos = m_cam->getEye();
        glm::vec3 sphereCenter = glm::vec3(obstacle->getPositionModern().x, obstacle->getPositionModern().y, obstacle->getPositionModern().z);
        float sphereRadius = obstacle->getSphereRadius();
        float tHit;
        if (intersectRaySphere(camPos, rayWorld, sphereCenter, sphereRadius, tHit)) {
            m_obstacleSelected = true;
            m_lastMousePos = glm::vec2(x, y);
            glm::vec3 hitPoint = camPos + tHit * rayWorld;
            m_obstacleDragStartWorld = Vector(hitPoint.x, hitPoint.y, hitPoint.z);
            if (m_shiftDown) {
                m_obstacleDragPlane = ObstacleDragPlane::XZ;
                m_obstacleDragPlaneZ = hitPoint.y;
            } else if (m_ctrlDown) {
                m_obstacleDragPlane = ObstacleDragPlane::YZ;
                m_obstacleDragPlaneZ = hitPoint.x;
            } else {
                m_obstacleDragPlane = ObstacleDragPlane::XY;
                m_obstacleDragPlaneZ = hitPoint.z;
            }
            return;
        }
        m_origX = static_cast<int>(x);
        m_origY = static_cast<int>(y);
        m_rotate = true;
    }
    else if(pressed && button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        m_origXPos = static_cast<int>(x);
        m_origYPos = static_cast<int>(y);
        m_translate = true;
    }
    else if(pressed && button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        m_origXPos = static_cast<int>(x);
        m_origYPos = static_cast<int>(y);
        m_pan = true;
    }

    if (!pressed) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            m_rotate = false;
            if (m_obstacleSelected) {
                m_obstacleSelected = false;
            }
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            m_translate = false;
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            m_pan = false;
        }
    }

    m_lastMousePos = glm::vec2(x, y);
}

void GLWindow::onScroll(double /*xoffset*/, double yoffset)
{
    float zoomFactor = 10.0f;
    
    if(yoffset > 0)
    {
        m_cameraDistance -= zoomFactor;
        if (m_cameraDistance < 5.0f) m_cameraDistance = 5.0f;
    }
    else if(yoffset < 0 )
    {
        m_cameraDistance += zoomFactor;
        if (m_cameraDistance > 1500.0f) m_cameraDistance = 1500.0f;
    }
    
    updateCameraPosition();
    m_needsRedraw = true;
}

void GLWindow::onKey(int key, int action)
{
    if (action != GLFW_PRESS) {
        return;
    }

    switch (key) {
        case GLFW_KEY_P:
            printPerformanceComparison();
            break;
        case GLFW_KEY_R:
            PerformanceProfiler::getInstance().printReport();
            break;
        case GLFW_KEY_T:
            PerformanceProfiler::getInstance().reset();
            std::cout << "Performance profiler statistics reset." << std::endl;
            break;
        case GLFW_KEY_M:
            setPerformanceMonitoring(!m_performanceMonitor.isEnabled());
            break;
        case GLFW_KEY_C:
            m_performanceMonitor.clear();
            break;
        case GLFW_KEY_SPACE:
            m_animate = !m_animate;
            break;
        case GLFW_KEY_V:
            validateBehaviorDifferences();
            break;
        case GLFW_KEY_G:
            if (m_gpuFlockingManager) {
                bool wasEnabled = m_gpuFlockingManager->isEnabled();
                m_gpuFlockingManager->toggleGPUMode();
                std::cout << "Switched from " << (wasEnabled ? "GPU" : "CPU") 
                          << " to " << (m_gpuFlockingManager->isEnabled() ? "GPU" : "CPU") 
                          << " flocking mode" << std::endl;
            } else {
                std::cout << "GPU flocking manager not available" << std::endl;
            }
            break;
        case GLFW_KEY_KP_ADD:
        case GLFW_KEY_EQUAL:
            if (flock) {
                for (int i = 0; i < 5; i++) {
                    flock->addBoids();
                }
                std::cout << "Added boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
        case GLFW_KEY_MINUS:
            if (flock) {
                for (int i = 0; i < 5 && flock->getFlockSize() > 10; i++) {
                    flock->removeBoids();
                }
                std::cout << "Removed boids. Total: " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_1:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 500;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_2:
            if (flock) {
                int currentSize = flock->getFlockSize();
                int target = 1000;
                std::cout << "Setting flock size from " << currentSize << " to " << target << std::endl;
                
                if (currentSize < target) {
                    while (flock->getFlockSize() < target) {
                        flock->addBoids();
                    }
                } else if (currentSize > target) {
                    while (flock->getFlockSize() > target) {
                        flock->removeBoids();
                    }
                }
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_3:
            if (flock) {
                int target = 2000;
                std::cout << "Setting flock size to " << target << " - TARGET!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_4:
            if (flock) {
                int target = 4000;
                std::cout << "Setting flock size to " << target << " - MASSIVE!" << std::endl;
                flock->setFlockSize(target);
                std::cout << "Flock size set to " << flock->getFlockSize() << std::endl;
            }
            break;
        case GLFW_KEY_0:
            if (flock) {
                int target = 200;
                std::cout << "Resetting flock size to " << target << " (default)" << std::endl;
                
                while (flock->getFlockSize() < target) {
                    flock->addBoids();
                }
                while (flock->getFlockSize() > target) {
                    flock->removeBoids();
                }
                std::cout << "Flock size reset to " << flock->getFlockSize() << std::endl;
            }
            break;
        default:
            break;
    }
}

void GLWindow::updateCameraPosition()
{
    float radAzimuth = glm::radians(m_cameraAzimuth);
    float radElevation = glm::radians(m_cameraElevation);
    
    float x = m_cameraDistance * cos(radElevation) * cos(radAzimuth);
    float y = m_cameraDistance * sin(radElevation);
    float z = m_cameraDistance * cos(radElevation) * sin(radAzimuth);
    
    Vector cameraPos(m_cameraTarget.m_x + x, m_cameraTarget.m_y + y, m_cameraTarget.m_z + z);
    Vector up(0, 1, 0);
    
    m_cam->lookAt(cameraPos, m_cameraTarget, up);
}

bool GLWindow::intersectRaySphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& tHit) const {
    glm::vec3 oc = rayOrigin - sphereCenter;
    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;
    tHit = (-b - sqrt(discriminant)) / (2.0f * a);
    return tHit >= 0;
}
