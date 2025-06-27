#include "../include/ngl_compat/Camera.h"
#include "../include/ngl_compat/Vector.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ngl {

Camera::Camera() : m_eye(0.0f, 0.0f, 10.0f), m_target(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f),
                   m_fov(45.0f), m_aspect(1.0f), m_nearPlane(0.1f), m_farPlane(100.0f), m_mode(PERSPECTIVE) {
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::Camera(const Vector& from, const Vector& to, const Vector& up, CameraMode mode) 
    : m_eye(from.m_x, from.m_y, from.m_z), m_target(to.m_x, to.m_y, to.m_z), m_up(up.m_x, up.m_y, up.m_z),
      m_fov(45.0f), m_aspect(1.0f), m_nearPlane(0.1f), m_farPlane(100.0f), m_mode(mode) {
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::~Camera() {
}

void Camera::setShape(float fov, float aspect, float nearPlane, float farPlane, CameraMode mode) {
    m_fov = fov;
    m_aspect = aspect;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_mode = mode;
    updateProjectionMatrix();
}

Vector Camera::getEye() const {
    return Vector(m_eye.x, m_eye.y, m_eye.z);
}

void Camera::lookAt(const Vector& from, const Vector& to, const Vector& up) {
    m_eye = glm::vec3(from.m_x, from.m_y, from.m_z);
    m_target = glm::vec3(to.m_x, to.m_y, to.m_z);
    m_up = glm::vec3(up.m_x, up.m_y, up.m_z);
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_eye, m_target, m_up);
}

void Camera::updateProjectionMatrix() {
    if (m_mode == PERSPECTIVE) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearPlane, m_farPlane);
    } else {
        float halfWidth = m_fov * m_aspect * 0.5f;
        float halfHeight = m_fov * 0.5f;
        m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_nearPlane, m_farPlane);
    }
}

} // namespace ngl