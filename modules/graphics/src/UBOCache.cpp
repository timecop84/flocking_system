
/**
 * @file UBOCache.cpp
 * @brief Implementation of a caching system for Uniform Buffer Objects (UBOs) in the flocking simulation renderer.
 *
 * This file provides logic to efficiently update OpenGL UBOs for matrices, materials, and lights, minimizing redundant GPU uploads.
 * The cache tracks changes and only updates the GPU when data has actually changed, improving rendering performance.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */

#include "../include/UBOCache.h"
#include "../include/ShaderLib.h"
#include <iostream>
#include <cstring>

UBOCache* UBOCache::s_instance = nullptr;


/**
 * @brief Get the singleton instance of the UBOCache.
 *
 * Ensures only one cache exists for the lifetime of the application.
 * @return Pointer to the UBOCache instance.
 */
UBOCache* UBOCache::instance() {
    if (!s_instance) {
        s_instance = new UBOCache();
    }
    return s_instance;
}


/**
 * @brief Set the matrix data for the UBO cache.
 *
 * Marks the matrix UBO as dirty if the data has changed.
 * @param matrices The new matrix block to cache.
 */
void UBOCache::setMatrixData(const FlockingShaders::MatrixBlock& matrices) {
    if (!matricesEqual(m_cachedMatrix, matrices)) {
        m_cachedMatrix = matrices;
        m_matrixDirty = true;
    }
}


/**
 * @brief Update the matrix UBO on the GPU if the cached data is dirty.
 *
 * Only performs the update if the data has changed since the last upload.
 * @return True if the UBO was updated, false otherwise.
 */
bool UBOCache::updateMatrixUBOIfDirty() {
    if (!m_matrixDirty) {
        m_stats.matrixSkipped++;
        return false;
    }
    // TODO: Actually update OpenGL UBO when we have proper UBO management
    // For now, just mark as clean
    m_matrixDirty = false;
    m_stats.matrixUpdates++;
    return true;
}


/**
 * @brief Set the material data for the UBO cache.
 *
 * Marks the material UBO as dirty if the data has changed.
 * @param material The new material block to cache.
 */
void UBOCache::setMaterialData(const FlockingShaders::MaterialBlock& material) {
    if (!materialsEqual(m_cachedMaterial, material)) {
        m_cachedMaterial = material;
        m_materialDirty = true;
    }
}


/**
 * @brief Update the material UBO on the GPU if the cached data is dirty.
 *
 * Only performs the update if the data has changed since the last upload.
 * @return True if the UBO was updated, false otherwise.
 */
bool UBOCache::updateMaterialUBOIfDirty() {
    if (!m_materialDirty) {
        m_stats.materialSkipped++;
        return false;
    }
    // TODO: Actually update OpenGL UBO when we have proper UBO management
    // For now, just mark as clean
    m_materialDirty = false;
    m_stats.materialUpdates++;
    return true;
}


/**
 * @brief Set the light data for the UBO cache.
 *
 * Marks the light UBO as dirty if the data has changed.
 * @param light The new light block to cache.
 */
void UBOCache::setLightData(const FlockingShaders::LightBlock& light) {
    if (!lightsEqual(m_cachedLight, light)) {
        m_cachedLight = light;
        m_lightDirty = true;
    }
}

bool UBOCache::updateLightUBOIfDirty() {
    if (!m_lightDirty) {
        m_stats.lightSkipped++;
        return false;
    }
    
    // TODO: Actually update OpenGL UBO when we have proper UBO management
    // For now, just mark as clean
    m_lightDirty = false;
    m_stats.lightUpdates++;
    return true;
}

void UBOCache::forceUpdateAll() {
    m_matrixDirty = true;
    m_materialDirty = true;
    m_lightDirty = true;
    
    updateMatrixUBOIfDirty();
    updateMaterialUBOIfDirty();
    updateLightUBOIfDirty();
}

bool UBOCache::matricesEqual(const FlockingShaders::MatrixBlock& a, const FlockingShaders::MatrixBlock& b) const {
    return a.MVP == b.MVP &&
           a.MV == b.MV &&
           a.M == b.M &&
           a.normalMatrix == b.normalMatrix &&
           a.viewerPos == b.viewerPos &&
           a.shouldNormalize == b.shouldNormalize;
}

bool UBOCache::materialsEqual(const FlockingShaders::MaterialBlock& a, const FlockingShaders::MaterialBlock& b) const {
    return a.ambient == b.ambient &&
           a.diffuse == b.diffuse &&
           a.specular == b.specular &&
           a.shininess == b.shininess;
}

bool UBOCache::lightsEqual(const FlockingShaders::LightBlock& a, const FlockingShaders::LightBlock& b) const {
    return a.position == b.position &&
           a.ambient == b.ambient &&
           a.diffuse == b.diffuse &&
           a.specular == b.specular &&
           a.constantAttenuation == b.constantAttenuation &&
           a.linearAttenuation == b.linearAttenuation &&
           a.quadraticAttenuation == b.quadraticAttenuation &&
           a.spotCosCutoff == b.spotCosCutoff;
}
