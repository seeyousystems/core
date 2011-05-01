/****************************************************************************
** Meta object code from reading C++ file 'WeightEditor.h'
**
** Created: Sun May 1 00:15:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/WeightEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WeightEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WeightEditor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x0a,
      49,   43,   13,   13, 0x0a,
      73,   13,   13,   13, 0x0a,
      85,   13,   13,   13, 0x0a,
     100,   13,   13,   13, 0x0a,
     117,   13,   13,   13, 0x0a,
     132,   13,   13,   13, 0x0a,
     149,   13,   13,   13, 0x0a,
     174,  163,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WeightEditor[] = {
    "WeightEditor\0\0create\0registerCore(Create*)\0"
    "index\0showWeight(QModelIndex)\0addWeight()\0"
    "deleteWeight()\0activateWeight()\0"
    "moveWeightUp()\0moveWeightDown()\0"
    "refreshList()\0navigation\0"
    "setNavigation(Navigation*)\0"
};

const QMetaObject WeightEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_WeightEditor,
      qt_meta_data_WeightEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WeightEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WeightEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WeightEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WeightEditor))
        return static_cast<void*>(const_cast< WeightEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int WeightEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registerCore((*reinterpret_cast< Create*(*)>(_a[1]))); break;
        case 1: showWeight((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: addWeight(); break;
        case 3: deleteWeight(); break;
        case 4: activateWeight(); break;
        case 5: moveWeightUp(); break;
        case 6: moveWeightDown(); break;
        case 7: refreshList(); break;
        case 8: setNavigation((*reinterpret_cast< Navigation*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
