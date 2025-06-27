#include "ngl_compat/TransformStack.h"
#include "ngl_compat/Vector.h"

namespace ngl {

void TransformStack::setPosition(const Vector& pos) {
    // Apply translation to current transform
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(pos.m_x, pos.m_y, pos.m_z));
    m_stack.top() = translation * m_stack.top();
}

void TransformStack::setScale(float x, float y, float z) {
    // Apply scale to current transform
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
    m_stack.top() = scale * m_stack.top();
}

void TransformStack::setScale(const Vector& scale) {
    setScale(scale.m_x, scale.m_y, scale.m_z);
}

}
