/****************************************************************************
** Meta object code from reading C++ file 'JoystickView.h'
**
** Created: Wed Apr 13 11:55:26 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../JoystickView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JoystickView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_JoystickView[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   13,   13,   13, 0x0a,
      61,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_JoystickView[] = {
    "JoystickView\0\0yokeX,yokeY\0"
    "yokeChanged(double,double)\0paint()\0"
    "accelerate()\0"
};

const QMetaObject JoystickView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_JoystickView,
      qt_meta_data_JoystickView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &JoystickView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *JoystickView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *JoystickView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_JoystickView))
        return static_cast<void*>(const_cast< JoystickView*>(this));
    return QWidget::qt_metacast(_clname);
}

int JoystickView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: yokeChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: paint(); break;
        case 2: accelerate(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void JoystickView::yokeChanged(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
