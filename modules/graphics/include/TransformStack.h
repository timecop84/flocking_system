#ifndef TRANSFORMSTACK_H
#define TRANSFORMSTACK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include "Matrix.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern TransformStack using GLM instead of ngl_compat
/// @details Pure C++ transform stack implementation with modern GLM math
//----------------------------------------------------------------------------------------------------------------------
class TransformStack {
public:
    TransformStack() {
        m_stack.push(glm::mat4(1.0f));
        m_projection = glm::mat4(1.0f);
        m_view = glm::mat4(1.0f);
        m_globalTransform = glm::mat4(1.0f);
    }
    
    void pushTransform() {
        m_stack.push(m_stack.top());
    }
    
    void popTransform() {
        if (m_stack.size() > 1) {
            m_stack.pop();
        }
    }
    
    void setPosition(const glm::vec3& pos);
    void setScale(float x, float y, float z);
    void setScale(const glm::vec3& scale);
    
    // Matrix management for modern OpenGL compatibility
    void setProjection(const glm::mat4& projection) {
        m_projection = projection;
    }
    
    void setView(const glm::mat4& view) {
        m_view = view;
    }
    
    void setModel(const glm::mat4& model) {
        // Update the current top of the stack
        if (!m_stack.empty()) {
            m_stack.pop();
            m_stack.push(model);
        }
    }
    
    glm::mat4 getProjectionMatrix() const { return m_projection; }
    glm::mat4 getViewMatrix() const { return m_view; }
    glm::mat4 getModelMatrix() const { return m_stack.empty() ? glm::mat4(1.0f) : m_stack.top(); }
    
    void setGlobal(const glm::mat4& transform) {
        m_globalTransform = transform;
    }
    
    glm::mat4 getCurrentTransform() const {
        return m_stack.empty() ? glm::mat4(1.0f) : m_stack.top();
    }
    
    Matrix getCurrAndGlobal() const {
        glm::mat4 current = getCurrentTransform();
        return Matrix(m_globalTransform * current);
    }
    
private:
    std::stack<glm::mat4> m_stack;
    glm::mat4 m_globalTransform;
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

#endif // TRANSFORMSTACK_H
