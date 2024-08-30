/****************************************************************************
** Meta object code from reading C++ file 'qmsgtextedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../qmsgtextedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmsgtextedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QmsgTextEdit_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QmsgTextEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QmsgTextEdit_t qt_meta_stringdata_QmsgTextEdit = {
    {
QT_MOC_LITERAL(0, 0, 12), // "QmsgTextEdit"
QT_MOC_LITERAL(1, 13, 25), // "OnEmotionImageFrameChange"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "frame"
QT_MOC_LITERAL(4, 46, 13), // "AddEmotionUrl"
QT_MOC_LITERAL(5, 60, 11), // "emotion_num"
QT_MOC_LITERAL(6, 72, 21) // "DeleteAllEmotionImage"

    },
    "QmsgTextEdit\0OnEmotionImageFrameChange\0"
    "\0frame\0AddEmotionUrl\0emotion_num\0"
    "DeleteAllEmotionImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QmsgTextEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       6,    0,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

       0        // eod
};

void QmsgTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QmsgTextEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnEmotionImageFrameChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->AddEmotionUrl((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->DeleteAllEmotionImage(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QmsgTextEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QTextEdit::staticMetaObject>(),
    qt_meta_stringdata_QmsgTextEdit.data,
    qt_meta_data_QmsgTextEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QmsgTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QmsgTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QmsgTextEdit.stringdata0))
        return static_cast<void*>(this);
    return QTextEdit::qt_metacast(_clname);
}

int QmsgTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
