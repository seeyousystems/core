/****************************************************************************
** Meta object code from reading C++ file 'Network.h'
**
** Created: Sun May 1 00:15:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Network/Network.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Network.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Network[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,    9,    8,    8, 0x05,
      58,    9,    8,    8, 0x05,
      95,    8,    8,    8, 0x05,
     113,    8,    8,    8, 0x05,
     132,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Network[] = {
    "Network\0\0message\0"
    "messageReceived(RemoteInterfaceMessage*)\0"
    "messageSent(RemoteInterfaceMessage*)\0"
    "messageReceived()\0hostUnsubscribed()\0"
    "hostSubscribed()\0"
};

const QMetaObject Network::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Network,
      qt_meta_data_Network, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Network::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Network::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Network::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Network))
        return static_cast<void*>(const_cast< Network*>(this));
    return QThread::qt_metacast(_clname);
}

int Network::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: messageReceived((*reinterpret_cast< RemoteInterfaceMessage*(*)>(_a[1]))); break;
        case 1: messageSent((*reinterpret_cast< RemoteInterfaceMessage*(*)>(_a[1]))); break;
        case 2: messageReceived(); break;
        case 3: hostUnsubscribed(); break;
        case 4: hostSubscribed(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Network::messageReceived(RemoteInterfaceMessage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Network::messageSent(RemoteInterfaceMessage * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Network::messageReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Network::hostUnsubscribed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Network::hostSubscribed()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
