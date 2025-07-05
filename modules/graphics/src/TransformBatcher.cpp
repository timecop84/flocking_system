#include "../include/TransformBatcher.h"
#include <iostream>

TransformBatcher* TransformBatcher::s_instance = nullptr;

TransformBatcher* TransformBatcher::instance() {
    if (!s_instance) {
        s_instance = new TransformBatcher();
    }
    return s_instance;
}

void TransformBatcher::addToBatch(const std::string& batchKey, 
                                 const glm::mat4& transform, 
                                 const glm::vec4& color) {
    // Simplified stub implementation
    auto& batch = m_batches[batchKey];
    batch.transforms.push_back(transform);
    batch.colors.push_back(color);
}

void TransformBatcher::renderBatches() {
    // Simplified stub implementation
    for (auto& pair : m_batches) {
        // TODO: Implement optimized batch rendering
        std::cout << "Rendering batch: " << pair.first << " with " << pair.second.transforms.size() << " instances" << std::endl;
    }
}

void TransformBatcher::clearBatches() {
    m_batches.clear();
}
