#include "ObstacleRenderer.h"
#include <ShaderLib.h>
#include <Material.h>
#include <UBOStructures.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ObstacleRenderer::ObstacleRenderer() {}
ObstacleRenderer::~ObstacleRenderer() {}

void ObstacleRenderer::render(const Obstacle* obstacle, TransformStack& transform, Camera* cam) {
    if (!obstacle) {
        std::cout << "[ObstacleRenderer] render: obstacle is null" << std::endl;
        return;
    }

    std::cout << "[ObstacleRenderer] render: obstacle found, rendering..." << std::endl;

    // Push a new transform level for the obstacle
    transform.pushTransform();
    {
        // Use obstacle's own position and radius
        flock::Vec3 obstaclePos = obstacle->getPositionModern();
        float obstacleRadius = obstacle->getSphereRadius();
        std::cout << "[ObstacleRenderer] render: pos=" << obstaclePos.x << ", " << obstaclePos.y << ", " << obstaclePos.z << ", radius=" << obstacleRadius << std::endl;
        
        Matrix obstacleTransform;
        obstacleTransform.identity();
        obstacleTransform.translate(obstaclePos.x, obstaclePos.y, obstaclePos.z);
        obstacleTransform.scale(obstacleRadius, obstacleRadius, obstacleRadius);
        transform.setModel(obstacleTransform.getGLMMat4());

        // Use obstacle's own color
        flock::Color color = obstacle->getColorModern();
        std::cout << "[ObstacleRenderer] render: color=" << color.r << ", " << color.g << ", " << color.b << std::endl;
        
        Material obstacleMaterial;
        obstacleMaterial.setAmbient(Colour(0.25f, 0.20f, 0.15f, 1.0f));
        obstacleMaterial.setDiffuse(Colour(color.r * 1.8f, color.g * 1.8f, color.b * 1.8f, 1.0f));
        obstacleMaterial.setSpecular(Colour(1.2f, 1.2f, 1.0f, 1.0f));
        obstacleMaterial.setShininess(48.0f);
        obstacleMaterial.loadToShader();

        // Use the Phong shader for obstacle rendering
        ShaderLib::instance()->use("Phong");

        // Update UBO with current transform (this was missing!)
        // Compose matrices for UBO
        glm::mat4 model = transform.getCurrentTransform();
        glm::mat4 view = cam->getViewMatrix();
        glm::mat4 proj = cam->getProjectionMatrix();
        
        FlockingShaders::MatrixBlock matrices;
        matrices.M = model;
        matrices.MV = view * model;
        matrices.MVP = proj * view * model;
        matrices.normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
        matrices.viewerPos = cam->getEye();
        matrices.shouldNormalize = 1.0f;
        
        ShaderLib::instance()->updateUBO("MatrixUBO", &matrices, sizeof(FlockingShaders::MatrixBlock));

        // Render the obstacle using modern VBO/VAO approach
        obstacle->ObsDraw("Phong", transform, cam);
    }
    transform.popTransform();
}
