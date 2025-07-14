// TransformBatcher.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "FlockTypes.h"

/**
 * @file TransformBatcher.h
 * @brief Simplified transform batching system for efficient rendering.
 *
 * Modernized for clarity and maintainability. Uses FlockTypes.h for type aliases and helpers.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class TransformBatcher {
public:
    static TransformBatcher* instance();
    
    // Transform batching for similar objects
    struct TransformBatch {
        std::vector<glm::mat4> transforms;
        std::vector<glm::vec4> colors; // Per-instance data
        std::string geometryKey;
        std::string shaderName;
        bool wireframe = false;
    };
    
    // Add objects to batches
    void addToBatch(const std::string& batchKey, 
                   const glm::mat4& transform, 
                   const glm::vec4& color = glm::vec4(1.0f));
    
    // Execute all batches efficiently
    void renderBatches();
    
    // Clear all batches
    void clearBatches();
    
private:
    TransformBatcher() = default;
    static TransformBatcher* s_instance;
    
    std::unordered_map<std::string, TransformBatch> m_batches;
};
