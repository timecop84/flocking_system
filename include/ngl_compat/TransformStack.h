#ifndef NGL_TRANSFORMSTACK_H
#define NGL_TRANSFORMSTACK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

namespace ngl {

// Forward declaration to avoid circular includes
class Vector;

class Transformation {
public:
    Transformation() : m_matrix(1.0f) {}
    
    void setMatrix(const glm::mat4& matrix) { m_matrix = matrix; }
    glm::mat4 getMatrix() const { return m_matrix; }
    
private:
    glm::mat4 m_matrix;
};

class TransformStack {
public:
    TransformStack() {
        m_stack.push(glm::mat4(1.0f));
    }
    
    void pushTransform() {
        m_stack.push(m_stack.top());
    }
    
    void popTransform() {
        if (m_stack.size() > 1) {
            m_stack.pop();
        }
    }
    
    void setPosition(const Vector& pos);
    void setScale(float x, float y, float z);
    void setScale(const Vector& scale);
    
    // Methods for modern NGL compatibility
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
    
    void setGlobal(const Transformation& transform) {
        m_globalTransform = transform.getMatrix();
    }
    
    Transformation getCurrentTransform() const {
        Transformation result;
        result.setMatrix(m_stack.top());
        return result;
    }
    
    Transformation getCurrAndGlobal() const {
        Transformation result;
        result.setMatrix(m_stack.top() * m_globalTransform);
        return result;
    }
    
private:
    std::stack<glm::mat4> m_stack;
    glm::mat4 m_globalTransform;
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

} // namespace ngl

#endif // NGL_TRANSFORMSTACK_H
