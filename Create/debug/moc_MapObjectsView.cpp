/****************************************************************************
** Meta object code from reading C++ file 'MapObjectsView.h'
**
** Created: Sun May 1 00:15:02 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/MapObjectsView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapObjectsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapObjectsView[] = {

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
      23,   16,   15,   15, 0x08,
      45,   15,   15,   15, 0x08,
      70,   65,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MapObjectsView[] = {
    "MapObjectsView\0\0create\0registerCore(Create*)\0"
    "refreshMapObjects()\0item\0"
    "itemChanged(QListWidgetItem*)\0"
};

const QMetaObject MapObjectsView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapObjectsView,
      qt_meta_data_MapObjectsView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MapObjectsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MapObjectsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MapObjectsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapObjectsView))
        return static_cast<void*>(const_cast< MapObjectsView*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapObjectsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registerCore((*reinterpret_cast< Create*(*)>(_a[1]))); break;
        case 1: refreshMapObjects(); break;
        case 2: itemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
