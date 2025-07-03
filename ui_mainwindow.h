/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QLabel *label_4;
    QSlider *m_flockSpeedSlider;
    QLabel *m_flockSpeedLabel;
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
    QGridLayout *gridLayout_41;
    QPushButton *m_obstacleColour;
    QLabel *label_specular;
    QDoubleSpinBox *m_obstacleSpecularR;
    QDoubleSpinBox *m_obstacleSpecularG;
    QDoubleSpinBox *m_obstacleSpecularB;
    QLabel *label_diffuse;
    QDoubleSpinBox *m_obstacleDiffuseR;
    QDoubleSpinBox *m_obstacleDiffuseG;
    QDoubleSpinBox *m_obstacleDiffuseB;
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
    QGroupBox *groupBox_41;
    QGridLayout *gridLayout_5;
    QLabel *label_41;
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
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        s_mainWindowGridLayout = new QGridLayout(centralwidget);
        s_mainWindowGridLayout->setObjectName("s_mainWindowGridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        s_mainWindowGridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName("toolBox");
        toolBox->setMaximumSize(QSize(310, 16777215));
        toolBox->setFrameShape(QFrame::Box);
        toolBox->setFrameShadow(QFrame::Sunken);
        m_page1_flock = new QWidget();
        m_page1_flock->setObjectName("m_page1_flock");
        m_page1_flock->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout = new QVBoxLayout(m_page1_flock);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(m_page1_flock);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 1, 1, 1);

        m_flockDensity = new QSpinBox(groupBox);
        m_flockDensity->setObjectName("m_flockDensity");
        m_flockDensity->setMinimum(20);
        m_flockDensity->setMaximum(2000);
        m_flockDensity->setValue(200);

        gridLayout->addWidget(m_flockDensity, 0, 2, 1, 1);

        m_applyFlock = new QPushButton(groupBox);
        m_applyFlock->setObjectName("m_applyFlock");
        m_applyFlock->setEnabled(false);
        m_applyFlock->setFlat(false);

        gridLayout->addWidget(m_applyFlock, 1, 2, 1, 1);

        m_resetFlock = new QPushButton(groupBox);
        m_resetFlock->setObjectName("m_resetFlock");

        gridLayout->addWidget(m_resetFlock, 1, 1, 1, 1);

        m_addBoids = new QPushButton(groupBox);
        m_addBoids->setObjectName("m_addBoids");

        gridLayout->addWidget(m_addBoids, 3, 1, 1, 1);

        m_removeBoids = new QPushButton(groupBox);
        m_removeBoids->setObjectName("m_removeBoids");

        gridLayout->addWidget(m_removeBoids, 3, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(m_page1_flock);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName("gridLayout_2");
        m_changeFlockColour = new QPushButton(groupBox_2);
        m_changeFlockColour->setObjectName("m_changeFlockColour");

        gridLayout_2->addWidget(m_changeFlockColour, 0, 0, 1, 2);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 5, 0, 1, 1);

        m_changeBoidSize = new QDoubleSpinBox(groupBox_2);
        m_changeBoidSize->setObjectName("m_changeBoidSize");
        m_changeBoidSize->setMinimum(0.500000000000000);
        m_changeBoidSize->setMaximum(10.000000000000000);
        m_changeBoidSize->setSingleStep(0.100000000000000);
        m_changeBoidSize->setValue(1.000000000000000);

        gridLayout_2->addWidget(m_changeBoidSize, 5, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(m_page1_flock);
        groupBox_4->setObjectName("groupBox_4");
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName("gridLayout_4");
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName("label_4");

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        m_flockSpeedSlider = new QSlider(groupBox_4);
        m_flockSpeedSlider->setObjectName("m_flockSpeedSlider");
        m_flockSpeedSlider->setMinimum(10);
        m_flockSpeedSlider->setMaximum(300);
        m_flockSpeedSlider->setValue(100);
        m_flockSpeedSlider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(m_flockSpeedSlider, 0, 1, 1, 1);

        m_flockSpeedLabel = new QLabel(groupBox_4);
        m_flockSpeedLabel->setObjectName("m_flockSpeedLabel");
        m_flockSpeedLabel->setMinimumSize(QSize(40, 0));

        gridLayout_4->addWidget(m_flockSpeedLabel, 0, 2, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        m_renderFlockWireframe = new QCheckBox(m_page1_flock);
        m_renderFlockWireframe->setObjectName("m_renderFlockWireframe");

        verticalLayout->addWidget(m_renderFlockWireframe);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        toolBox->addItem(m_page1_flock, QString::fromUtf8("Flock"));
        m_page2_obstacle = new QWidget();
        m_page2_obstacle->setObjectName("m_page2_obstacle");
        m_page2_obstacle->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_2 = new QVBoxLayout(m_page2_obstacle);
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox_3 = new QGroupBox(m_page2_obstacle);
        groupBox_3->setObjectName("groupBox_3");
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName("gridLayout_3");
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName("label_6");

        gridLayout_3->addWidget(label_6, 0, 1, 1, 1);

        m_obstaclePosX = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosX->setObjectName("m_obstaclePosX");
        m_obstaclePosX->setMinimum(-99.000000000000000);
        m_obstaclePosX->setValue(0.000000000000000);

        gridLayout_3->addWidget(m_obstaclePosX, 1, 1, 1, 1);

        m_obstaclePosY = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosY->setObjectName("m_obstaclePosY");
        m_obstaclePosY->setMinimum(-99.000000000000000);
        m_obstaclePosY->setValue(0.000000000000000);

        gridLayout_3->addWidget(m_obstaclePosY, 1, 2, 1, 1);

        m_obstaclePosZ = new QDoubleSpinBox(groupBox_3);
        m_obstaclePosZ->setObjectName("m_obstaclePosZ");
        m_obstaclePosZ->setMinimum(-99.000000000000000);
        m_obstaclePosZ->setValue(0.000000000000000);

        gridLayout_3->addWidget(m_obstaclePosZ, 1, 3, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName("label_3");

        gridLayout_3->addWidget(label_3, 2, 1, 1, 1);

        m_obstacleSize = new QDoubleSpinBox(groupBox_3);
        m_obstacleSize->setObjectName("m_obstacleSize");
        m_obstacleSize->setMinimum(1.000000000000000);
        m_obstacleSize->setMaximum(10.000000000000000);
        m_obstacleSize->setValue(4.000000000000000);

        gridLayout_3->addWidget(m_obstacleSize, 2, 2, 1, 2);


        verticalLayout_2->addWidget(groupBox_3);

        groupBox_5 = new QGroupBox(m_page2_obstacle);
        groupBox_5->setObjectName("groupBox_5");
        gridLayout_41 = new QGridLayout(groupBox_5);
        gridLayout_41->setObjectName("gridLayout_41");
        m_obstacleColour = new QPushButton(groupBox_5);
        m_obstacleColour->setObjectName("m_obstacleColour");

        gridLayout_41->addWidget(m_obstacleColour, 0, 0, 1, 1);

        label_specular = new QLabel(groupBox_5);
        label_specular->setObjectName("label_specular");

        gridLayout_41->addWidget(label_specular, 2, 0, 1, 3);

        m_obstacleSpecularR = new QDoubleSpinBox(groupBox_5);
        m_obstacleSpecularR->setObjectName("m_obstacleSpecularR");
        m_obstacleSpecularR->setMinimum(0.000000000000000);
        m_obstacleSpecularR->setMaximum(1.000000000000000);
        m_obstacleSpecularR->setSingleStep(0.010000000000000);
        m_obstacleSpecularR->setValue(1.000000000000000);

        gridLayout_41->addWidget(m_obstacleSpecularR, 3, 0, 1, 1);

        m_obstacleSpecularG = new QDoubleSpinBox(groupBox_5);
        m_obstacleSpecularG->setObjectName("m_obstacleSpecularG");
        m_obstacleSpecularG->setMinimum(0.000000000000000);
        m_obstacleSpecularG->setMaximum(1.000000000000000);
        m_obstacleSpecularG->setSingleStep(0.010000000000000);
        m_obstacleSpecularG->setValue(1.000000000000000);

        gridLayout_41->addWidget(m_obstacleSpecularG, 3, 1, 1, 1);

        m_obstacleSpecularB = new QDoubleSpinBox(groupBox_5);
        m_obstacleSpecularB->setObjectName("m_obstacleSpecularB");
        m_obstacleSpecularB->setMinimum(0.000000000000000);
        m_obstacleSpecularB->setMaximum(1.000000000000000);
        m_obstacleSpecularB->setSingleStep(0.010000000000000);
        m_obstacleSpecularB->setValue(1.000000000000000);

        gridLayout_41->addWidget(m_obstacleSpecularB, 3, 2, 1, 1);

        label_diffuse = new QLabel(groupBox_5);
        label_diffuse->setObjectName("label_diffuse");

        gridLayout_41->addWidget(label_diffuse, 4, 0, 1, 3);

        m_obstacleDiffuseR = new QDoubleSpinBox(groupBox_5);
        m_obstacleDiffuseR->setObjectName("m_obstacleDiffuseR");
        m_obstacleDiffuseR->setMinimum(0.000000000000000);
        m_obstacleDiffuseR->setMaximum(1.000000000000000);
        m_obstacleDiffuseR->setSingleStep(0.010000000000000);
        m_obstacleDiffuseR->setValue(0.600000000000000);

        gridLayout_41->addWidget(m_obstacleDiffuseR, 5, 0, 1, 1);

        m_obstacleDiffuseG = new QDoubleSpinBox(groupBox_5);
        m_obstacleDiffuseG->setObjectName("m_obstacleDiffuseG");
        m_obstacleDiffuseG->setMinimum(0.000000000000000);
        m_obstacleDiffuseG->setMaximum(1.000000000000000);
        m_obstacleDiffuseG->setSingleStep(0.010000000000000);
        m_obstacleDiffuseG->setValue(0.400000000000000);

        gridLayout_41->addWidget(m_obstacleDiffuseG, 5, 1, 1, 1);

        m_obstacleDiffuseB = new QDoubleSpinBox(groupBox_5);
        m_obstacleDiffuseB->setObjectName("m_obstacleDiffuseB");
        m_obstacleDiffuseB->setMinimum(0.000000000000000);
        m_obstacleDiffuseB->setMaximum(1.000000000000000);
        m_obstacleDiffuseB->setSingleStep(0.010000000000000);
        m_obstacleDiffuseB->setValue(0.200000000000000);

        gridLayout_41->addWidget(m_obstacleDiffuseB, 5, 2, 1, 1);

        m_obstacleWireframe = new QCheckBox(groupBox_5);
        m_obstacleWireframe->setObjectName("m_obstacleWireframe");

        gridLayout_41->addWidget(m_obstacleWireframe, 1, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        toolBox->addItem(m_page2_obstacle, QString::fromUtf8("Obstacle"));
        m_page3_simulation = new QWidget();
        m_page3_simulation->setObjectName("m_page3_simulation");
        m_page3_simulation->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_4 = new QVBoxLayout(m_page3_simulation);
        verticalLayout_4->setObjectName("verticalLayout_4");
        groupBox_7 = new QGroupBox(m_page3_simulation);
        groupBox_7->setObjectName("groupBox_7");
        gridLayout_7 = new QGridLayout(groupBox_7);
        gridLayout_7->setObjectName("gridLayout_7");
        label_5 = new QLabel(groupBox_7);
        label_5->setObjectName("label_5");

        gridLayout_7->addWidget(label_5, 0, 0, 1, 1);

        m_simDistance = new QDoubleSpinBox(groupBox_7);
        m_simDistance->setObjectName("m_simDistance");
        m_simDistance->setValue(20.000000000000000);

        gridLayout_7->addWidget(m_simDistance, 1, 0, 1, 1);

        label_7 = new QLabel(groupBox_7);
        label_7->setObjectName("label_7");

        gridLayout_7->addWidget(label_7, 4, 0, 1, 1);

        m_simCohesion = new QDoubleSpinBox(groupBox_7);
        m_simCohesion->setObjectName("m_simCohesion");
        m_simCohesion->setValue(2.000000000000000);

        gridLayout_7->addWidget(m_simCohesion, 5, 0, 1, 1);

        label_8 = new QLabel(groupBox_7);
        label_8->setObjectName("label_8");

        gridLayout_7->addWidget(label_8, 6, 0, 1, 1);

        m_simSeparation = new QDoubleSpinBox(groupBox_7);
        m_simSeparation->setObjectName("m_simSeparation");
        m_simSeparation->setValue(9.000000000000000);

        gridLayout_7->addWidget(m_simSeparation, 7, 0, 1, 1);

        label_9 = new QLabel(groupBox_7);
        label_9->setObjectName("label_9");

        gridLayout_7->addWidget(label_9, 8, 0, 1, 1);

        m_simAlignment = new QDoubleSpinBox(groupBox_7);
        m_simAlignment->setObjectName("m_simAlignment");
        m_simAlignment->setValue(10.000000000000000);

        gridLayout_7->addWidget(m_simAlignment, 9, 0, 1, 1);

        label_10 = new QLabel(groupBox_7);
        label_10->setObjectName("label_10");

        gridLayout_7->addWidget(label_10, 2, 0, 1, 1);

        m_simFlockDistance = new QDoubleSpinBox(groupBox_7);
        m_simFlockDistance->setObjectName("m_simFlockDistance");
        m_simFlockDistance->setValue(4.000000000000000);

        gridLayout_7->addWidget(m_simFlockDistance, 3, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_7);

        toolBox->addItem(m_page3_simulation, QString::fromUtf8("Simulation"));
        m_page4_environment = new QWidget();
        m_page4_environment->setObjectName("m_page4_environment");
        m_page4_environment->setGeometry(QRect(0, 0, 308, 529));
        verticalLayout_3 = new QVBoxLayout(m_page4_environment);
        verticalLayout_3->setObjectName("verticalLayout_3");
        groupBox_41 = new QGroupBox(m_page4_environment);
        groupBox_41->setObjectName("groupBox_41");
        gridLayout_5 = new QGridLayout(groupBox_41);
        gridLayout_5->setObjectName("gridLayout_5");
        label_41 = new QLabel(groupBox_41);
        label_41->setObjectName("label_41");

        gridLayout_5->addWidget(label_41, 0, 0, 1, 1);

        m_bboxSize = new QDoubleSpinBox(groupBox_41);
        m_bboxSize->setObjectName("m_bboxSize");
        m_bboxSize->setMinimum(50.000000000000000);
        m_bboxSize->setMaximum(220.000000000000000);
        m_bboxSize->setValue(120.000000000000000);

        gridLayout_5->addWidget(m_bboxSize, 0, 1, 1, 1);


        verticalLayout_3->addWidget(groupBox_41);

        groupBox_6 = new QGroupBox(m_page4_environment);
        groupBox_6->setObjectName("groupBox_6");
        gridLayout_6 = new QGridLayout(groupBox_6);
        gridLayout_6->setObjectName("gridLayout_6");
        m_backColour = new QPushButton(groupBox_6);
        m_backColour->setObjectName("m_backColour");

        gridLayout_6->addWidget(m_backColour, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        toolBox->addItem(m_page4_environment, QString::fromUtf8("Environment"));

        s_mainWindowGridLayout->addWidget(toolBox, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
#if QT_CONFIG(shortcut)
        label->setBuddy(m_flockDensity);
        label_2->setBuddy(m_changeBoidSize);
        label_4->setBuddy(m_flockSpeedSlider);
        label_3->setBuddy(m_obstacleSize);
        label_41->setBuddy(m_bboxSize);
#endif // QT_CONFIG(shortcut)
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "General", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Flock density:", nullptr));
#if QT_CONFIG(tooltip)
        m_flockDensity->setToolTip(QCoreApplication::translate("MainWindow", "Sets the total number of Boids in the Flock", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_flockDensity->setStatusTip(QCoreApplication::translate("MainWindow", "Sets the total number of Boids in the Flock", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        m_applyFlock->setToolTip(QCoreApplication::translate("MainWindow", "Set the new number of boids in the flock", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_applyFlock->setStatusTip(QCoreApplication::translate("MainWindow", "Set the new number of boids in the flock", nullptr));
#endif // QT_CONFIG(statustip)
        m_applyFlock->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        m_resetFlock->setText(QCoreApplication::translate("MainWindow", "Reset Flock", nullptr));
#if QT_CONFIG(tooltip)
        m_addBoids->setToolTip(QCoreApplication::translate("MainWindow", "Adds 10 boids to the current flock", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_addBoids->setStatusTip(QCoreApplication::translate("MainWindow", "Adds 10 boids to the current flock", nullptr));
#endif // QT_CONFIG(statustip)
        m_addBoids->setText(QCoreApplication::translate("MainWindow", "+ Add Boids", nullptr));
#if QT_CONFIG(tooltip)
        m_removeBoids->setToolTip(QCoreApplication::translate("MainWindow", "Removes 10 boids from the current flock", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_removeBoids->setStatusTip(QCoreApplication::translate("MainWindow", "Removes 10 boids from the current flock", nullptr));
#endif // QT_CONFIG(statustip)
        m_removeBoids->setText(QCoreApplication::translate("MainWindow", "- Remove Boids", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Appearence", nullptr));
#if QT_CONFIG(tooltip)
        m_changeFlockColour->setToolTip(QCoreApplication::translate("MainWindow", "Change the flock colour", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_changeFlockColour->setStatusTip(QCoreApplication::translate("MainWindow", "Change the flock colour", nullptr));
#endif // QT_CONFIG(statustip)
        m_changeFlockColour->setText(QCoreApplication::translate("MainWindow", "Change Flock Colour", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Set Boid Size :", nullptr));
#if QT_CONFIG(tooltip)
        m_changeBoidSize->setToolTip(QCoreApplication::translate("MainWindow", "Changes the size of each boid in the flock", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_changeBoidSize->setStatusTip(QCoreApplication::translate("MainWindow", "Changes the size of each boid in the flock", nullptr));
#endif // QT_CONFIG(statustip)
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Movement", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Flock Speed:", nullptr));
#if QT_CONFIG(tooltip)
        m_flockSpeedSlider->setToolTip(QCoreApplication::translate("MainWindow", "Controls how fast the flock moves", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_flockSpeedSlider->setStatusTip(QCoreApplication::translate("MainWindow", "Controls how fast the flock moves", nullptr));
#endif // QT_CONFIG(statustip)
        m_flockSpeedLabel->setText(QCoreApplication::translate("MainWindow", "100%", nullptr));
#if QT_CONFIG(tooltip)
        m_renderFlockWireframe->setToolTip(QCoreApplication::translate("MainWindow", "Renders the boids in wireframe mode if checked", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        m_renderFlockWireframe->setStatusTip(QCoreApplication::translate("MainWindow", "Renders the boids in wireframe mode if checked", nullptr));
#endif // QT_CONFIG(statustip)
        m_renderFlockWireframe->setText(QCoreApplication::translate("MainWindow", "Render Wireframe", nullptr));
        toolBox->setItemText(toolBox->indexOf(m_page1_flock), QCoreApplication::translate("MainWindow", "Flock", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "General", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Position:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Size:", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Appearence", nullptr));
        m_obstacleColour->setText(QCoreApplication::translate("MainWindow", "Change Colour", nullptr));
        label_specular->setText(QCoreApplication::translate("MainWindow", "Specular (RGB):", nullptr));
        label_diffuse->setText(QCoreApplication::translate("MainWindow", "Diffuse (RGB):", nullptr));
        m_obstacleWireframe->setText(QCoreApplication::translate("MainWindow", "Render Wireframe", nullptr));
        toolBox->setItemText(toolBox->indexOf(m_page2_obstacle), QCoreApplication::translate("MainWindow", "Obstacle", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "General", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Neighbourhood Distance :", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Cohesion Force :", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Separation Force :", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Alignment Weight :", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Flock Distance", nullptr));
        toolBox->setItemText(toolBox->indexOf(m_page3_simulation), QCoreApplication::translate("MainWindow", "Simulation", nullptr));
        groupBox_41->setTitle(QCoreApplication::translate("MainWindow", "Bounding Box", nullptr));
        label_41->setText(QCoreApplication::translate("MainWindow", "Size :", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "Appearence", nullptr));
        m_backColour->setText(QCoreApplication::translate("MainWindow", "Background Colour", nullptr));
        toolBox->setItemText(toolBox->indexOf(m_page4_environment), QCoreApplication::translate("MainWindow", "Environment", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
