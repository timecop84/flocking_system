#include "BoidRenderer.h"
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

    // Add each boid as an instance
    for(const Boid* boid : boids) {
        // Calculate model matrix for this boid
        flock::Vec3 boidPos = boid->getPositionModern();
        Vector boidScale = boid->getScale();
        Matrix boidTransform;
        boidTransform.identity();
        boidTransform.translate(boidPos.x, boidPos.y, boidPos.z);
        boidTransform.scale(boidScale.m_x, boidScale.m_y, boidScale.m_z);
        glm::mat4 modelMatrix = boidTransform.getGLMMat4();

        // Get the actual boid color from the boid object
        flock::Color boidColorModern = boid->getColorModern();
        glm::vec4 boidColor(boidColorModern.r, boidColorModern.g, boidColorModern.b, boidColorModern.a);

        // Set up per-boid material and update the UBO
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

        m_instancedRenderer->addInstance(modelMatrix, boidColor);
    }

    // Render all boids in a single instanced draw call
    m_instancedRenderer->renderInstances("PhongInstanced");
}
