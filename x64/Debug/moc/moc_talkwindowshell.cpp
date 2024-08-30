/****************************************************************************
** Meta object code from reading C++ file 'talkwindowshell.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../talkwindowshell.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'talkwindowshell.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TaikWindowShell_t {
    QByteArrayData data[15];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TaikWindowShell_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TaikWindowShell_t qt_meta_stringdata_TaikWindowShell = {
    {
QT_MOC_LITERAL(0, 0, 15), // "TaikWindowShell"
QT_MOC_LITERAL(1, 16, 19), // "OnEmotionBtnClicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 16), // "UpdateSendTcpMsg"
QT_MOC_LITERAL(4, 54, 8), // "QString&"
QT_MOC_LITERAL(5, 63, 8), // "str_data"
QT_MOC_LITERAL(6, 72, 4), // "int&"
QT_MOC_LITERAL(7, 77, 8), // "msg_type"
QT_MOC_LITERAL(8, 86, 9), // "file_name"
QT_MOC_LITERAL(9, 96, 23), // "OnTalkWindowItemClicked"
QT_MOC_LITERAL(10, 120, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 137, 4), // "item"
QT_MOC_LITERAL(12, 142, 20), // "OnEmotionItemClicked"
QT_MOC_LITERAL(13, 163, 11), // "emotion_num"
QT_MOC_LITERAL(14, 175, 18) // "ProcessPendingData"

    },
    "TaikWindowShell\0OnEmotionBtnClicked\0"
    "\0UpdateSendTcpMsg\0QString&\0str_data\0"
    "int&\0msg_type\0file_name\0OnTalkWindowItemClicked\0"
    "QListWidgetItem*\0item\0OnEmotionItemClicked\0"
    "emotion_num\0ProcessPendingData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TaikWindowShell[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       3,    3,   47,    2, 0x0a /* Public */,
       3,    2,   54,    2, 0x2a /* Public | MethodCloned */,
       9,    1,   59,    2, 0x08 /* Private */,
      12,    1,   62,    2, 0x08 /* Private */,
      14,    0,   65,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, QMetaType::QString,    5,    7,    8,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    5,    7,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,

       0        // eod
};

void TaikWindowShell::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TaikWindowShell *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnEmotionBtnClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->UpdateSendTcpMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->UpdateSendTcpMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->OnTalkWindowItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->OnEmotionItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ProcessPendingData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TaikWindowShell::staticMetaObject = { {
    QMetaObject::SuperData::link<BasicWindow::staticMetaObject>(),
    qt_meta_stringdata_TaikWindowShell.data,
    qt_meta_data_TaikWindowShell,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TaikWindowShell::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaikWindowShell::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TaikWindowShell.stringdata0))
        return static_cast<void*>(this);
    return BasicWindow::qt_metacast(_clname);
}

int TaikWindowShell::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasicWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
