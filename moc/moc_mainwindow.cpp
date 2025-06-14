/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue Jul 10 15:58:09 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      53,   11,   11,   11, 0x08,
      79,   11,   11,   11, 0x08,
     105,   11,   11,   11, 0x08,
     129,   11,   11,   11, 0x08,
     156,   12,   11,   11, 0x08,
     197,   11,   11,   11, 0x08,
     238,  230,   11,   11, 0x08,
     278,   12,   11,   11, 0x08,
     317,   12,   11,   11, 0x08,
     356,   12,   11,   11, 0x08,
     395,   12,   11,   11, 0x08,
     434,   11,   11,   11, 0x08,
     464,  230,   11,   11, 0x08,
     501,   12,   11,   11, 0x08,
     539,   12,   11,   11, 0x08,
     582,   12,   11,   11, 0x08,
     620,   12,   11,   11, 0x08,
     660,   12,   11,   11, 0x08,
     699,   11,   11,   11, 0x08,
     725,   12,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0arg1\0on_m_flockDensity_valueChanged(int)\0"
    "on_m_resetFlock_clicked()\0"
    "on_m_applyFlock_clicked()\0"
    "on_m_addBoids_clicked()\0"
    "on_m_removeBoids_clicked()\0"
    "on_m_changeBoidSize_valueChanged(double)\0"
    "on_m_changeFlockColour_clicked()\0"
    "checked\0on_m_renderFlockWireframe_toggled(bool)\0"
    "on_m_obstaclePosX_valueChanged(double)\0"
    "on_m_obstaclePosY_valueChanged(double)\0"
    "on_m_obstaclePosZ_valueChanged(double)\0"
    "on_m_obstacleSize_valueChanged(double)\0"
    "on_m_obstacleColour_clicked()\0"
    "on_m_obstacleWireframe_toggled(bool)\0"
    "on_m_simDistance_valueChanged(double)\0"
    "on_m_simFlockDistance_valueChanged(double)\0"
    "on_m_simCohesion_valueChanged(double)\0"
    "on_m_simSeparation_valueChanged(double)\0"
    "on_m_simAlignment_valueChanged(double)\0"
    "on_m_backColour_clicked()\0"
    "on_m_bboxSize_valueChanged(double)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_m_flockDensity_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_m_resetFlock_clicked(); break;
        case 2: on_m_applyFlock_clicked(); break;
        case 3: on_m_addBoids_clicked(); break;
        case 4: on_m_removeBoids_clicked(); break;
        case 5: on_m_changeBoidSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: on_m_changeFlockColour_clicked(); break;
        case 7: on_m_renderFlockWireframe_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: on_m_obstaclePosX_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: on_m_obstaclePosY_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: on_m_obstaclePosZ_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: on_m_obstacleSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: on_m_obstacleColour_clicked(); break;
        case 13: on_m_obstacleWireframe_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: on_m_simDistance_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: on_m_simFlockDistance_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: on_m_simCohesion_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: on_m_simSeparation_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 18: on_m_simAlignment_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: on_m_backColour_clicked(); break;
        case 20: on_m_bboxSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
