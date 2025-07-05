# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.9.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QCheckBox, QDoubleSpinBox, QFrame,
    QGridLayout, QGroupBox, QLabel, QMainWindow,
    QMenuBar, QPushButton, QSizePolicy, QSlider,
    QSpacerItem, QSpinBox, QStatusBar, QToolBox,
    QVBoxLayout, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(1280, 720)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.s_mainWindowGridLayout = QGridLayout(self.centralwidget)
        self.s_mainWindowGridLayout.setObjectName(u"s_mainWindowGridLayout")
        self.horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)

        self.s_mainWindowGridLayout.addItem(self.horizontalSpacer, 0, 0, 1, 1)

        self.toolBox = QToolBox(self.centralwidget)
        self.toolBox.setObjectName(u"toolBox")
        self.toolBox.setMaximumSize(QSize(310, 16777215))
        self.toolBox.setFrameShape(QFrame.Box)
        self.toolBox.setFrameShadow(QFrame.Sunken)
        self.m_page1_flock = QWidget()
        self.m_page1_flock.setObjectName(u"m_page1_flock")
        self.m_page1_flock.setGeometry(QRect(0, 0, 308, 529))
        self.verticalLayout = QVBoxLayout(self.m_page1_flock)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.groupBox = QGroupBox(self.m_page1_flock)
        self.groupBox.setObjectName(u"groupBox")
        self.gridLayout = QGridLayout(self.groupBox)
        self.gridLayout.setObjectName(u"gridLayout")
        self.label = QLabel(self.groupBox)
        self.label.setObjectName(u"label")

        self.gridLayout.addWidget(self.label, 0, 1, 1, 1)

        self.m_flockDensity = QSpinBox(self.groupBox)
        self.m_flockDensity.setObjectName(u"m_flockDensity")
        self.m_flockDensity.setMinimum(20)
        self.m_flockDensity.setMaximum(2000)
        self.m_flockDensity.setValue(200)

        self.gridLayout.addWidget(self.m_flockDensity, 0, 2, 1, 1)

        self.m_applyFlock = QPushButton(self.groupBox)
        self.m_applyFlock.setObjectName(u"m_applyFlock")
        self.m_applyFlock.setEnabled(False)
        self.m_applyFlock.setFlat(False)

        self.gridLayout.addWidget(self.m_applyFlock, 1, 2, 1, 1)

        self.m_resetFlock = QPushButton(self.groupBox)
        self.m_resetFlock.setObjectName(u"m_resetFlock")

        self.gridLayout.addWidget(self.m_resetFlock, 1, 1, 1, 1)

        self.m_addBoids = QPushButton(self.groupBox)
        self.m_addBoids.setObjectName(u"m_addBoids")

        self.gridLayout.addWidget(self.m_addBoids, 3, 1, 1, 1)

        self.m_removeBoids = QPushButton(self.groupBox)
        self.m_removeBoids.setObjectName(u"m_removeBoids")

        self.gridLayout.addWidget(self.m_removeBoids, 3, 2, 1, 1)


        self.verticalLayout.addWidget(self.groupBox)

        self.groupBox_2 = QGroupBox(self.m_page1_flock)
        self.groupBox_2.setObjectName(u"groupBox_2")
        self.gridLayout_2 = QGridLayout(self.groupBox_2)
        self.gridLayout_2.setObjectName(u"gridLayout_2")
        self.m_changeFlockColour = QPushButton(self.groupBox_2)
        self.m_changeFlockColour.setObjectName(u"m_changeFlockColour")

        self.gridLayout_2.addWidget(self.m_changeFlockColour, 0, 0, 1, 2)

        self.label_2 = QLabel(self.groupBox_2)
        self.label_2.setObjectName(u"label_2")

        self.gridLayout_2.addWidget(self.label_2, 5, 0, 1, 1)

        self.m_changeBoidSize = QDoubleSpinBox(self.groupBox_2)
        self.m_changeBoidSize.setObjectName(u"m_changeBoidSize")
        self.m_changeBoidSize.setMinimum(0.500000000000000)
        self.m_changeBoidSize.setMaximum(10.000000000000000)
        self.m_changeBoidSize.setSingleStep(0.100000000000000)
        self.m_changeBoidSize.setValue(1.000000000000000)

        self.gridLayout_2.addWidget(self.m_changeBoidSize, 5, 1, 1, 1)


        self.verticalLayout.addWidget(self.groupBox_2)

        self.groupBox_4 = QGroupBox(self.m_page1_flock)
        self.groupBox_4.setObjectName(u"groupBox_4")
        self.gridLayout_4 = QGridLayout(self.groupBox_4)
        self.gridLayout_4.setObjectName(u"gridLayout_4")
        self.label_4 = QLabel(self.groupBox_4)
        self.label_4.setObjectName(u"label_4")

        self.gridLayout_4.addWidget(self.label_4, 0, 0, 1, 1)

        self.m_flockSpeedSlider = QSlider(self.groupBox_4)
        self.m_flockSpeedSlider.setObjectName(u"m_flockSpeedSlider")
        self.m_flockSpeedSlider.setMinimum(10)
        self.m_flockSpeedSlider.setMaximum(300)
        self.m_flockSpeedSlider.setValue(100)
        self.m_flockSpeedSlider.setOrientation(Qt.Horizontal)

        self.gridLayout_4.addWidget(self.m_flockSpeedSlider, 0, 1, 1, 1)

        self.m_flockSpeedLabel = QLabel(self.groupBox_4)
        self.m_flockSpeedLabel.setObjectName(u"m_flockSpeedLabel")
        self.m_flockSpeedLabel.setMinimumSize(QSize(40, 0))

        self.gridLayout_4.addWidget(self.m_flockSpeedLabel, 0, 2, 1, 1)


        self.verticalLayout.addWidget(self.groupBox_4)

        self.m_renderFlockWireframe = QCheckBox(self.m_page1_flock)
        self.m_renderFlockWireframe.setObjectName(u"m_renderFlockWireframe")

        self.verticalLayout.addWidget(self.m_renderFlockWireframe)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)

        self.toolBox.addItem(self.m_page1_flock, u"Flock")
        self.m_page2_obstacle = QWidget()
        self.m_page2_obstacle.setObjectName(u"m_page2_obstacle")
        self.m_page2_obstacle.setGeometry(QRect(0, 0, 308, 529))
        self.verticalLayout_2 = QVBoxLayout(self.m_page2_obstacle)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.groupBox_3 = QGroupBox(self.m_page2_obstacle)
        self.groupBox_3.setObjectName(u"groupBox_3")
        self.gridLayout_3 = QGridLayout(self.groupBox_3)
        self.gridLayout_3.setObjectName(u"gridLayout_3")
        self.m_enableObstacle = QCheckBox(self.groupBox_3)
        self.m_enableObstacle.setObjectName(u"m_enableObstacle")
        self.m_enableObstacle.setChecked(True)

        self.gridLayout_3.addWidget(self.m_enableObstacle, 0, 1, 1, 3)

        self.label_6 = QLabel(self.groupBox_3)
        self.label_6.setObjectName(u"label_6")

        self.gridLayout_3.addWidget(self.label_6, 1, 1, 1, 1)

        self.m_obstaclePosX = QDoubleSpinBox(self.groupBox_3)
        self.m_obstaclePosX.setObjectName(u"m_obstaclePosX")
        self.m_obstaclePosX.setMinimum(-99.000000000000000)
        self.m_obstaclePosX.setValue(0.000000000000000)

        self.gridLayout_3.addWidget(self.m_obstaclePosX, 2, 1, 1, 1)

        self.m_obstaclePosY = QDoubleSpinBox(self.groupBox_3)
        self.m_obstaclePosY.setObjectName(u"m_obstaclePosY")
        self.m_obstaclePosY.setMinimum(-99.000000000000000)
        self.m_obstaclePosY.setValue(0.000000000000000)

        self.gridLayout_3.addWidget(self.m_obstaclePosY, 2, 2, 1, 1)

        self.m_obstaclePosZ = QDoubleSpinBox(self.groupBox_3)
        self.m_obstaclePosZ.setObjectName(u"m_obstaclePosZ")
        self.m_obstaclePosZ.setMinimum(-99.000000000000000)
        self.m_obstaclePosZ.setValue(0.000000000000000)

        self.gridLayout_3.addWidget(self.m_obstaclePosZ, 2, 3, 1, 1)

        self.label_3 = QLabel(self.groupBox_3)
        self.label_3.setObjectName(u"label_3")

        self.gridLayout_3.addWidget(self.label_3, 3, 1, 1, 1)

        self.m_obstacleSize = QDoubleSpinBox(self.groupBox_3)
        self.m_obstacleSize.setObjectName(u"m_obstacleSize")
        self.m_obstacleSize.setMinimum(1.000000000000000)
        self.m_obstacleSize.setMaximum(10.000000000000000)
        self.m_obstacleSize.setValue(4.000000000000000)

        self.gridLayout_3.addWidget(self.m_obstacleSize, 3, 2, 1, 2)


        self.verticalLayout_2.addWidget(self.groupBox_3)

        self.groupBox_5 = QGroupBox(self.m_page2_obstacle)
        self.groupBox_5.setObjectName(u"groupBox_5")
        self.gridLayout_41 = QGridLayout(self.groupBox_5)
        self.gridLayout_41.setObjectName(u"gridLayout_41")
        self.m_obstacleColour = QPushButton(self.groupBox_5)
        self.m_obstacleColour.setObjectName(u"m_obstacleColour")

        self.gridLayout_41.addWidget(self.m_obstacleColour, 0, 0, 1, 1)

        self.label_specular = QLabel(self.groupBox_5)
        self.label_specular.setObjectName(u"label_specular")

        self.gridLayout_41.addWidget(self.label_specular, 2, 0, 1, 3)

        self.m_obstacleSpecularR = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleSpecularR.setObjectName(u"m_obstacleSpecularR")
        self.m_obstacleSpecularR.setMinimum(0.000000000000000)
        self.m_obstacleSpecularR.setMaximum(1.000000000000000)
        self.m_obstacleSpecularR.setSingleStep(0.010000000000000)
        self.m_obstacleSpecularR.setValue(1.000000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleSpecularR, 3, 0, 1, 1)

        self.m_obstacleSpecularG = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleSpecularG.setObjectName(u"m_obstacleSpecularG")
        self.m_obstacleSpecularG.setMinimum(0.000000000000000)
        self.m_obstacleSpecularG.setMaximum(1.000000000000000)
        self.m_obstacleSpecularG.setSingleStep(0.010000000000000)
        self.m_obstacleSpecularG.setValue(1.000000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleSpecularG, 3, 1, 1, 1)

        self.m_obstacleSpecularB = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleSpecularB.setObjectName(u"m_obstacleSpecularB")
        self.m_obstacleSpecularB.setMinimum(0.000000000000000)
        self.m_obstacleSpecularB.setMaximum(1.000000000000000)
        self.m_obstacleSpecularB.setSingleStep(0.010000000000000)
        self.m_obstacleSpecularB.setValue(1.000000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleSpecularB, 3, 2, 1, 1)

        self.label_diffuse = QLabel(self.groupBox_5)
        self.label_diffuse.setObjectName(u"label_diffuse")

        self.gridLayout_41.addWidget(self.label_diffuse, 4, 0, 1, 3)

        self.m_obstacleDiffuseR = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleDiffuseR.setObjectName(u"m_obstacleDiffuseR")
        self.m_obstacleDiffuseR.setMinimum(0.000000000000000)
        self.m_obstacleDiffuseR.setMaximum(1.000000000000000)
        self.m_obstacleDiffuseR.setSingleStep(0.010000000000000)
        self.m_obstacleDiffuseR.setValue(0.600000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleDiffuseR, 5, 0, 1, 1)

        self.m_obstacleDiffuseG = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleDiffuseG.setObjectName(u"m_obstacleDiffuseG")
        self.m_obstacleDiffuseG.setMinimum(0.000000000000000)
        self.m_obstacleDiffuseG.setMaximum(1.000000000000000)
        self.m_obstacleDiffuseG.setSingleStep(0.010000000000000)
        self.m_obstacleDiffuseG.setValue(0.400000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleDiffuseG, 5, 1, 1, 1)

        self.m_obstacleDiffuseB = QDoubleSpinBox(self.groupBox_5)
        self.m_obstacleDiffuseB.setObjectName(u"m_obstacleDiffuseB")
        self.m_obstacleDiffuseB.setMinimum(0.000000000000000)
        self.m_obstacleDiffuseB.setMaximum(1.000000000000000)
        self.m_obstacleDiffuseB.setSingleStep(0.010000000000000)
        self.m_obstacleDiffuseB.setValue(0.200000000000000)

        self.gridLayout_41.addWidget(self.m_obstacleDiffuseB, 5, 2, 1, 1)

        self.m_obstacleWireframe = QCheckBox(self.groupBox_5)
        self.m_obstacleWireframe.setObjectName(u"m_obstacleWireframe")

        self.gridLayout_41.addWidget(self.m_obstacleWireframe, 1, 0, 1, 1)


        self.verticalLayout_2.addWidget(self.groupBox_5)

        self.groupBox_obstacleAvoidance = QGroupBox(self.m_page2_obstacle)
        self.groupBox_obstacleAvoidance.setObjectName(u"groupBox_obstacleAvoidance")
        self.gridLayout_obstacleAvoidance = QGridLayout(self.groupBox_obstacleAvoidance)
        self.gridLayout_obstacleAvoidance.setObjectName(u"gridLayout_obstacleAvoidance")
        self.label_avoidanceRadius = QLabel(self.groupBox_obstacleAvoidance)
        self.label_avoidanceRadius.setObjectName(u"label_avoidanceRadius")

        self.gridLayout_obstacleAvoidance.addWidget(self.label_avoidanceRadius, 0, 0, 1, 1)

        self.m_obstacleAvoidanceRadiusSlider = QSlider(self.groupBox_obstacleAvoidance)
        self.m_obstacleAvoidanceRadiusSlider.setObjectName(u"m_obstacleAvoidanceRadiusSlider")
        self.m_obstacleAvoidanceRadiusSlider.setMinimum(10)
        self.m_obstacleAvoidanceRadiusSlider.setMaximum(100)
        self.m_obstacleAvoidanceRadiusSlider.setValue(30)
        self.m_obstacleAvoidanceRadiusSlider.setOrientation(Qt.Horizontal)

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleAvoidanceRadiusSlider, 0, 1, 1, 1)

        self.m_obstacleAvoidanceRadiusValue = QLabel(self.groupBox_obstacleAvoidance)
        self.m_obstacleAvoidanceRadiusValue.setObjectName(u"m_obstacleAvoidanceRadiusValue")

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleAvoidanceRadiusValue, 0, 2, 1, 1)

        self.label_collisionRadius = QLabel(self.groupBox_obstacleAvoidance)
        self.label_collisionRadius.setObjectName(u"label_collisionRadius")

        self.gridLayout_obstacleAvoidance.addWidget(self.label_collisionRadius, 1, 0, 1, 1)

        self.m_obstacleCollisionRadiusSlider = QSlider(self.groupBox_obstacleAvoidance)
        self.m_obstacleCollisionRadiusSlider.setObjectName(u"m_obstacleCollisionRadiusSlider")
        self.m_obstacleCollisionRadiusSlider.setMinimum(10)
        self.m_obstacleCollisionRadiusSlider.setMaximum(60)
        self.m_obstacleCollisionRadiusSlider.setValue(13)
        self.m_obstacleCollisionRadiusSlider.setOrientation(Qt.Horizontal)

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleCollisionRadiusSlider, 1, 1, 1, 1)

        self.m_obstacleCollisionRadiusValue = QLabel(self.groupBox_obstacleAvoidance)
        self.m_obstacleCollisionRadiusValue.setObjectName(u"m_obstacleCollisionRadiusValue")

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleCollisionRadiusValue, 1, 2, 1, 1)

        self.label_repulsionForce = QLabel(self.groupBox_obstacleAvoidance)
        self.label_repulsionForce.setObjectName(u"label_repulsionForce")

        self.gridLayout_obstacleAvoidance.addWidget(self.label_repulsionForce, 2, 0, 1, 1)

        self.m_obstacleRepulsionForceSlider = QSlider(self.groupBox_obstacleAvoidance)
        self.m_obstacleRepulsionForceSlider.setObjectName(u"m_obstacleRepulsionForceSlider")
        self.m_obstacleRepulsionForceSlider.setMinimum(10)
        self.m_obstacleRepulsionForceSlider.setMaximum(200)
        self.m_obstacleRepulsionForceSlider.setValue(45)
        self.m_obstacleRepulsionForceSlider.setOrientation(Qt.Horizontal)

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleRepulsionForceSlider, 2, 1, 1, 1)

        self.m_obstacleRepulsionForceValue = QLabel(self.groupBox_obstacleAvoidance)
        self.m_obstacleRepulsionForceValue.setObjectName(u"m_obstacleRepulsionForceValue")

        self.gridLayout_obstacleAvoidance.addWidget(self.m_obstacleRepulsionForceValue, 2, 2, 1, 1)


        self.verticalLayout_2.addWidget(self.groupBox_obstacleAvoidance)

        self.verticalSpacer_2 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_2.addItem(self.verticalSpacer_2)

        self.toolBox.addItem(self.m_page2_obstacle, u"Obstacle")
        self.m_page3_simulation = QWidget()
        self.m_page3_simulation.setObjectName(u"m_page3_simulation")
        self.m_page3_simulation.setGeometry(QRect(0, 0, 308, 529))
        self.verticalLayout_4 = QVBoxLayout(self.m_page3_simulation)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.groupBox_7 = QGroupBox(self.m_page3_simulation)
        self.groupBox_7.setObjectName(u"groupBox_7")
        self.gridLayout_7 = QGridLayout(self.groupBox_7)
        self.gridLayout_7.setObjectName(u"gridLayout_7")
        self.label_5 = QLabel(self.groupBox_7)
        self.label_5.setObjectName(u"label_5")

        self.gridLayout_7.addWidget(self.label_5, 0, 0, 1, 1)

        self.m_simDistance = QDoubleSpinBox(self.groupBox_7)
        self.m_simDistance.setObjectName(u"m_simDistance")
        self.m_simDistance.setValue(20.000000000000000)

        self.gridLayout_7.addWidget(self.m_simDistance, 1, 0, 1, 1)

        self.label_7 = QLabel(self.groupBox_7)
        self.label_7.setObjectName(u"label_7")

        self.gridLayout_7.addWidget(self.label_7, 4, 0, 1, 1)

        self.m_simCohesion = QDoubleSpinBox(self.groupBox_7)
        self.m_simCohesion.setObjectName(u"m_simCohesion")
        self.m_simCohesion.setValue(2.000000000000000)

        self.gridLayout_7.addWidget(self.m_simCohesion, 5, 0, 1, 1)

        self.label_8 = QLabel(self.groupBox_7)
        self.label_8.setObjectName(u"label_8")

        self.gridLayout_7.addWidget(self.label_8, 6, 0, 1, 1)

        self.m_simSeparation = QDoubleSpinBox(self.groupBox_7)
        self.m_simSeparation.setObjectName(u"m_simSeparation")
        self.m_simSeparation.setValue(9.000000000000000)

        self.gridLayout_7.addWidget(self.m_simSeparation, 7, 0, 1, 1)

        self.label_9 = QLabel(self.groupBox_7)
        self.label_9.setObjectName(u"label_9")

        self.gridLayout_7.addWidget(self.label_9, 8, 0, 1, 1)

        self.m_simAlignment = QDoubleSpinBox(self.groupBox_7)
        self.m_simAlignment.setObjectName(u"m_simAlignment")
        self.m_simAlignment.setValue(10.000000000000000)

        self.gridLayout_7.addWidget(self.m_simAlignment, 9, 0, 1, 1)

        self.label_10 = QLabel(self.groupBox_7)
        self.label_10.setObjectName(u"label_10")

        self.gridLayout_7.addWidget(self.label_10, 2, 0, 1, 1)

        self.m_simFlockDistance = QDoubleSpinBox(self.groupBox_7)
        self.m_simFlockDistance.setObjectName(u"m_simFlockDistance")
        self.m_simFlockDistance.setValue(4.000000000000000)

        self.gridLayout_7.addWidget(self.m_simFlockDistance, 3, 0, 1, 1)


        self.verticalLayout_4.addWidget(self.groupBox_7)

        self.toolBox.addItem(self.m_page3_simulation, u"Simulation")
        self.m_page4_environment = QWidget()
        self.m_page4_environment.setObjectName(u"m_page4_environment")
        self.m_page4_environment.setGeometry(QRect(0, 0, 308, 529))
        self.verticalLayout_3 = QVBoxLayout(self.m_page4_environment)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.groupBox_41 = QGroupBox(self.m_page4_environment)
        self.groupBox_41.setObjectName(u"groupBox_41")
        self.gridLayout_5 = QGridLayout(self.groupBox_41)
        self.gridLayout_5.setObjectName(u"gridLayout_5")
        self.label_41 = QLabel(self.groupBox_41)
        self.label_41.setObjectName(u"label_41")

        self.gridLayout_5.addWidget(self.label_41, 0, 0, 1, 1)

        self.m_bboxSize = QDoubleSpinBox(self.groupBox_41)
        self.m_bboxSize.setObjectName(u"m_bboxSize")
        self.m_bboxSize.setMinimum(50.000000000000000)
        self.m_bboxSize.setMaximum(220.000000000000000)
        self.m_bboxSize.setValue(120.000000000000000)

        self.gridLayout_5.addWidget(self.m_bboxSize, 0, 1, 1, 1)


        self.verticalLayout_3.addWidget(self.groupBox_41)

        self.groupBox_6 = QGroupBox(self.m_page4_environment)
        self.groupBox_6.setObjectName(u"groupBox_6")
        self.gridLayout_6 = QGridLayout(self.groupBox_6)
        self.gridLayout_6.setObjectName(u"gridLayout_6")
        self.m_backColour = QPushButton(self.groupBox_6)
        self.m_backColour.setObjectName(u"m_backColour")

        self.gridLayout_6.addWidget(self.m_backColour, 0, 0, 1, 1)


        self.verticalLayout_3.addWidget(self.groupBox_6)

        self.verticalSpacer_3 = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout_3.addItem(self.verticalSpacer_3)

        self.toolBox.addItem(self.m_page4_environment, u"Environment")

        self.s_mainWindowGridLayout.addWidget(self.toolBox, 0, 1, 1, 1)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1280, 25))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)
