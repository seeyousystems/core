/****************************************************************************
** Meta object code from reading C++ file 'FluidDriveController.h'
**
** Created: Thu Mar 24 15:18:38 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Controller/FluidDriveController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FluidDriveController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FluidDriveController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      34,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FluidDriveController[] = {
    "FluidDriveController\0\0yokeX,yokeY\0"
    "setYoke(double,double)\0"
};

const QMetaObject FluidDriveController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_FluidDriveController,
      qt_meta_data_FluidDriveController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FluidDriveController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FluidDriveController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FluidDriveController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FluidDriveController))
        return static_cast<void*>(const_cast< FluidDriveController*>(this));
    return Controller::qt_metacast(_clname);
}

int FluidDriveController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setYoke((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
