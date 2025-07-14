
/**
 * @file TransformStack.cpp
 * @brief Implementation of a simple transformation stack for hierarchical object transforms.
 *
 * Provides methods to manipulate position and scale using a stack-based approach, useful for scene graph traversal and rendering.
 *
 * @author Dennis Toufexis
 * @date 2025
 */

#include "TransformStack.h"


/**
 * @brief Set the position of the current transform.
 *
 * Replaces the top of the stack with a translation matrix.
 * @param pos The new position as a glm::vec3.
 */
void TransformStack::setPosition(const glm::vec3& pos)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
    if (!m_stack.empty()) {
        m_stack.pop();
        m_stack.push(translation);
    }
}


/**
 * @brief Set the scale of the current transform using individual components.
 *
 * @param x Scale factor along the X axis.
 * @param y Scale factor along the Y axis.
 * @param z Scale factor along the Z axis.
 */
void TransformStack::setScale(float x, float y, float z)
{
    setScale(glm::vec3(x, y, z));
}


/**
 * @brief Set the scale of the current transform using a vector.
 *
 * Multiplies the top of the stack by a scaling matrix.
 * @param scale The scale vector (x, y, z).
 */
void TransformStack::setScale(const glm::vec3& scale)
{
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
    if (!m_stack.empty()) {
        glm::mat4 current = m_stack.top();
        m_stack.pop();
        m_stack.push(current * scaling);
    }
}
