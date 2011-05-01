/****************************************************************************
** Meta object code from reading C++ file 'AccelerationFilterWeight.h'
**
** Created: Sun May 1 00:15:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Navigation/Weight/AccelerationFilterWeight.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AccelerationFilterWeight.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AccelerationFilterWeight[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_AccelerationFilterWeight[] = {
    "AccelerationFilterWeight\0"
};

const QMetaObject AccelerationFilterWeight::staticMetaObject = {
    { &Weight::staticMetaObject, qt_meta_stringdata_AccelerationFilterWeight,
      qt_meta_data_AccelerationFilterWeight, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AccelerationFilterWeight::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AccelerationFilterWeight::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AccelerationFilterWeight::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AccelerationFilterWeight))
        return static_cast<void*>(const_cast< AccelerationFilterWeight*>(this));
    return Weight::qt_metacast(_clname);
}

int AccelerationFilterWeight::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Weight::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
