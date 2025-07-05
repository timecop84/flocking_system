#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <glm/glm.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    // Modern dark theme stylesheet
    QString darkStyle = R"(
    QWidget {
        background-color: #232629;
        color: #e0e0e0;
        font-family: 'Segoe UI', 'Roboto', 'Arial', sans-serif;
        font-size: 9pt;
    }
    QMainWindow::separator {
        background: #444;
    }
    QGroupBox {
        border: 1px solid #444;
        border-radius: 8px;
        margin-top: 4px;
        padding: 4px;
        background-color: #282c34;
        font-weight: bold;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        left: 10px;
        padding: 0 4px 0 4px;
        color: #7fd6ff;
    }
    QToolBox, QTabWidget::pane {
        background: #232629;
        border: 1px solid #333;
        border-radius: 8px;
    }
    QPushButton {
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                         stop:0 #3a3f44, stop:0.5 #31363b, stop:1 #232629);
        color: #e0e0e0;
        border: 1px solid #555;
        border-radius: 6px;
        padding: 2px 6px;
        font-weight: 500;
        /* Qt-specific shadow effect for depth */
        /* Use a subtle drop shadow for depth */
        /* Qt doesn't support box-shadow, so use border and gradient for 3D look */
    }
    QPushButton:hover {
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                         stop:0 #4ec3ff, stop:1 #3daee9);
        color: #fff;
        border: 1px solid #3daee9;
    }
    QPushButton:pressed {
        background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                         stop:0 #007acc, stop:1 #005f99);
        border: 1px solid #007acc;
    }
    QLabel {
        color: #e0e0e0;
    }
    QSlider::groove:horizontal {
        border: 1px solid #444;
        height: 6px;
        background: #31363b;
        border-radius: 3px;
    }
    QSlider::handle:horizontal {
        background: #3daee9;
        border: 1px solid #007acc;
        width: 16px;
        margin: -5px 0;
        border-radius: 8px;
    }
    QSpinBox, QDoubleSpinBox {
        background: #232629;
        color: #e0e0e0;
        border: 1px solid #555;
        border-radius: 4px;
        padding: 2px 6px;
    }
    QCheckBox {
        spacing: 8px;
    }
    QCheckBox::indicator {
        width: 18px;
        height: 18px;
        border-radius: 4px;
        border: 1px solid #3daee9;
        background: #232629;
    }
    QCheckBox::indicator:checked {
        background: #3daee9;
        border: 1px solid #3daee9;
    }
    QMenuBar, QMenu, QStatusBar {
        background: #232629;
        color: #e0e0e0;
    }
    QMenu::item:selected {
        background: #3daee9;
        color: #fff;
    }
    )";
    this->setStyleSheet(darkStyle);

    m_gl = new GLWindow(this);
    m_ui->s_mainWindowGridLayout->addWidget(m_gl, 0, 0 ,2, 1);
    this->setWindowTitle(QString("Swarm Flock"));

    // Note: UI initialization moved to after OpenGL initialization
    // This will be done via timer or signal to ensure proper order
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::initializeUIValues()
{
    // Initialize UI controls with their default values
    // This ensures the objects match the UI control values at startup
    m_gl->setBoidSize(m_ui->m_changeBoidSize->value());
    m_gl->setObstacleSize(m_ui->m_obstacleSize->value());
}

void MainWindow::on_m_flockDensity_valueChanged(int arg1)
{
    if (arg1 != m_gl->getCurrentBoidSize() && arg1 != m_gl->getCurrentBoidSize())
        m_ui->m_applyFlock->setEnabled(true);
    else
        m_ui->m_applyFlock->setEnabled(false);

    if (m_gl->getCurrentBoidSize() > 1990)
        m_ui->m_addBoids->setEnabled(false);
    else
        m_ui->m_addBoids->setEnabled(true);

    if (m_gl->getCurrentBoidSize() < 10)
        m_ui->m_removeBoids->setEnabled(false);
    else
        m_ui->m_removeBoids->setEnabled(true);
}