#if QT_CONFIG(shortcut)
        self.label.setBuddy(self.m_flockDensity)
        self.label_2.setBuddy(self.m_changeBoidSize)
        self.label_4.setBuddy(self.m_flockSpeedSlider)
        self.label_3.setBuddy(self.m_obstacleSize)
        self.label_41.setBuddy(self.m_bboxSize)
#endif // QT_CONFIG(shortcut)
        QWidget.setTabOrder(self.m_flockDensity, self.m_resetFlock)
        QWidget.setTabOrder(self.m_resetFlock, self.m_applyFlock)
        QWidget.setTabOrder(self.m_applyFlock, self.m_addBoids)
        QWidget.setTabOrder(self.m_addBoids, self.m_removeBoids)
        QWidget.setTabOrder(self.m_removeBoids, self.m_changeFlockColour)
        QWidget.setTabOrder(self.m_changeFlockColour, self.m_changeBoidSize)
        QWidget.setTabOrder(self.m_changeBoidSize, self.m_renderFlockWireframe)
        QWidget.setTabOrder(self.m_renderFlockWireframe, self.m_obstaclePosX)
        QWidget.setTabOrder(self.m_obstaclePosX, self.m_obstaclePosY)
        QWidget.setTabOrder(self.m_obstaclePosY, self.m_obstaclePosZ)
        QWidget.setTabOrder(self.m_obstaclePosZ, self.m_obstacleSize)
        QWidget.setTabOrder(self.m_obstacleSize, self.m_obstacleColour)
        QWidget.setTabOrder(self.m_obstacleColour, self.m_obstacleWireframe)
        QWidget.setTabOrder(self.m_obstacleWireframe, self.m_simDistance)
        QWidget.setTabOrder(self.m_simDistance, self.m_simCohesion)
        QWidget.setTabOrder(self.m_simCohesion, self.m_simSeparation)
        QWidget.setTabOrder(self.m_simSeparation, self.m_simAlignment)
        QWidget.setTabOrder(self.m_simAlignment, self.m_bboxSize)
        QWidget.setTabOrder(self.m_bboxSize, self.m_backColour)

        self.retranslateUi(MainWindow)

        self.toolBox.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.groupBox.setTitle(QCoreApplication.translate("MainWindow", u"General", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"Flock density:", None))
