#ifndef NGL_CAMERA_H
#define NGL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ngl {

enum CameraMode {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Vector; // Forward declaration

class Camera {
public:
    Camera();
    Camera(const Vector& from, const Vector& to, const Vector& up, CameraMode mode = PERSPECTIVE);
    ~Camera();

    void setShape(float fov, float aspect, float nearPlane, float farPlane, CameraMode mode = PERSPECTIVE);
    
    glm::mat4 getViewMatrix() const { return m_viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
    glm::mat4 getVPMatrix() const { return m_projectionMatrix * m_viewMatrix; }
    
    Vector getEye() const;
    
    void lookAt(const Vector& from, const Vector& to, const Vector& up);
    
private:
    glm::vec3 m_eye;
    glm::vec3 m_target;
    glm::vec3 m_up;
    
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;
    
    float m_fov;
    float m_aspect;
    float m_nearPlane;
    float m_farPlane;
    CameraMode m_mode;
    
    void updateViewMatrix();
    void updateProjectionMatrix();
};

} // namespace ngl

#endif // NGL_CAMERA_H