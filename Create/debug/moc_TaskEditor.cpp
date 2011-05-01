/****************************************************************************
** Meta object code from reading C++ file 'TaskEditor.h'
**
** Created: Sat Apr 30 02:09:52 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/TaskEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TaskEditor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      40,   11,   11,   11, 0x08,
      51,   11,   11,   11, 0x08,
      72,   61,   11,   11, 0x08,
     108,  101,   11,   11, 0x0a,
     130,   11,   11,   11, 0x0a,
     156,  144,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TaskEditor[] = {
    "TaskEditor\0\0index\0showTask(QModelIndex)\0"
    "stopTask()\0newTask()\0task,title\0"
    "addTaskToList(Task*,QString)\0create\0"
    "registerCore(Create*)\0refreshList()\0"
    "taskManager\0setTaskManager(TaskManager*)\0"
};

const QMetaObject TaskEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TaskEditor,
      qt_meta_data_TaskEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TaskEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TaskEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TaskEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TaskEditor))
        return static_cast<void*>(const_cast< TaskEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int TaskEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showTask((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: stopTask(); break;
        case 2: newTask(); break;
        case 3: addTaskToList((*reinterpret_cast< Task*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: registerCore((*reinterpret_cast< Create*(*)>(_a[1]))); break;
        case 5: refreshList(); break;
        case 6: setTaskManager((*reinterpret_cast< TaskManager*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