#if QT_CONFIG(tooltip)
        self.m_flockDensity.setToolTip(QCoreApplication.translate("MainWindow", u"Sets the total number of Boids in the Flock", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_flockDensity.setStatusTip(QCoreApplication.translate("MainWindow", u"Sets the total number of Boids in the Flock", None))
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(tooltip)
        self.m_applyFlock.setToolTip(QCoreApplication.translate("MainWindow", u"Set the new number of boids in the flock", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_applyFlock.setStatusTip(QCoreApplication.translate("MainWindow", u"Set the new number of boids in the flock", None))
#endif // QT_CONFIG(statustip)
        self.m_applyFlock.setText(QCoreApplication.translate("MainWindow", u"Apply", None))
        self.m_resetFlock.setText(QCoreApplication.translate("MainWindow", u"Reset Flock", None))
#if QT_CONFIG(tooltip)
        self.m_addBoids.setToolTip(QCoreApplication.translate("MainWindow", u"Adds 10 boids to the current flock", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_addBoids.setStatusTip(QCoreApplication.translate("MainWindow", u"Adds 10 boids to the current flock", None))
#endif // QT_CONFIG(statustip)
        self.m_addBoids.setText(QCoreApplication.translate("MainWindow", u"+ Add Boids", None))
#if QT_CONFIG(tooltip)
        self.m_removeBoids.setToolTip(QCoreApplication.translate("MainWindow", u"Removes 10 boids from the current flock", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_removeBoids.setStatusTip(QCoreApplication.translate("MainWindow", u"Removes 10 boids from the current flock", None))
#endif // QT_CONFIG(statustip)
        self.m_removeBoids.setText(QCoreApplication.translate("MainWindow", u"- Remove Boids", None))
        self.groupBox_2.setTitle(QCoreApplication.translate("MainWindow", u"Appearence", None))
#if QT_CONFIG(tooltip)
        self.m_changeFlockColour.setToolTip(QCoreApplication.translate("MainWindow", u"Change the flock colour", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_changeFlockColour.setStatusTip(QCoreApplication.translate("MainWindow", u"Change the flock colour", None))
#endif // QT_CONFIG(statustip)
        self.m_changeFlockColour.setText(QCoreApplication.translate("MainWindow", u"Change Flock Colour", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"Set Boid Size :", None))
#if QT_CONFIG(tooltip)
        self.m_changeBoidSize.setToolTip(QCoreApplication.translate("MainWindow", u"Changes the size of each boid in the flock", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_changeBoidSize.setStatusTip(QCoreApplication.translate("MainWindow", u"Changes the size of each boid in the flock", None))
#endif // QT_CONFIG(statustip)
        self.groupBox_4.setTitle(QCoreApplication.translate("MainWindow", u"Movement", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"Flock Speed:", None))
#if QT_CONFIG(tooltip)
        self.m_flockSpeedSlider.setToolTip(QCoreApplication.translate("MainWindow", u"Controls how fast the flock moves", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_flockSpeedSlider.setStatusTip(QCoreApplication.translate("MainWindow", u"Controls how fast the flock moves", None))
#endif // QT_CONFIG(statustip)
        self.m_flockSpeedLabel.setText(QCoreApplication.translate("MainWindow", u"100%", None))
#if QT_CONFIG(tooltip)
        self.m_renderFlockWireframe.setToolTip(QCoreApplication.translate("MainWindow", u"Renders the boids in wireframe mode if checked", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_renderFlockWireframe.setStatusTip(QCoreApplication.translate("MainWindow", u"Renders the boids in wireframe mode if checked", None))
#endif // QT_CONFIG(statustip)
        self.m_renderFlockWireframe.setText(QCoreApplication.translate("MainWindow", u"Render Wireframe", None))
        self.toolBox.setItemText(self.toolBox.indexOf(self.m_page1_flock), QCoreApplication.translate("MainWindow", u"Flock", None))
        self.groupBox_3.setTitle(QCoreApplication.translate("MainWindow", u"General", None))
#if QT_CONFIG(tooltip)
        self.m_enableObstacle.setToolTip(QCoreApplication.translate("MainWindow", u"Enable or disable the obstacle in the simulation", None))
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        self.m_enableObstacle.setStatusTip(QCoreApplication.translate("MainWindow", u"Enable or disable the obstacle in the simulation", None))
#endif // QT_CONFIG(statustip)
        self.m_enableObstacle.setText(QCoreApplication.translate("MainWindow", u"Enable Obstacle", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"Position:", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"Size:", None))
        self.groupBox_5.setTitle(QCoreApplication.translate("MainWindow", u"Appearence", None))
        self.m_obstacleColour.setText(QCoreApplication.translate("MainWindow", u"Change Colour", None))
        self.label_specular.setText(QCoreApplication.translate("MainWindow", u"Specular (RGB):", None))
        self.label_diffuse.setText(QCoreApplication.translate("MainWindow", u"Diffuse (RGB):", None))
        self.m_obstacleWireframe.setText(QCoreApplication.translate("MainWindow", u"Render Wireframe", None))
        self.groupBox_obstacleAvoidance.setTitle(QCoreApplication.translate("MainWindow", u"Obstacle Avoidance", None))
        self.label_avoidanceRadius.setText(QCoreApplication.translate("MainWindow", u"Avoidance Radius Scale:", None))
        self.m_obstacleAvoidanceRadiusValue.setText(QCoreApplication.translate("MainWindow", u"3.0", None))
        self.label_collisionRadius.setText(QCoreApplication.translate("MainWindow", u"Collision Radius Scale:", None))
        self.m_obstacleCollisionRadiusValue.setText(QCoreApplication.translate("MainWindow", u"1.3", None))
        self.label_repulsionForce.setText(QCoreApplication.translate("MainWindow", u"Repulsion Force:", None))
        self.m_obstacleRepulsionForceValue.setText(QCoreApplication.translate("MainWindow", u"0.45", None))
        self.toolBox.setItemText(self.toolBox.indexOf(self.m_page2_obstacle), QCoreApplication.translate("MainWindow", u"Obstacle", None))
        self.groupBox_7.setTitle(QCoreApplication.translate("MainWindow", u"General", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"Neighbourhood Distance :", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"Cohesion Force :", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"Separation Force :", None))
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"Alignment Weight :", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"Flock Distance", None))
        self.toolBox.setItemText(self.toolBox.indexOf(self.m_page3_simulation), QCoreApplication.translate("MainWindow", u"Simulation", None))
        self.groupBox_41.setTitle(QCoreApplication.translate("MainWindow", u"Bounding Box", None))
        self.label_41.setText(QCoreApplication.translate("MainWindow", u"Size :", None))
        self.groupBox_6.setTitle(QCoreApplication.translate("MainWindow", u"Appearence", None))
        self.m_backColour.setText(QCoreApplication.translate("MainWindow", u"Background Colour", None))
        self.toolBox.setItemText(self.toolBox.indexOf(self.m_page4_environment), QCoreApplication.translate("MainWindow", u"Environment", None))
    # retranslateUi

