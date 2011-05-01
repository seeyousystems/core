/****************************************************************************
** Meta object code from reading C++ file 'create.h'
**
** Created: Sun May 1 00:16:15 2011
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
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,
      36,    7,    7,    7, 0x05,
      48,    7,    7,    7, 0x05,
      66,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      92,    7,   87,    7, 0x0a,
     105,    7,    7,    7, 0x0a,
     113,    7,    7,    7, 0x0a,
     120,    7,    7,    7, 0x0a,
     128,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Create[] = {
    "Create\0\0x,y\0focusOnPoint(long,long)\0"
    "coreReset()\0createConnected()\0"
    "createDisconnected()\0bool\0disconnect()\0"
    "start()\0stop()\0abort()\0reset()\0"
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
        case 2: createConnected(); break;
        case 3: createDisconnected(); break;
        case 4: { bool _r = disconnect();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: start(); break;
        case 6: stop(); break;
        case 7: abort(); break;
        case 8: reset(); break;
        default: ;
        }
        _id -= 9;
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

// SIGNAL 2
void Create::createConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Create::createDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
