// UBOCache.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <string>
#include "UBOStructures.h"
#include "FlockTypes.h"

/**
 * @file UBOCache.h
 * @brief Smart UBO cache manager for optimal performance.
 *
 * Only updates UBOs when data actually changes, reducing GPU bandwidth usage.
 *
 * @author Dennis Toufexis
 * @date 2025
 */
class UBOCache {
public:
    static UBOCache* instance();
    
    // Matrix UBO with dirty checking
    void setMatrixData(const FlockingShaders::MatrixBlock& matrices);
    bool updateMatrixUBOIfDirty();
    
    // Material UBO with dirty checking  
    void setMaterialData(const FlockingShaders::MaterialBlock& material);
    bool updateMaterialUBOIfDirty();
    
    // Light UBO with dirty checking
    void setLightData(const FlockingShaders::LightingBlock& light);
    bool updateLightUBOIfDirty();
    
    // Force updates (useful for debugging)
    void forceUpdateAll();
    
    // Statistics for optimization tracking
    struct Stats {
        size_t matrixUpdates = 0;
        size_t matrixSkipped = 0;
        size_t materialUpdates = 0;
        size_t materialSkipped = 0;
        size_t lightUpdates = 0;
        size_t lightSkipped = 0;
    };
    
    const Stats& getStats() const { return m_stats; }
    void clearStats() { m_stats = {}; }
    
private:
    UBOCache() = default;
    static UBOCache* s_instance;
    
    // Cached data with dirty flags
    FlockingShaders::MatrixBlock m_cachedMatrix;
    FlockingShaders::MaterialBlock m_cachedMaterial;
    FlockingShaders::LightingBlock m_cachedLight;
    
    bool m_matrixDirty = true;
    bool m_materialDirty = true;
    bool m_lightDirty = true;
    
    Stats m_stats;
    
    // Comparison helpers
    bool matricesEqual(const FlockingShaders::MatrixBlock& a, const FlockingShaders::MatrixBlock& b) const;
    bool materialsEqual(const FlockingShaders::MaterialBlock& a, const FlockingShaders::MaterialBlock& b) const;
    bool lightsEqual(const FlockingShaders::LightingBlock& a, const FlockingShaders::LightingBlock& b) const;
};
