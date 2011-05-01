/****************************************************************************
** Meta object code from reading C++ file 'SeeYouController.h'
**
** Created: Sun May 1 00:16:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Controller/SeeYouController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SeeYouController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SeeYouController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   18,   17,   17, 0x0a,
      59,   53,   17,   17, 0x0a,
      73,   53,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SeeYouController[] = {
    "SeeYouController\0\0yokeX,yokeY\0"
    "setYoke(double,double)\0value\0setSpeed(int)\0"
    "setAngle(int)\0"
};

const QMetaObject SeeYouController::staticMetaObject = {
    { &Controller::staticMetaObject, qt_meta_stringdata_SeeYouController,
      qt_meta_data_SeeYouController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SeeYouController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SeeYouController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SeeYouController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SeeYouController))
        return static_cast<void*>(const_cast< SeeYouController*>(this));
    return Controller::qt_metacast(_clname);
}

int SeeYouController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Controller::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setYoke((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: setSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setAngle((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
