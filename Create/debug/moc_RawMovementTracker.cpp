/****************************************************************************
** Meta object code from reading C++ file 'RawMovementTracker.h'
**
** Created: Thu Mar 24 15:18:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MovementTracker/RawMovementTracker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RawMovementTracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RawMovementTracker[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x0a,
      65,   59,   19,   19, 0x0a,
      94,   19,   19,   19, 0x0a,
     129,  114,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RawMovementTracker[] = {
    "RawMovementTracker\0\0distance\0"
    "registerMovedDistance(double)\0angle\0"
    "registerChangedAngle(double)\0"
    "registerCollision()\0distance,angle\0"
    "registerObjectDetected(double,double)\0"
};

const QMetaObject RawMovementTracker::staticMetaObject = {
    { &MovementTracker::staticMetaObject, qt_meta_stringdata_RawMovementTracker,
      qt_meta_data_RawMovementTracker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RawMovementTracker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RawMovementTracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RawMovementTracker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RawMovementTracker))
        return static_cast<void*>(const_cast< RawMovementTracker*>(this));
    return MovementTracker::qt_metacast(_clname);
}

int RawMovementTracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MovementTracker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registerMovedDistance((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: registerChangedAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: registerCollision(); break;
        case 3: registerObjectDetected((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
