#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_gl = new GLWindow(this);
    m_ui->s_mainWindowGridLayout->addWidget(m_gl, 0, 0 ,2, 1);
    this->setWindowTitle(QString("Swarm Flock"));


}

MainWindow::~MainWindow()
{
    delete m_ui;
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

void MainWindow::on_m_obstaclePosX_valueChanged(double arg1)
{
    ngl::Vector pos;
    pos.set(arg1, m_ui->m_obstaclePosY->value(), m_ui->m_obstaclePosZ->value());

    m_gl->setObstaclePosition(pos);
}

void MainWindow::on_m_obstaclePosY_valueChanged(double arg1)
{
    ngl::Vector pos;
    pos.set(m_ui->m_obstaclePosX->value(), arg1, m_ui->m_obstaclePosZ->value());

    m_gl->setObstaclePosition(pos);
}

void MainWindow::on_m_obstaclePosZ_valueChanged(double arg1)
{
    ngl::Vector pos;
    pos.set(m_ui->m_obstaclePosX->value(), m_ui->m_obstaclePosY->value(), arg1);

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

void MainWindow::on_m_simDistance_valueChanged(double arg1)
{
    m_gl->setSimDistance(arg1);
}

void MainWindow::on_m_simFlockDistance_valueChanged(double arg1)
{
    m_gl->setSimFlockDistance(arg1);
}

void MainWindow::on_m_simCohesion_valueChanged(double arg1)
{
    m_gl->setSimCohesion(arg1);
}

void MainWindow::on_m_simSeparation_valueChanged(double arg1)
{
    m_gl->setSimSeparation(arg1);
}

void MainWindow::on_m_simAlignment_valueChanged(double arg1)
{
    m_gl->setSimAlignment(arg1);
}

void MainWindow::on_m_backColour_clicked()
{
    QColor colour = QColorDialog::getColor();
    ngl::Colour c;
    c.set(colour.redF(), colour.greenF(), colour.blueF());
    m_gl->setBackgroundColour(c);
}

void MainWindow::on_m_bboxSize_valueChanged(double arg1)
{
    ngl::Vector size;
    size.set(arg1, arg1, arg1);

    m_gl->setBBoxSize(size);
}
