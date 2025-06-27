/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/mainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "on_m_flockDensity_valueChanged",
        "",
        "arg1",
        "on_m_resetFlock_clicked",
        "on_m_applyFlock_clicked",
        "on_m_addBoids_clicked",
        "on_m_removeBoids_clicked",
        "on_m_changeBoidSize_valueChanged",
        "on_m_changeFlockColour_clicked",
        "on_m_renderFlockWireframe_toggled",
        "checked",
        "on_m_obstaclePosX_valueChanged",
        "on_m_obstaclePosY_valueChanged",
        "on_m_obstaclePosZ_valueChanged",
        "on_m_obstacleSize_valueChanged",
        "on_m_obstacleColour_clicked",
        "on_m_obstacleWireframe_toggled",
        "on_m_simDistance_valueChanged",
        "on_m_simFlockDistance_valueChanged",
        "on_m_simCohesion_valueChanged",
        "on_m_simSeparation_valueChanged",
        "on_m_simAlignment_valueChanged",
        "on_m_backColour_clicked",
        "on_m_bboxSize_valueChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_m_flockDensity_valueChanged'
        QtMocHelpers::SlotData<void(int)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'on_m_resetFlock_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_applyFlock_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_addBoids_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_removeBoids_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_changeBoidSize_valueChanged'
        QtMocHelpers::SlotData<void(double)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_changeFlockColour_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_renderFlockWireframe_toggled'
        QtMocHelpers::SlotData<void(bool)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 11 },
        }}),
        // Slot 'on_m_obstaclePosX_valueChanged'
        QtMocHelpers::SlotData<void(double)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_obstaclePosY_valueChanged'
        QtMocHelpers::SlotData<void(double)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_obstaclePosZ_valueChanged'
        QtMocHelpers::SlotData<void(double)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_obstacleSize_valueChanged'
        QtMocHelpers::SlotData<void(double)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_obstacleColour_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_obstacleWireframe_toggled'
        QtMocHelpers::SlotData<void(bool)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 11 },
        }}),
        // Slot 'on_m_simDistance_valueChanged'
        QtMocHelpers::SlotData<void(double)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_simFlockDistance_valueChanged'
        QtMocHelpers::SlotData<void(double)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_simCohesion_valueChanged'
        QtMocHelpers::SlotData<void(double)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_simSeparation_valueChanged'
        QtMocHelpers::SlotData<void(double)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_simAlignment_valueChanged'
        QtMocHelpers::SlotData<void(double)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
        // Slot 'on_m_backColour_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_m_bboxSize_valueChanged'
        QtMocHelpers::SlotData<void(double)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_m_flockDensity_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->on_m_resetFlock_clicked(); break;
        case 2: _t->on_m_applyFlock_clicked(); break;
        case 3: _t->on_m_addBoids_clicked(); break;
        case 4: _t->on_m_removeBoids_clicked(); break;
        case 5: _t->on_m_changeBoidSize_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->on_m_changeFlockColour_clicked(); break;
        case 7: _t->on_m_renderFlockWireframe_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->on_m_obstaclePosX_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 9: _t->on_m_obstaclePosY_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 10: _t->on_m_obstaclePosZ_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->on_m_obstacleSize_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 12: _t->on_m_obstacleColour_clicked(); break;
        case 13: _t->on_m_obstacleWireframe_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->on_m_simDistance_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 15: _t->on_m_simFlockDistance_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 16: _t->on_m_simCohesion_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 17: _t->on_m_simSeparation_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 18: _t->on_m_simAlignment_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 19: _t->on_m_backColour_clicked(); break;
        case 20: _t->on_m_bboxSize_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
