/****************************************************************************
** Meta object code from reading C++ file 'NavPathMapObject.h'
**
** Created: Sun May 1 00:15:12 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Map/MapObject/NavPathMapObject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NavPathMapObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NavPathMapObject[] = {

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
      22,   18,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NavPathMapObject[] = {
    "NavPathMapObject\0\0x,y\0addNavPoint(long,long)\0"
};

const QMetaObject NavPathMapObject::staticMetaObject = {
    { &MapObject::staticMetaObject, qt_meta_stringdata_NavPathMapObject,
      qt_meta_data_NavPathMapObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NavPathMapObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NavPathMapObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NavPathMapObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NavPathMapObject))
        return static_cast<void*>(const_cast< NavPathMapObject*>(this));
    return MapObject::qt_metacast(_clname);
}

int NavPathMapObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MapObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addNavPoint((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
