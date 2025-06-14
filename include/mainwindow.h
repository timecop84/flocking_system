#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GLWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_m_flockDensity_valueChanged(int arg1);

    void on_m_resetFlock_clicked();

    void on_m_applyFlock_clicked();

    void on_m_addBoids_clicked();

    void on_m_removeBoids_clicked();

    void on_m_changeBoidSize_valueChanged(double arg1);

    void on_m_changeFlockColour_clicked();

    void on_m_renderFlockWireframe_toggled(bool checked);

    void on_m_obstaclePosX_valueChanged(double arg1);

    void on_m_obstaclePosY_valueChanged(double arg1);

    void on_m_obstaclePosZ_valueChanged(double arg1);

    void on_m_obstacleSize_valueChanged(double arg1);

    void on_m_obstacleColour_clicked();

    void on_m_obstacleWireframe_toggled(bool checked);

    void on_m_simDistance_valueChanged(double arg1);

    void on_m_simFlockDistance_valueChanged(double arg1);

    void on_m_simCohesion_valueChanged(double arg1);

    void on_m_simSeparation_valueChanged(double arg1);

    void on_m_simAlignment_valueChanged(double arg1);

    void on_m_backColour_clicked();

    void on_m_bboxSize_valueChanged(double arg1);

private:
    Ui::MainWindow *m_ui;

    /// @brief our openGL widget
    GLWindow *m_gl;
};

#endif // MAINWINDOW_H
