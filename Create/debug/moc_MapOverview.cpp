/****************************************************************************
** Meta object code from reading C++ file 'MapOverview.h'
**
** Created: Sun May 1 00:15:03 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/MapOverview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapOverview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapOverview[] = {

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
      21,   13,   12,   12, 0x0a,
      45,   12,   12,   12, 0x0a,
      63,   12,   12,   12, 0x0a,
      87,   80,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MapOverview[] = {
    "MapOverview\0\0painter\0drawOverview(QPainter*)\0"
    "compileOverview()\0compileAndShow()\0"
    "action\0menuAction(QAction*)\0"
};

const QMetaObject MapOverview::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapOverview,
      qt_meta_data_MapOverview, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapOverview::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapOverview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapOverview::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapOverview))
        return static_cast<void*>(const_cast< MapOverview*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapOverview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: drawOverview((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 1: compileOverview(); break;
        case 2: compileAndShow(); break;
        case 3: menuAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
