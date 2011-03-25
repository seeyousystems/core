/****************************************************************************
** Meta object code from reading C++ file 'Controller.h'
**
** Created: Thu Mar 24 15:18:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Controller/Controller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Controller[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      55,   49,   11,   11, 0x05,
      82,   11,   11,   11, 0x05,
     115,  100,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Controller[] = {
    "Controller\0\0distance\0signalMovedDistance(double)\0"
    "angle\0signalChangedAngle(double)\0"
    "signalCollision()\0distance,angle\0"
    "signalObjectDetected(double,double)\0"
};

const QMetaObject Controller::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Controller,
      qt_meta_data_Controller, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Controller::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Controller))
        return static_cast<void*>(const_cast< Controller*>(this));
    return QThread::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalMovedDistance((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: signalChangedAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: signalCollision(); break;
        case 3: signalObjectDetected((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Controller::signalMovedDistance(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Controller::signalChangedAngle(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Controller::signalCollision()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Controller::signalObjectDetected(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
