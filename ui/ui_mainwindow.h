/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jul 10 15:57:54 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *s_mainWindowGridLayout;
    QSpacerItem *horizontalSpacer;
    QToolBox *toolBox;
    QWidget *m_page1_flock;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *m_flockDensity;
    QPushButton *m_applyFlock;
    QPushButton *m_resetFlock;
    QPushButton *m_addBoids;
    QPushButton *m_removeBoids;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *m_changeFlockColour;
    QLabel *label_2;
    QDoubleSpinBox *m_changeBoidSize;
    QCheckBox *m_renderFlockWireframe;
    QSpacerItem *verticalSpacer;
    QWidget *m_page2_obstacle;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QDoubleSpinBox *m_obstaclePosX;
    QDoubleSpinBox *m_obstaclePosY;
    QDoubleSpinBox *m_obstaclePosZ;
    QLabel *label_3;
    QDoubleSpinBox *m_obstacleSize;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_4;
    QPushButton *m_obstacleColour;
    QCheckBox *m_obstacleWireframe;
    QSpacerItem *verticalSpacer_2;
    QWidget *m_page3_simulation;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_7;
    QLabel *label_5;
    QDoubleSpinBox *m_simDistance;
    QLabel *label_7;
    QDoubleSpinBox *m_simCohesion;
    QLabel *label_8;
    QDoubleSpinBox *m_simSeparation;
    QLabel *label_9;
    QDoubleSpinBox *m_simAlignment;
    QLabel *label_10;
    QDoubleSpinBox *m_simFlockDistance;
    QWidget *m_page4_environment;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QLabel *label_4;
    QDoubleSpinBox *m_bboxSize;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_6;
    QPushButton *m_backColour;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName(QString::fromUtf8("s_mainWindowGridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setMaximumSize(QSize(310, 16777215));
        toolBox->setFrameShape(QFrame::Box);
        toolBox->setFrameShadow(QFrame::Sunken);
        m_page1_flock = new QWidget();
        m_page1_flock->setObjectName(QString::fromUtf8("m_page1_flock"));
        m_page1_flock->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout = new QVBoxLayout(m_page1_flock);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(m_page1_flock);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        m_flockDensity = new QSpinBox(groupBox);
        m_flockDensity->setObjectName(QString::fromUtf8("m_flockDensity"));
        m_flockDensity->setMinimum(20);
        m_flockDensity->setMaximum(2000);
        m_flockDensity->setValue(200);

        gridLayout->addWidget(m_flockDensity, 0, 2, 1, 1);

        m_applyFlock = new QPushButton(groupBox);
        m_applyFlock->setObjectName(QString::fromUtf8("m_applyFlock"));
        m_applyFlock->setEnabled(false);
        m_applyFlock->setFlat(false);

        gridLayout->addWidget(m_applyFlock, 1, 2, 1, 1);

        m_resetFlock = new QPushButton(groupBox);
        m_resetFlock->setObjectName(QString::fromUtf8("m_resetFlock"));

        gridLayout->addWidget(m_resetFlock, 1, 1, 1, 1);

        m_addBoids = new QPushButton(groupBox);
        m_addBoids->setObjectName(QString::fromUtf8("m_addBoids"));

        gridLayout->addWidget(m_addBoids, 3, 1, 1, 1);

        m_removeBoids = new QPushButton(groupBox);
        m_removeBoids->setObjectName(QString::fromUtf8("m_removeBoids"));

        gridLayout->addWidget(m_removeBoids, 3, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(m_page1_flock);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_changeFlockColour = new QPushButton(groupBox_2);
        m_changeFlockColour->setObjectName(QString::fromUtf8("m_changeFlockColour"));

        gridLayout_2->addWidget(m_changeFlockColour, 0, 0, 1, 2);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 5, 0, 1, 1);

        m_changeBoidSize = new QDoubleSpinBox(groupBox_2);
        m_changeBoidSize->setObjectName(QString::fromUtf8("m_changeBoidSize"));
        m_changeBoidSize->setMinimum(0.5);
        m_changeBoidSize->setMaximum(10);
        m_changeBoidSize->setSingleStep(0.1);
        m_changeBoidSize->setValue(1);

        gridLayout_2->addWidget(m_changeBoidSize, 5, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        m_renderFlockWireframe = new QCheckBox(m_page1_flock);
        m_renderFlockWireframe->setObjectName(QString::fromUtf8("m_renderFlockWireframe"));

        verticalLayout->addWidget(m_renderFlockWireframe);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        toolBox->addItem(m_page1_flock, QString::fromUtf8("Flock"));
        m_page2_obstacle = new QWidget();
        m_page2_obstacle->setObjectName(QString::fromUtf8("m_page2_obstacle"));
        m_page2_obstacle->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_2 = new QVBoxLayout(m_page2_obstacle);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_3 = new QGroupBox(m_page2_obstacle);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 1, 1, 1);

        m_obstaclePosX = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosX->setObjectName(QString::fromUtf8("m_obstaclePosX"));
        m_obstaclePosX->setMinimum(-99);
        m_obstaclePosX->setValue(12);

        gridLayout_3->addWidget(m_obstaclePosX, 1, 1, 1, 1);

        m_obstaclePosY = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosY->setObjectName(QString::fromUtf8("m_obstaclePosY"));
        m_obstaclePosY->setMinimum(-99);
        m_obstaclePosY->setValue(30);

        gridLayout_3->addWidget(m_obstaclePosY, 1, 2, 1, 1);

        m_obstaclePosZ = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosZ->setObjectName(QString::fromUtf8("m_obstaclePosZ"));
        m_obstaclePosZ->setMinimum(-99);

        gridLayout_3->addWidget(m_obstaclePosZ, 1, 3, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 1, 1, 1);

        m_obstacleSize = new QDoubleSpinBox(groupBox_3);
        m_obstacleSize->setObjectName(QString::fromUtf8("m_obstacleSize"));
        m_obstacleSize->setMinimum(1);
        m_obstacleSize->setMaximum(10);
        m_obstacleSize->setValue(4);

        gridLayout_3->addWidget(m_obstacleSize, 2, 2, 1, 2);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_5 = new QGroupBox(m_page2_obstacle);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_4 = new QGridLayout(groupBox_5);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        m_obstacleColour = new QPushButton(groupBox_5);
        m_obstacleColour->setObjectName(QString::fromUtf8("m_obstacleColour"));

        gridLayout_4->addWidget(m_obstacleColour, 0, 0, 1, 1);

        m_obstacleWireframe = new QCheckBox(groupBox_5);
        m_obstacleWireframe->setObjectName(QString::fromUtf8("m_obstacleWireframe"));

        gridLayout_4->addWidget(m_obstacleWireframe, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        toolBox->addItem(m_page2_obstacle, QString::fromUtf8("Obstacle"));
        m_page3_simulation = new QWidget();
        m_page3_simulation->setObjectName(QString::fromUtf8("m_page3_simulation"));
        m_page3_simulation->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_4 = new QVBoxLayout(m_page3_simulation);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_7 = new QGroupBox(m_page3_simulation);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_7 = new QGridLayout(groupBox_7);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_5 = new QLabel(groupBox_7);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_7->addWidget(label_5, 0, 0, 1, 1);

        m_simDistance = new QDoubleSpinBox(groupBox_7);
        m_simDistance->setObjectName(QString::fromUtf8("m_simDistance"));
        m_simDistance->setValue(20);

        gridLayout_7->addWidget(m_simDistance, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox_7);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_7->addWidget(label_7, 4, 0, 1, 1);

        m_simCohesion = new QDoubleSpinBox(groupBox_7);
        m_simCohesion->setObjectName(QString::fromUtf8("m_simCohesion"));
        m_simCohesion->setValue(2);

        gridLayout_7->addWidget(m_simCohesion, 5, 0, 1, 1);

        label_8 = new QLabel(groupBox_7);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_7->addWidget(label_8, 6, 0, 1, 1);

        m_simSeparation = new QDoubleSpinBox(groupBox_7);
        m_simSeparation->setObjectName(QString::fromUtf8("m_simSeparation"));
        m_simSeparation->setValue(9);

        gridLayout_7->addWidget(m_simSeparation, 7, 0, 1, 1);

        label_9 = new QLabel(groupBox_7);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_7->addWidget(label_9, 8, 0, 1, 1);

        m_simAlignment = new QDoubleSpinBox(groupBox_7);
        m_simAlignment->setObjectName(QString::fromUtf8("m_simAlignment"));
        m_simAlignment->setValue(10);

        gridLayout_7->addWidget(m_simAlignment, 9, 0, 1, 1);

        label_10 = new QLabel(groupBox_7);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_7->addWidget(label_10, 2, 0, 1, 1);

        m_simFlockDistance = new QDoubleSpinBox(groupBox_7);
        m_simFlockDistance->setObjectName(QString::fromUtf8("m_simFlockDistance"));
        m_simFlockDistance->setValue(4);

        gridLayout_7->addWidget(m_simFlockDistance, 3, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_7);

        toolBox->addItem(m_page3_simulation, QString::fromUtf8("Simulation"));
        m_page4_environment = new QWidget();
        m_page4_environment->setObjectName(QString::fromUtf8("m_page4_environment"));
        m_page4_environment->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_3 = new QVBoxLayout(m_page4_environment);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_4 = new QGroupBox(m_page4_environment);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 0, 0, 1, 1);

        m_bboxSize = new QDoubleSpinBox(groupBox_4);
        m_bboxSize->setObjectName(QString::fromUtf8("m_bboxSize"));
        m_bboxSize->setMinimum(50);
        m_bboxSize->setMaximum(220);
        m_bboxSize->setValue(120);

        gridLayout_5->addWidget(m_bboxSize, 0, 1, 1, 1);


        verticalLayout_3->addWidget(groupBox_4);

        groupBox_6 = new QGroupBox(m_page4_environment);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        m_backColour = new QPushButton(groupBox_6);
        m_backColour->setObjectName(QString::fromUtf8("m_backColour"));

        gridLayout_6->addWidget(m_backColour, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        toolBox->addItem(m_page4_environment, QString::fromUtf8("Environment"));

        s_mainWindowGridLayout->addWidget(toolBox, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1280, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(m_flockDensity);
        label_2->setBuddy(m_changeBoidSize);
        label_3->setBuddy(m_obstacleSize);
        label_4->setBuddy(m_bboxSize);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(m_flockDensity, m_resetFlock);
        QWidget::setTabOrder(m_resetFlock, m_applyFlock);
        QWidget::setTabOrder(m_applyFlock, m_addBoids);
        QWidget::setTabOrder(m_addBoids, m_removeBoids);
        QWidget::setTabOrder(m_removeBoids, m_changeFlockColour);
        QWidget::setTabOrder(m_changeFlockColour, m_changeBoidSize);
        QWidget::setTabOrder(m_changeBoidSize, m_renderFlockWireframe);
        QWidget::setTabOrder(m_renderFlockWireframe, m_obstaclePosX);
        QWidget::setTabOrder(m_obstaclePosX, m_obstaclePosY);
        QWidget::setTabOrder(m_obstaclePosY, m_obstaclePosZ);
        QWidget::setTabOrder(m_obstaclePosZ, m_obstacleSize);
        QWidget::setTabOrder(m_obstacleSize, m_obstacleColour);
        QWidget::setTabOrder(m_obstacleColour, m_obstacleWireframe);
        QWidget::setTabOrder(m_obstacleWireframe, m_simDistance);
        QWidget::setTabOrder(m_simDistance, m_simCohesion);
        QWidget::setTabOrder(m_simCohesion, m_simSeparation);
        QWidget::setTabOrder(m_simSeparation, m_simAlignment);
        QWidget::setTabOrder(m_simAlignment, m_bboxSize);
        QWidget::setTabOrder(m_bboxSize, m_backColour);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "General", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Flock density:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_flockDensity->setToolTip(QApplication::translate("MainWindow", "Sets the total number of Boids in the Flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_flockDensity->setStatusTip(QApplication::translate("MainWindow", "Sets the total number of Boids in the Flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        m_applyFlock->setToolTip(QApplication::translate("MainWindow", "Set the new number of boids in the flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_applyFlock->setStatusTip(QApplication::translate("MainWindow", "Set the new number of boids in the flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        m_applyFlock->setText(QApplication::translate("MainWindow", "Apply", 0, QApplication::UnicodeUTF8));
        m_resetFlock->setText(QApplication::translate("MainWindow", "Reset Flock", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_addBoids->setToolTip(QApplication::translate("MainWindow", "Adds 10 boids to the current flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_addBoids->setStatusTip(QApplication::translate("MainWindow", "Adds 10 boids to the current flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        m_addBoids->setText(QApplication::translate("MainWindow", "+ Add Boids", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_removeBoids->setToolTip(QApplication::translate("MainWindow", "Removes 10 boids from the current flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_removeBoids->setStatusTip(QApplication::translate("MainWindow", "Removes 10 boids from the current flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        m_removeBoids->setText(QApplication::translate("MainWindow", "- Remove Boids", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Appearence", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_changeFlockColour->setToolTip(QApplication::translate("MainWindow", "Change the flock colour", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_changeFlockColour->setStatusTip(QApplication::translate("MainWindow", "Change the flock colour", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        m_changeFlockColour->setText(QApplication::translate("MainWindow", "Change Flock Colour", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Set Boid Size :", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_changeBoidSize->setToolTip(QApplication::translate("MainWindow", "Changes the size of each boid in the flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_changeBoidSize->setStatusTip(QApplication::translate("MainWindow", "Changes the size of each boid in the flock", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        m_renderFlockWireframe->setToolTip(QApplication::translate("MainWindow", "Renders the boids in wireframe mode if checked", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        m_renderFlockWireframe->setStatusTip(QApplication::translate("MainWindow", "Renders the boids in wireframe mode if checked", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        m_renderFlockWireframe->setText(QApplication::translate("MainWindow", "Render Wireframe", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(m_page1_flock), QApplication::translate("MainWindow", "Flock", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "General", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Position:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Size:", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Appearence", 0, QApplication::UnicodeUTF8));
        m_obstacleColour->setText(QApplication::translate("MainWindow", "Change Colour", 0, QApplication::UnicodeUTF8));
        m_obstacleWireframe->setText(QApplication::translate("MainWindow", "Render Wireframe", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(m_page2_obstacle), QApplication::translate("MainWindow", "Obstacle", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "General", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Neighbourhood Distance :", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Cohesion Force :", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Separation Force :", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Alignment Weight :", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Flock Distance", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(m_page3_simulation), QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Bounding Box", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Size :", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Appearence", 0, QApplication::UnicodeUTF8));
        m_backColour->setText(QApplication::translate("MainWindow", "Background Colour", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(m_page4_environment), QApplication::translate("MainWindow", "Environment", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
