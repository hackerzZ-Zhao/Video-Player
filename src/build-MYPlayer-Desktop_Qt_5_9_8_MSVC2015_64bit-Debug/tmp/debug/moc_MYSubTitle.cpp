/****************************************************************************
** Meta object code from reading C++ file 'MYSubTitle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../MYPlayer/MYSubTitle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MYSubTitle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MYSubTitle_t {
    QByteArrayData data[5];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MYSubTitle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MYSubTitle_t qt_meta_stringdata_MYSubTitle = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MYSubTitle"
QT_MOC_LITERAL(1, 11, 13), // "SUBTITLE_TYPE"
QT_MOC_LITERAL(2, 25, 20), // "SUBTITLE_TYPE_UNKNOW"
QT_MOC_LITERAL(3, 46, 17), // "SUBTITLE_TYPE_SRT"
QT_MOC_LITERAL(4, 64, 18) // "SUBTITLE_TYPPE_SSA"

    },
    "MYSubTitle\0SUBTITLE_TYPE\0SUBTITLE_TYPE_UNKNOW\0"
    "SUBTITLE_TYPE_SRT\0SUBTITLE_TYPPE_SSA"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MYSubTitle[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    3,   18,

 // enum data: key, value
       2, uint(MYSubTitle::SUBTITLE_TYPE_UNKNOW),
       3, uint(MYSubTitle::SUBTITLE_TYPE_SRT),
       4, uint(MYSubTitle::SUBTITLE_TYPPE_SSA),

       0        // eod
};

void MYSubTitle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MYSubTitle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MYSubTitle.data,
      qt_meta_data_MYSubTitle,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MYSubTitle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MYSubTitle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MYSubTitle.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MYSubTitle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
