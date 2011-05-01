/****************************************************************************
** Meta object code from reading C++ file 'FadingCollisionMap.h'
**
** Created: Sun May 1 00:15:06 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Map/FadingCollisionMap.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FadingCollisionMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FadingCollisionMap[] = {

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
      48,   20,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FadingCollisionMap[] = {
    "FadingCollisionMap\0\0distance,angle,opacity,size\0"
    "registerObjectDetected(double,double,double,int)\0"
};

const QMetaObject FadingCollisionMap::staticMetaObject = {
    { &Map::staticMetaObject, qt_meta_stringdata_FadingCollisionMap,
      qt_meta_data_FadingCollisionMap, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FadingCollisionMap::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FadingCollisionMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FadingCollisionMap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FadingCollisionMap))
        return static_cast<void*>(const_cast< FadingCollisionMap*>(this));
    return Map::qt_metacast(_clname);
}

int FadingCollisionMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Map::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registerObjectDetected((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
