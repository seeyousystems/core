/****************************************************************************
** Meta object code from reading C++ file 'MovementTracker.h'
**
** Created: Wed Apr 13 11:55:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MovementTracker/MovementTracker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MovementTracker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MovementTracker[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   17,   16,   16, 0x05,
      58,   54,   16,   16, 0x05,
      79,   54,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     114,  105,   16,   16, 0x0a,
     150,  144,   16,   16, 0x0a,
     179,   16,   16,   16, 0x0a,
     214,  199,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MovementTracker[] = {
    "MovementTracker\0\0x,y,rotation\0"
    "moved(long,long,double)\0x,y\0"
    "collision(long,long)\0objectDetected(long,long)\0"
    "distance\0registerMovedDistance(double)\0"
    "angle\0registerChangedAngle(double)\0"
    "registerCollision()\0distance,angle\0"
    "registerObjectDetected(double,double)\0"
};

const QMetaObject MovementTracker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MovementTracker,
      qt_meta_data_MovementTracker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MovementTracker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MovementTracker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MovementTracker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MovementTracker))
        return static_cast<void*>(const_cast< MovementTracker*>(this));
    return QObject::qt_metacast(_clname);
}

int MovementTracker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: moved((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 1: collision((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 2: objectDetected((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 3: registerMovedDistance((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: registerChangedAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: registerCollision(); break;
        case 6: registerObjectDetected((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MovementTracker::moved(long _t1, long _t2, double _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MovementTracker::collision(long _t1, long _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MovementTracker::objectDetected(long _t1, long _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
