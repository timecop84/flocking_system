#include "TransformStack.h"

void TransformStack::setPosition(const glm::vec3& pos)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
    if (!m_stack.empty()) {
        m_stack.pop();
        m_stack.push(translation);
    }
}

void TransformStack::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}

void TransformStack::setScale(const glm::vec3& scale)
{
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
    if (!m_stack.empty()) {
        glm::mat4 current = m_stack.top();
        m_stack.pop();
        m_stack.push(current * scaling);
    }
}