void MainWindow::on_m_resetFlock_clicked()
{
    m_ui->m_flockDensity->setValue(200);
    m_gl->resetFlock();
    m_ui->m_applyFlock->setEnabled(false);
}

void MainWindow::on_m_applyFlock_clicked()
{
    m_gl->applyFlock(m_ui->m_flockDensity->value());
    m_ui->m_applyFlock->setEnabled(false);

    if (m_gl->getCurrentBoidSize() > 1990)
        m_ui->m_addBoids->setEnabled(false);
    else
        m_ui->m_addBoids->setEnabled(true);

    if (m_gl->getCurrentBoidSize() < 30)
        m_ui->m_removeBoids->setEnabled(false);
    else
        m_ui->m_removeBoids->setEnabled(true);
}

void MainWindow::on_m_addBoids_clicked()
{
    if (m_gl->getCurrentBoidSize() <= 1990)
    {
        m_ui->m_flockDensity->setValue(m_ui->m_flockDensity->value() + 10);
        m_gl->addBoidsToFlock();
        m_ui->m_applyFlock->setEnabled(false);
    }
}

void MainWindow::on_m_removeBoids_clicked()
{
    if (m_gl->getCurrentBoidSize() > 30)
    {
        m_ui->m_flockDensity->setValue(m_ui->m_flockDensity->value() - 10);
        m_gl->removeBoidsFromFlock();
        m_ui->m_applyFlock->setEnabled(false);
    }
}

void MainWindow::on_m_changeBoidSize_valueChanged(double arg1)
{
    m_gl->setBoidSize(arg1);
}

void MainWindow::on_m_changeFlockColour_clicked()
{
    QColor colour = QColorDialog::getColor();
    m_gl->setBoidColor(colour);
}

void MainWindow::on_m_renderFlockWireframe_toggled(bool checked)
{
    m_gl->setFlockWireframe(checked);
}

void MainWindow::on_m_flockSpeedSlider_valueChanged(int value)
{
    // Update the label to show the percentage
    m_ui->m_flockSpeedLabel->setText(QString("%1%").arg(value));
    
    // Convert slider value (10-300) to a speed multiplier (0.1-3.0)
    float speedMultiplier = value / 100.0f;
    
    // Pass the speed multiplier to the GL window
    m_gl->setFlockSpeedMultiplier(speedMultiplier);
}

void MainWindow::on_m_obstaclePosX_valueChanged(double arg1)
{
    glm::vec3 pos(arg1, m_ui->m_obstaclePosY->value(), m_ui->m_obstaclePosZ->value());
    m_gl->setObstaclePosition(pos);
}

void MainWindow::on_m_obstaclePosY_valueChanged(double arg1)
{
    glm::vec3 pos(m_ui->m_obstaclePosX->value(), arg1, m_ui->m_obstaclePosZ->value());
    m_gl->setObstaclePosition(pos);
}

void MainWindow::on_m_obstaclePosZ_valueChanged(double arg1)
{
    glm::vec3 pos(m_ui->m_obstaclePosX->value(), m_ui->m_obstaclePosY->value(), arg1);
    m_gl->setObstaclePosition(pos);
}

void MainWindow::on_m_obstacleSize_valueChanged(double arg1)
{
    m_gl->setObstacleSize(arg1);
}

void MainWindow::on_m_obstacleColour_clicked()
{
    QColor colour = QColorDialog::getColor();
    m_gl->setObstacleColour(colour);
}

void MainWindow::on_m_obstacleWireframe_toggled(bool checked)
{
    m_gl->setObstacleWireframe(checked);
}

void MainWindow::on_m_simDistanceSlider_valueChanged(int value)
{
    double simDistance = static_cast<double>(value);
    m_ui->m_simDistanceValue->setText(QString::number(simDistance, 'f', 1));
    m_gl->setSimDistance(simDistance);
}

void MainWindow::on_m_simFlockDistanceSlider_valueChanged(int value)
{
    double flockDistance = static_cast<double>(value);
    m_ui->m_simFlockDistanceValue->setText(QString::number(flockDistance, 'f', 1));
    m_gl->setSimFlockDistance(flockDistance);
}

