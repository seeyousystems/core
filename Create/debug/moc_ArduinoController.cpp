/****************************************************************************
** Meta object code from reading C++ file 'ArduinoController.h'
**
** Created: Wed Apr 13 11:55:21 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Controller/ArduinoController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ArduinoController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ArduinoController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   19,   18,   18, 0x05,
      75,   70,   18,   18, 0x05,
     107,   97,   18,   18, 0x05,
     140,  134,   18,   18, 0x05,
     174,  163,   18,   18, 0x05,
     208,  202,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ArduinoController[] = {
    "ArduinoController\0\0distance,angle\0"
    "signalObjectDetected(double,double)\0"
    "left\0leftSensorSignal(int)\0upperLeft\0"
    "upperLeftSensorSignal(int)\0front\0"
    "frontSensorSignal(int)\0upperRight\0"
    "upperRightSensorSignal(int)\0right\0"
    "rightSensorSignal(int)\0"
};

const QMetaObject ArduinoController::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ArduinoController,
      qt_meta_data_ArduinoController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ArduinoController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ArduinoController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ArduinoController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ArduinoController))
        return static_cast<void*>(const_cast< ArduinoController*>(this));
    return QThread::qt_metacast(_clname);
}

int ArduinoController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signalObjectDetected((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: leftSensorSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: upperLeftSensorSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: frontSensorSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: upperRightSensorSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: rightSensorSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ArduinoController::signalObjectDetected(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArduinoController::leftSensorSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ArduinoController::upperLeftSensorSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ArduinoController::frontSensorSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ArduinoController::upperRightSensorSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ArduinoController::rightSensorSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
