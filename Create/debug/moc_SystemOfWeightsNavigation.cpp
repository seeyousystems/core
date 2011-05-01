/****************************************************************************
** Meta object code from reading C++ file 'SystemOfWeightsNavigation.h'
**
** Created: Sun May 1 00:15:44 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Navigation/SystemOfWeightsNavigation.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SystemOfWeightsNavigation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SystemOfWeightsNavigation[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SystemOfWeightsNavigation[] = {
    "SystemOfWeightsNavigation\0\0weightsChanged()\0"
};

const QMetaObject SystemOfWeightsNavigation::staticMetaObject = {
    { &Navigation::staticMetaObject, qt_meta_stringdata_SystemOfWeightsNavigation,
      qt_meta_data_SystemOfWeightsNavigation, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SystemOfWeightsNavigation::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SystemOfWeightsNavigation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SystemOfWeightsNavigation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SystemOfWeightsNavigation))
        return static_cast<void*>(const_cast< SystemOfWeightsNavigation*>(this));
    return Navigation::qt_metacast(_clname);
}

int SystemOfWeightsNavigation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Navigation::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: weightsChanged(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SystemOfWeightsNavigation::weightsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
