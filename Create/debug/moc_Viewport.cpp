/****************************************************************************
** Meta object code from reading C++ file 'Viewport.h'
**
** Created: Sun May 1 00:15:26 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Viewport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Viewport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Viewport[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      29,   22,    9,    9, 0x05,
      70,   51,    9,    9, 0x05,
     122,  113,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     146,  140,    9,    9, 0x0a,
     182,  174,    9,    9, 0x0a,
     202,    9,    9,    9, 0x0a,
     217,    9,    9,    9, 0x0a,
     243,  236,    9,    9, 0x0a,
     264,  236,    9,    9, 0x0a,
     292,  288,    9,    9, 0x0a,
     316,  288,    9,    9, 0x0a,
     337,    9,    9,    9, 0x0a,
     352,    9,    9,    9, 0x0a,
     366,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Viewport[] = {
    "Viewport\0\0leftClick()\0status\0"
    "statusUpdate(QString)\0viewport,value,x,y\0"
    "toolbarAction(Viewport*,QString,long,long)\0"
    "viewport\0closed(Viewport*)\0event\0"
    "canvasClicked(QMouseEvent*)\0painter\0"
    "drawMaps(QPainter*)\0paintAndShow()\0"
    "resizeScrollbars()\0action\0"
    "menuAction(QAction*)\0toolbarAction(QAction*)\0"
    "x,y\0focusOnPoint(long,long)\0"
    "autoFocus(long,long)\0resetToolbar()\0"
    "zoomChanged()\0setNaturalZoom()\0"
};

const QMetaObject Viewport::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Viewport,
      qt_meta_data_Viewport, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Viewport::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Viewport::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Viewport::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Viewport))
        return static_cast<void*>(const_cast< Viewport*>(this));
    return QWidget::qt_metacast(_clname);
}

int Viewport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: leftClick(); break;
        case 1: statusUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: toolbarAction((*reinterpret_cast< Viewport*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3])),(*reinterpret_cast< long(*)>(_a[4]))); break;
        case 3: closed((*reinterpret_cast< Viewport*(*)>(_a[1]))); break;
        case 4: canvasClicked((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: drawMaps((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 6: paintAndShow(); break;
        case 7: resizeScrollbars(); break;
        case 8: menuAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 9: toolbarAction((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 10: focusOnPoint((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 11: autoFocus((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 12: resetToolbar(); break;
        case 13: zoomChanged(); break;
        case 14: setNaturalZoom(); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void Viewport::leftClick()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Viewport::statusUpdate(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Viewport::toolbarAction(Viewport * _t1, QString _t2, long _t3, long _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Viewport::closed(Viewport * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
