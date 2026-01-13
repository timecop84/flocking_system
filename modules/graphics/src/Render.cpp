// Render.cpp
// Renders a frame for GLWindow: loads matrices/materials into UBOs, draws bbox/boids/obstacle, and tracks FPS.

#include "GLWindow.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "PerformanceProfiler.h"
#include <FrameCoordinator.h>
#include <RenderManager.h>

void GLWindow::loadMatricesToShader(
        TransformStack &_tx
        )
{
    // Use UBO-based approach for modern shader pipeline
    updateMatrixUBO(_tx);
}

void GLWindow::loadMatricesToColourShader(
        TransformStack &_tx
        )

{
    // Use UBO-based approach for modern shader pipeline
    updateMatrixUBO(_tx);
    
    ShaderLib *shader=ShaderLib::instance();
    (*shader)["Colour"]->use();
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::render()
{
    PROFILE_SCOPE("Total Frame Render");
    
    static int frame_count = 0;
    frame_count++;
    
    FlockingGraphics::FrameCoordinator::getInstance().beginFrame();
    
    glClearColor(m_backgroundColour.m_r, m_backgroundColour.m_g, m_backgroundColour.m_b, m_backgroundColour.m_a);
    
    {
        PROFILE_SCOPE("Clear Buffers");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 project = m_cam->getProjectionMatrix();
    
    Matrix nglProject(project);
    Matrix nglView(view);
    m_transformStack.setProjection(nglProject);
    m_transformStack.setView(nglView);
    
    Matrix model;
    model.identity();
    model.translate(m_modelPos.m_x, m_modelPos.m_y, m_modelPos.m_z);
    m_transformStack.setModel(model);
    
    updateLightingUBO();

    if (bbox) {
        PROFILE_SCOPE("Draw Bounding Box");
        m_transformStack.pushTransform();
        {
            Matrix bboxTransform;
            bboxTransform.identity();
            m_transformStack.setModel(bboxTransform.getGLMMat4());
            
            updateMatrixUBO(m_transformStack);
            
            ShaderLib *shader = ShaderLib::instance();
            shader->use("Colour");
            shader->setShaderParam4f("Colour", 1.0f, 1.0f, 1.0f, 1.0f);
            bbox->draw();
        }
        m_transformStack.popTransform();
    }

    if (flock && m_instancedBoidRenderer) {
        m_boidRenderer->render(flock->getBoidList(), m_transformStack, m_cam.get());
    }

    if (obstacle && m_obstacleEnabled) {
        m_obstacleRenderer->render(obstacle.get(), m_transformStack, m_cam.get());
    }
    
    updateFPS();
    
    FlockingGraphics::FrameCoordinator::getInstance().endFrame();
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Initialize UBOs for modern shader pipeline
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeUBOs()
{
    // Create UBOs for matrix, material, and lighting data
    m_shader->createUBO("MatrixUBO", sizeof(FlockingShaders::MatrixBlock));
    m_shader->createUBO("MaterialUBO", sizeof(FlockingShaders::MaterialBlock));
    m_shader->createUBO("LightingUBO", sizeof(FlockingShaders::LightingBlock));
    
    // Bind UBOs to their respective binding points
    m_shader->bindUBOToBindingPoint("MatrixUBO", FlockingShaders::MATRIX_BINDING_POINT);
    m_shader->bindUBOToBindingPoint("MaterialUBO", FlockingShaders::MATERIAL_BINDING_POINT);
    m_shader->bindUBOToBindingPoint("LightingUBO", FlockingShaders::LIGHTING_BINDING_POINT);
    
    // Setup lighting UBO
    setupLightingUBO();
}

void GLWindow::setupLightingUBO() {
    FlockingShaders::LightingBlock lighting;
    
    // Set default light position and color
    lighting.lightPos = glm::vec3(50.0f, 50.0f, 50.0f);
    lighting.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    lighting.shininess = 32.0f;
    lighting.pad1 = lighting.pad2 = 0.0f;  // Required padding for std140 layout
    
    // Create and initialize the UBO
    ShaderLib::instance()->createUBO("LightingUBO", sizeof(FlockingShaders::LightingBlock));
    ShaderLib::instance()->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
    ShaderLib::instance()->bindUBOToBindingPoint("LightingUBO", FlockingShaders::LIGHTING_BINDING_POINT);
}

void GLWindow::updateLightingUBO() {
    FlockingShaders::LightingBlock lighting;
    // In view space, the camera is always at the origin
    lighting.viewPos = glm::vec3(0.0f, 0.0f, 0.0f);
    
    // Transform light position to view space
    // Use the same light source as the obstacle for consistency
    glm::vec3 lightWorldPos = m_light ? glm::vec3(m_light->getPosition()) : glm::vec3(150.0f, 200.0f, 120.0f);
    glm::mat4 viewMatrix = m_cam->getViewMatrix();
    glm::vec4 lightViewPos = viewMatrix * glm::vec4(lightWorldPos, 1.0f);
    lighting.lightPos = glm::vec3(lightViewPos); // Light position in view space
    
    lighting.lightColor = m_light ? glm::vec3(m_light->getColor()) * 0.6f : glm::vec3(0.6f, 0.6f, 0.55f);  // Further reduced intensity for darker, more natural appearance
    lighting.shininess = 32.0f;
    lighting.pad1 = lighting.pad2 = 0.0f;
    
    // Update the UBO
    ShaderLib::instance()->updateUBO("LightingUBO", &lighting, sizeof(FlockingShaders::LightingBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update matrix UBO with current transform data
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateMatrixUBO(const TransformStack& transformStack) {
    FlockingShaders::MatrixBlock matrices;
    
    // Compose matrices for UBO
    glm::mat4 model = transformStack.getCurrentTransform();
    glm::mat4 view = m_cam->getViewMatrix();
    glm::mat4 proj = m_cam->getProjectionMatrix();
    matrices.M = model;
    matrices.MV = view * model;
    matrices.MVP = proj * view * model;
    matrices.normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
    matrices.viewerPos = m_cam->getEye();
    matrices.shouldNormalize = 1.0f;
    ShaderLib::instance()->updateUBO("MatrixUBO", &matrices, sizeof(FlockingShaders::MatrixBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update material UBO with material properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateMaterialUBO(const Material& material)
{
    // Convert NGL Material to UBO format
    Colour ambient = material.getAmbient();
    Colour diffuse = material.getDiffuse();
    Colour specular = material.getSpecular();
    
    m_materialData.ambient = glm::vec4(ambient.m_r, ambient.m_g, ambient.m_b, ambient.m_a);
    m_materialData.diffuse = glm::vec4(diffuse.m_r, diffuse.m_g, diffuse.m_b, diffuse.m_a);
    m_materialData.specular = glm::vec4(specular.m_r, specular.m_g, specular.m_b, specular.m_a);
    m_materialData.shininess = material.getShininess();
    
    // Update the UBO
    m_shader->updateUBO("MaterialUBO", &m_materialData, sizeof(FlockingShaders::MaterialBlock));
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update light UBO with light properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateLightUBO()
{
    if (m_light) {
        glm::vec3 lightPos = m_light->getPosition();
        glm::vec3 lightColor = m_light->getColor();

        m_lightData.lightPos = lightPos;
        m_lightData.viewPos = glm::vec3(0.0f, 0.0f, 0.0f); // view-space origin
        m_lightData.lightColor = lightColor;
        m_lightData.shininess = 32.0f;

        m_shader->updateUBO("LightingUBO", &m_lightData, sizeof(FlockingShaders::LightingBlock));
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Update material UBO with boid-specific material properties
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateBoidMaterialUBO(const Boid& boid)
{
    // Get boid color
    flock::Color boidColor = boid.getColorModern();
    
    // Set up boid material properties similar to immediate mode
    m_materialData.ambient = glm::vec4(boidColor.r * 0.3f, boidColor.g * 0.3f, boidColor.b * 0.3f, 1.0f);
    m_materialData.diffuse = glm::vec4(boidColor.r, boidColor.g, boidColor.b, 1.0f);
    m_materialData.specular = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    m_materialData.shininess = 64.0f;
    
    // Update the UBO
    m_shader->updateUBO("MaterialUBO", &m_materialData, sizeof(FlockingShaders::MaterialBlock));
}

void GLWindow::setBoidMaterial(Material& material, const Boid& boid)
{
    // Get boid color
    flock::Color boidColor = boid.getColorModern();
    
    // Set up boid material properties similar to immediate mode
    material.setAmbient(Colour(boidColor.r * 0.3f, boidColor.g * 0.3f, boidColor.b * 0.3f, 1.0f));
    material.setDiffuse(Colour(boidColor.r, boidColor.g, boidColor.b, 1.0f));
    material.setSpecular(Colour(0.8f, 0.8f, 0.8f, 1.0f));
    material.setShininess(64.0f);
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::updateFPS() {
    m_frameCount++;
    
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastTime);
    
    // Update FPS every 500ms for smooth display
    if (timeDiff.count() >= 500) {
        m_currentFPS = (m_frameCount * 1000.0f) / timeDiff.count();
        m_frameCount = 0;
        m_lastTime = currentTime;
    }
}
