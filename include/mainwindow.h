// mainwindow.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <QMainWindow>

#include "GLWindow.h"

namespace Ui {
class MainWindow;
}

/**
 * @file mainwindow.h
 * @brief Main application window for the flocking simulation UI.
 *
 * Provides the main Qt window, connects UI controls to simulation logic, and manages the OpenGL widget.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Construct the main window.
     * @param parent Parent widget (default nullptr)
     */
    explicit MainWindow(QWidget* parent = nullptr);
    /**
     * @brief Destructor.
     */
    ~MainWindow();

private slots:
    /// @brief Update flock density when the value changes.
    void on_m_flockDensity_valueChanged(int arg1);
    /// @brief Reset the flock to its initial state.
    void on_m_resetFlock_clicked();
    /// @brief Apply flocking parameters from the UI.
    void on_m_applyFlock_clicked();
    /// @brief Add boids to the simulation.
    void on_m_addBoids_clicked();
    /// @brief Remove boids from the simulation.
    void on_m_removeBoids_clicked();
    /// @brief Change the size of boids.
    void on_m_changeBoidSize_valueChanged(double arg1);
    /// @brief Change the flock color.
    void on_m_changeFlockColour_clicked();
    /// @brief Toggle wireframe rendering for the flock.
    void on_m_renderFlockWireframe_toggled(bool checked);
    /// @brief Update flock speed from slider.
    void on_m_flockSpeedSlider_valueChanged(int value);
    /// @brief Update obstacle X position.
    void on_m_obstaclePosX_valueChanged(double arg1);
    /// @brief Update obstacle Y position.
    void on_m_obstaclePosY_valueChanged(double arg1);
    /// @brief Update obstacle Z position.
    void on_m_obstaclePosZ_valueChanged(double arg1);
    /// @brief Update obstacle size.
    void on_m_obstacleSize_valueChanged(double arg1);
    /// @brief Change obstacle color.
    void on_m_obstacleColour_clicked();
    /// @brief Toggle wireframe rendering for the obstacle.
    void on_m_obstacleWireframe_toggled(bool checked);
    /// @brief Update simulation distance parameter.
    void on_m_simDistanceSlider_valueChanged(int value);
    /// @brief Update flock distance parameter.
    void on_m_simFlockDistanceSlider_valueChanged(int value);
    /// @brief Update cohesion parameter.
    void on_m_simCohesionSlider_valueChanged(int value);
    /// @brief Update separation parameter.
    void on_m_simSeparationSlider_valueChanged(int value);
    /// @brief Update alignment parameter.
    void on_m_simAlignmentSlider_valueChanged(int value);
    /// @brief Change background color.
    void on_m_backColour_clicked();
    /// @brief Update bounding box size.
    void on_m_bboxSize_valueChanged(double arg1);
    /// @brief Update obstacle specular R value.
    void on_m_obstacleSpecularR_valueChanged(double arg1);
    /// @brief Update obstacle specular G value.
    void on_m_obstacleSpecularG_valueChanged(double arg1);
    /// @brief Update obstacle specular B value.
    void on_m_obstacleSpecularB_valueChanged(double arg1);
    /// @brief Update obstacle diffuse R value.
    void on_m_obstacleDiffuseR_valueChanged(double arg1);
    /// @brief Update obstacle diffuse G value.
    void on_m_obstacleDiffuseG_valueChanged(double arg1);
    /// @brief Update obstacle diffuse B value.
    void on_m_obstacleDiffuseB_valueChanged(double arg1);
    /// @brief Enable or disable the obstacle.
    void on_m_enableObstacle_toggled(bool checked);
    /// @brief Update obstacle avoidance radius.
    void on_m_obstacleAvoidanceRadiusSlider_valueChanged(int value);
    /// @brief Update obstacle collision radius.
    void on_m_obstacleCollisionRadiusSlider_valueChanged(int value);
    /// @brief Update obstacle repulsion force.
    void on_m_obstacleRepulsionForceSlider_valueChanged(int value);

public:
    /// @brief Initialize UI control values (called after OpenGL initialization)
    void initializeUIValues();

private:
    Ui::MainWindow *m_ui; ///< Pointer to the UI object
    GLWindow *m_gl;       ///< Pointer to the OpenGL widget
};
