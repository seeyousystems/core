/****************************************************************************
** Meta object code from reading C++ file 'TaskManager.h'
**
** Created: Wed Apr 13 11:55:32 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Task/TaskManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskManager[] = {

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

static const char qt_meta_stringdata_TaskManager[] = {
    "TaskManager\0"
};

const QMetaObject TaskManager::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_TaskManager,
      qt_meta_data_TaskManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskManager))
        return static_cast<void*>(const_cast< TaskManager*>(this));
    return QThread::qt_metacast(_clname);
}

int TaskManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