void MainWindow::on_m_simCohesionSlider_valueChanged(int value)
{
    double cohesion = static_cast<double>(value);
    m_ui->m_simCohesionValue->setText(QString::number(cohesion, 'f', 1));
    m_gl->setSimCohesion(cohesion);
}

void MainWindow::on_m_simSeparationSlider_valueChanged(int value)
{
    double separation = static_cast<double>(value);
    m_ui->m_simSeparationValue->setText(QString::number(separation, 'f', 1));
    m_gl->setSimSeparation(separation);
}

void MainWindow::on_m_simAlignmentSlider_valueChanged(int value)
{
    double alignment = static_cast<double>(value);
    m_ui->m_simAlignmentValue->setText(QString::number(alignment, 'f', 1));
    m_gl->setSimAlignment(alignment);
}

void MainWindow::on_m_backColour_clicked()
{
    QColor colour = QColorDialog::getColor();
    Colour c;
    c.set(colour.redF(), colour.greenF(), colour.blueF());
    m_gl->setBackgroundColour(c);
}

void MainWindow::on_m_bboxSize_valueChanged(double arg1)
{
    glm::vec3 size(arg1, arg1, arg1);
    m_gl->setBBoxSize(size);
}

void MainWindow::on_m_obstacleSpecularR_valueChanged(double arg1)
{
    double g = m_ui->m_obstacleSpecularG->value();
    double b = m_ui->m_obstacleSpecularB->value();
    m_gl->setObstacleSpecular(arg1, g, b);
}

void MainWindow::on_m_obstacleSpecularG_valueChanged(double arg1)
{
    double r = m_ui->m_obstacleSpecularR->value();
    double b = m_ui->m_obstacleSpecularB->value();
    m_gl->setObstacleSpecular(r, arg1, b);
}

void MainWindow::on_m_obstacleSpecularB_valueChanged(double arg1)
{
    double r = m_ui->m_obstacleSpecularR->value();
    double g = m_ui->m_obstacleSpecularG->value();
    m_gl->setObstacleSpecular(r, g, arg1);
}

void MainWindow::on_m_obstacleDiffuseR_valueChanged(double arg1)
{
    double g = m_ui->m_obstacleDiffuseG->value();
    double b = m_ui->m_obstacleDiffuseB->value();
    m_gl->setObstacleDiffuse(arg1, g, b);
}

void MainWindow::on_m_obstacleDiffuseG_valueChanged(double arg1)
{
    double r = m_ui->m_obstacleDiffuseR->value();
    double b = m_ui->m_obstacleDiffuseB->value();
    m_gl->setObstacleDiffuse(r, arg1, b);
}

void MainWindow::on_m_obstacleDiffuseB_valueChanged(double arg1)
{
    double r = m_ui->m_obstacleDiffuseR->value();
    double g = m_ui->m_obstacleDiffuseG->value();
    m_gl->setObstacleDiffuse(r, g, arg1);
}

void MainWindow::on_m_enableObstacle_toggled(bool checked)
{
    m_gl->setObstacleEnabled(checked);
}

void MainWindow::on_m_obstacleAvoidanceRadiusSlider_valueChanged(int value)
{
    float scale = value / 10.0f; // Slider 10-50 -> 1.0-5.0
    m_ui->m_obstacleAvoidanceRadiusValue->setText(QString::number(scale, 'f', 2));
    m_gl->setObstacleAvoidanceRadiusScale(scale);
}

void MainWindow::on_m_obstacleCollisionRadiusSlider_valueChanged(int value)
{
    float scale = value / 10.0f; // Slider 10-30 -> 1.0-3.0
    m_ui->m_obstacleCollisionRadiusValue->setText(QString::number(scale, 'f', 2));
    m_gl->setObstacleCollisionRadiusScale(scale);
}

void MainWindow::on_m_obstacleRepulsionForceSlider_valueChanged(int value)
{
    float force = value / 100.0f; // Slider 10-100 -> 0.10-1.00
    m_ui->m_obstacleRepulsionForceValue->setText(QString::number(force, 'f', 2));
    m_gl->setObstacleRepulsionForce(force);
}
