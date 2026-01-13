
// UBOCache.cpp
// Minimal UBO cache: tracks matrix/material/lighting blocks and only pushes them to GPU when the data changes.

#include "../include/UBOCache.h"
#include "../include/ShaderLib.h"
#include <iostream>
#include <cstring>

UBOCache* UBOCache::s_instance = nullptr;


UBOCache* UBOCache::instance() {
    if (!s_instance) {
        s_instance = new UBOCache();
    }
    return s_instance;
}


void UBOCache::setMatrixData(const FlockingShaders::MatrixBlock& matrices) {
    if (!matricesEqual(m_cachedMatrix, matrices)) {
        m_cachedMatrix = matrices;
        m_matrixDirty = true;
    }
}


bool UBOCache::updateMatrixUBOIfDirty() {
    if (!m_matrixDirty) {
        m_stats.matrixSkipped++;
        return false;
    }
    ShaderLib::instance()->updateUBO("MatrixUBO", &m_cachedMatrix, sizeof(FlockingShaders::MatrixBlock));
    m_matrixDirty = false;
    m_stats.matrixUpdates++;
    return true;
}


void UBOCache::setMaterialData(const FlockingShaders::MaterialBlock& material) {
    if (!materialsEqual(m_cachedMaterial, material)) {
        m_cachedMaterial = material;
        m_materialDirty = true;
    }
}


bool UBOCache::updateMaterialUBOIfDirty() {
    if (!m_materialDirty) {
        m_stats.materialSkipped++;
        return false;
    }
    ShaderLib::instance()->updateUBO("MaterialUBO", &m_cachedMaterial, sizeof(FlockingShaders::MaterialBlock));
    m_materialDirty = false;
    m_stats.materialUpdates++;
    return true;
}


void UBOCache::setLightData(const FlockingShaders::LightingBlock& light) {
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
    
    ShaderLib::instance()->updateUBO("LightingUBO", &m_cachedLight, sizeof(FlockingShaders::LightingBlock));
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

bool UBOCache::lightsEqual(const FlockingShaders::LightingBlock& a, const FlockingShaders::LightingBlock& b) const {
    return a.lightPos == b.lightPos &&
           a.viewPos == b.viewPos &&
           a.lightColor == b.lightColor &&
           a.shininess == b.shininess;
}
