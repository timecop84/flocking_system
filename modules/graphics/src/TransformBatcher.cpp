
/**
 * @file TransformBatcher.cpp
 * @brief Implementation of a batching system for object transforms in the renderer.
 *
 * Collects and manages batches of transformation matrices and colors for efficient instanced rendering.
 * This system is designed to minimize draw calls and improve performance when rendering many similar objects.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */

#include "../include/TransformBatcher.h"
#include <iostream>


// Static singleton instance pointer
TransformBatcher* TransformBatcher::s_instance = nullptr;


/**
 * @brief Get the singleton instance of the TransformBatcher.
 *
 * Ensures only one batcher exists for the lifetime of the application.
 * @return Pointer to the TransformBatcher instance.
 */
TransformBatcher* TransformBatcher::instance() {
    if (!s_instance) {
        s_instance = new TransformBatcher();
    }
    return s_instance;
}


/**
 * @brief Add a transform and color to a named batch.
 *
 * Batches are keyed by string, allowing grouping of similar objects for instanced rendering.
 * @param batchKey The name of the batch.
 * @param transform The transformation matrix to add.
 * @param color The color to associate with the transform.
 */
void TransformBatcher::addToBatch(const std::string& batchKey, 
                                 const glm::mat4& transform, 
                                 const glm::vec4& color) {
    // Simplified stub implementation
    auto& batch = m_batches[batchKey];
    batch.transforms.push_back(transform);
    batch.colors.push_back(color);
}


/**
 * @brief Render all batches.
 *
 * Iterates through all batches and renders them. In a full implementation, this would issue instanced draw calls.
 */
void TransformBatcher::renderBatches() {
    // Simplified stub implementation
    for (auto& pair : m_batches) {
        // TODO: Implement optimized batch rendering
        std::cout << "Rendering batch: " << pair.first << " with " << pair.second.transforms.size() << " instances" << std::endl;
    }
}


/**
 * @brief Clear all batches from the batcher.
 *
 * Removes all stored transforms and colors, preparing for the next frame.
 */
void TransformBatcher::clearBatches() {
    m_batches.clear();
}
