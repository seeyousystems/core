/****************************************************************************
** Meta object code from reading C++ file 'create.h'
**
** Created: Wed Apr 13 11:55:36 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../create.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'create.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Create[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,
      36,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   48,    7,    7, 0x0a,
      90,    8,    7,    7, 0x0a,
     115,    8,    7,    7, 0x0a,
     146,    7,  141,    7, 0x0a,
     159,    7,    7,    7, 0x0a,
     165,    7,    7,    7, 0x0a,
     172,    7,    7,    7, 0x0a,
     180,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Create[] = {
    "Create\0\0x,y\0focusOnPoint(long,long)\0"
    "coreReset()\0x,y,rotation\0"
    "robotMoved(long,long,double)\0"
    "robotCollided(long,long)\0"
    "objectDetected(long,long)\0bool\0"
    "disconnect()\0run()\0stop()\0abort()\0"
    "reset()\0"
};

const QMetaObject Create::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Create,
      qt_meta_data_Create, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Create::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Create::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Create::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Create))
        return static_cast<void*>(const_cast< Create*>(this));
    return QObject::qt_metacast(_clname);
}

int Create::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: focusOnPoint((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 1: coreReset(); break;
        case 2: robotMoved((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 3: robotCollided((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 4: objectDetected((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 5: { bool _r = disconnect();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: run(); break;
        case 7: stop(); break;
        case 8: abort(); break;
        case 9: reset(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Create::focusOnPoint(long _t1, long _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Create::coreReset()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
