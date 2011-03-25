/****************************************************************************
** Meta object code from reading C++ file 'BlockDriveController.h'
**
** Created: Thu Mar 24 15:18:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Controller/BlockDriveController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlockDriveController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BlockDriveController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x0a,
      36,   21,   21,   21, 0x0a,
      51,   21,   21,   21, 0x0a,
      62,   21,   21,   21, 0x0a,
      80,   74,   21,   21, 0x0a,
      94,   74,   21,   21, 0x0a,
     108,   74,   21,   21, 0x0a,
     130,  125,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BlockDriveController[] = {
    "BlockDriveController\0\0moveForward()\0"
    "moveBackward()\0turnLeft()\0turnRight()\0"
    "value\0setSpeed(int)\0setAngle(int)\0"
    "setDistance(int)\0mode\0setSlowStartMode(QString)\0"
};

const QMetaObject BlockDriveController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_BlockDriveController,
      qt_meta_data_BlockDriveController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BlockDriveController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BlockDriveController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BlockDriveController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlockDriveController))
        return static_cast<void*>(const_cast< BlockDriveController*>(this));
    return Controller::qt_metacast(_clname);
}

int BlockDriveController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: moveForward(); break;
        case 1: moveBackward(); break;
        case 2: turnLeft(); break;
        case 3: turnRight(); break;
        case 4: setSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setAngle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setDistance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: setSlowStartMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
