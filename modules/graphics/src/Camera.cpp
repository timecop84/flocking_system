#include "Camera.h"
#include "Vector.h"
#include <iostream>

Camera::Camera(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up, ProjectionType type)
    : m_eye(from), m_look(to), m_up(up), m_projectionType(type)
{
    // Default perspective parameters
    m_fovy = 45.0f;
    m_aspect = 1.0f;
    m_znear = 0.1f;
    m_zfar = 1000.0f;
    
    // Default orthographic parameters
    m_left = -1.0f;
    m_right = 1.0f;
    m_bottom = -1.0f;
    m_top = 1.0f;
    
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::Camera(const Vector& from, const Vector& to, const Vector& up, int type)
    : m_eye(from.m_x, from.m_y, from.m_z), m_look(to.m_x, to.m_y, to.m_z), m_up(up.m_x, up.m_y, up.m_z)
{
    m_projectionType = (type == 0) ? PERSPECTIVE : ORTHOGRAPHIC;
    updateViewMatrix();
    updateProjectionMatrix();
}

void Camera::set(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up)
{
    m_eye = from;
    m_look = to;
    m_up = up;
    updateViewMatrix();
}

void Camera::setEye(const glm::vec3& eye)
{
    m_eye = eye;
    updateViewMatrix();
}

void Camera::setLook(const glm::vec3& look)
{
    m_look = look;
    updateViewMatrix();
}

void Camera::setUp(const glm::vec3& up)
{
    m_up = up;
    updateViewMatrix();
}

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    m_eye = eye;
    m_look = center;
    m_up = up;
    updateViewMatrix();
}

void Camera::setPerspective(float fovy, float aspect, float znear, float zfar)
{
    m_projectionType = PERSPECTIVE;
    m_fovy = fovy;
    m_aspect = aspect;
    m_znear = znear;
    m_zfar = zfar;
    updateProjectionMatrix();
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float znear, float zfar)
{
    m_projectionType = ORTHOGRAPHIC;
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_znear = znear;
    m_zfar = zfar;
    updateProjectionMatrix();
}

void Camera::setShape(float fov, float aspect, float near, float far, int projType)
{
    if (projType == 0) { // PERSPECTIVE
        setPerspective(fov, aspect, near, far);
    } else {
        setOrthographic(-aspect, aspect, -1.0f, 1.0f, near, far);
    }
}

void Camera::setShape(float fovy, float aspect, float znear, float zfar)
{
    m_fovy = fovy;
    m_aspect = aspect;
    m_znear = znear;
    m_zfar = zfar;
    updateProjectionMatrix();
}

void Camera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_eye, m_look, m_up);
}

void Camera::updateProjectionMatrix()
{
    if (m_projectionType == PERSPECTIVE) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fovy), m_aspect, m_znear, m_zfar);
    } else {
        m_projectionMatrix = glm::ortho(m_left, m_right, m_bottom, m_top, m_znear, m_zfar);
    }
}
