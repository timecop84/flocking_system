#include "BoidRenderer.h"
#ifdef _OPENMP
#include <omp.h>
#endif
#include <ShaderLib.h>
#include <Material.h>
#include <modules/graphics/include/InstancedBoidRenderer.h>
#include <modules/graphics/include/UBOStructures.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

BoidRenderer::BoidRenderer() {
    // Constructor: initialize members if needed
}

BoidRenderer::~BoidRenderer() {
    // Destructor: cleanup resources if needed
}

void BoidRenderer::setInstancedBoidRenderer(FlockingGraphics::InstancedBoidRenderer* renderer) {
    m_instancedRenderer = renderer;
}

void BoidRenderer::initialize() {
    // TODO: Initialize shaders, VAOs, VBOs for boid rendering if needed
    std::cout << "BoidRenderer initialized." << std::endl;
}

void BoidRenderer::render(const std::vector<Boid*>& boids, const TransformStack& transform, Camera* cam) {
    if (!m_instancedRenderer || boids.empty()) return;

    // Clear previous instances
    m_instancedRenderer->clearInstances();

    // Set up material and update the UBO ONCE (assuming all boids share the same material)
    flock::Color boidColorModern = boids[0]->getColorModern();
    Material boidMaterial;
    boidMaterial.setAmbient(Colour(boidColorModern.r * 0.3f, boidColorModern.g * 0.3f, boidColorModern.b * 0.3f, 1.0f));
    boidMaterial.setDiffuse(Colour(boidColorModern.r, boidColorModern.g, boidColorModern.b, 1.0f));
    boidMaterial.setSpecular(Colour(0.8f, 0.8f, 0.8f, 1.0f));
    boidMaterial.setShininess(64.0f);
    FlockingShaders::MaterialBlock block;
    Colour ambient = boidMaterial.getAmbient();
    Colour diffuse = boidMaterial.getDiffuse();
    Colour specular = boidMaterial.getSpecular();
    block.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    block.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    block.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    block.shininess = boidMaterial.getShininess();
    ShaderLib::instance()->updateUBO("MaterialUBO", &block, sizeof(FlockingShaders::MaterialBlock));

    // Prepare storage for model matrices and colors
    std::vector<glm::mat4> modelMatrices(boids.size());
    std::vector<glm::vec4> boidColors(boids.size());

    // Parallelize the calculation of model matrices and colors
    int numThreads = 1;
    #ifdef _OPENMP
    numThreads = omp_get_max_threads();
    std::cout << "[BoidRenderer] OpenMP parallel for using " << numThreads << " threads." << std::endl;
    #else
    std::cout << "[BoidRenderer] OpenMP not enabled, running single-threaded." << std::endl;
    #endif
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(boids.size()); ++i) {
        const Boid* boid = boids[i];
        flock::Vec3 boidPos = boid->getPositionModern();
        Vector boidScale = boid->getScale();
        Matrix boidTransform;
        boidTransform.identity();
        boidTransform.translate(boidPos.x, boidPos.y, boidPos.z);
        boidTransform.scale(boidScale.m_x, boidScale.m_y, boidScale.m_z);
        modelMatrices[i] = boidTransform.getGLMMat4();

        flock::Color boidColorModern = boid->getColorModern();
        boidColors[i] = glm::vec4(boidColorModern.r, boidColorModern.g, boidColorModern.b, boidColorModern.a);
    }

    // Add each instance (single-threaded, as addInstance may not be thread-safe)
    for (size_t i = 0; i < boids.size(); ++i) {
        m_instancedRenderer->addInstance(modelMatrices[i], boidColors[i]);
    }

    // Render all boids in a single instanced draw call
    m_instancedRenderer->renderInstances("PhongInstanced");
}
