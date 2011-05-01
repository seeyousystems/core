/****************************************************************************
** Meta object code from reading C++ file 'MapObject.h'
**
** Created: Sun May 1 00:15:16 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Map/MapObject/MapObject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapObject[] = {

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
      24,   11,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapObject[] = {
    "MapObject\0\0x,y,rotation\0move(long,long,double)\0"
};

const QMetaObject MapObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MapObject,
      qt_meta_data_MapObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapObject))
        return static_cast<void*>(const_cast< MapObject*>(this));
    return QObject::qt_metacast(_clname);
}

int MapObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: move((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
