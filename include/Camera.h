#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern Camera class using GLM instead of ngl_compat
/// @details Pure C++ camera implementation with modern GLM math
//----------------------------------------------------------------------------------------------------------------------
class Camera {
public:
    enum ProjectionType {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    Camera(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up, ProjectionType type);
    Camera(const class Vector& from, const class Vector& to, const class Vector& up, int type);
    
    void set(const glm::vec3& from, const glm::vec3& to, const glm::vec3& up);
    
    // Matrix getters
    glm::mat4 getViewMatrix() const { return m_viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
    glm::mat4 getVP() const { return m_projectionMatrix * m_viewMatrix; }
    glm::mat4 getVPMatrix() const { return m_projectionMatrix * m_viewMatrix; }
    
    // Position setters/getters
    void setEye(const glm::vec3& eye);
    void setLook(const glm::vec3& look);
    void setUp(const glm::vec3& up);
    void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    
    glm::vec3 getEye() const { return m_eye; }
    glm::vec3 getLook() const { return m_look; }
    glm::vec3 getUp() const { return m_up; }
    
    // Projection settings
    void setPerspective(float fovy, float aspect, float znear, float zfar);
    void setOrthographic(float left, float right, float bottom, float top, float znear, float zfar);
    
    // Default perspective settings
    void setShape(float fovy = 45.0f, float aspect = 1.0f, float znear = 0.1f, float zfar = 1000.0f);
    void setShape(float fov, float aspect, float near, float far, int projType);
    
private:
    void updateViewMatrix();
    void updateProjectionMatrix();
    
    glm::vec3 m_eye;
    glm::vec3 m_look;
    glm::vec3 m_up;
    
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    
    ProjectionType m_projectionType;
    
    // Perspective parameters
    float m_fovy;
    float m_aspect;
    float m_znear;
    float m_zfar;
    
    // Orthographic parameters
    float m_left, m_right, m_bottom, m_top;
};

#endif // CAMERA_H
