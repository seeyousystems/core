/****************************************************************************
** Meta object code from reading C++ file 'HeatMap.h'
**
** Created: Sun May 1 00:15:22 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Map/HeatMap.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HeatMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HeatMap[] = {

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
      21,    9,    8,    8, 0x0a,
      91,   66,    8,    8, 0x0a,
     193,  148,    8,    8, 0x0a,
     289,  261,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HeatMap[] = {
    "HeatMap\0\0channel,x,y\0"
    "registerHeat(ColorMap::MapChannel,long,long)\0"
    "channel,x,y,opacity,size\0"
    "registerHeat(ColorMap::MapChannel,long,long,double,long)\0"
    "channel,position,distance,angle,opacity,size\0"
    "registerHeat(ColorMap::MapChannel,Trafo2D,double,double,double,int)\0"
    "distance,angle,opacity,size\0"
    "registerCollisionAtTrackerLocation(double,double,double,int)\0"
};

const QMetaObject HeatMap::staticMetaObject = {
    { &ColorMap::staticMetaObject, qt_meta_stringdata_HeatMap,
      qt_meta_data_HeatMap, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HeatMap::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HeatMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HeatMap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HeatMap))
        return static_cast<void*>(const_cast< HeatMap*>(this));
    return ColorMap::qt_metacast(_clname);
}

int HeatMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ColorMap::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registerHeat((*reinterpret_cast< ColorMap::MapChannel(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3]))); break;
        case 1: registerHeat((*reinterpret_cast< ColorMap::MapChannel(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< long(*)>(_a[5]))); break;
        case 2: registerHeat((*reinterpret_cast< ColorMap::MapChannel(*)>(_a[1])),(*reinterpret_cast< Trafo2D(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 3: registerCollisionAtTrackerLocation((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
