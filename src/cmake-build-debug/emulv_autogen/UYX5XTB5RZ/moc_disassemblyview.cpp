/****************************************************************************
** Meta object code from reading C++ file 'disassemblyview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/disassemblyview.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'disassemblyview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_DisassemblyView_t {
    uint offsetsAndSizes[14];
    char stringdata0[16];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[21];
    char stringdata5[5];
    char stringdata6[3];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_DisassemblyView_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_DisassemblyView_t qt_meta_stringdata_DisassemblyView = {
    {
        QT_MOC_LITERAL(0, 15),  // "DisassemblyView"
        QT_MOC_LITERAL(16, 25),  // "updateLineNumberAreaWidth"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 13),  // "newBlockCount"
        QT_MOC_LITERAL(57, 20),  // "updateLineNumberArea"
        QT_MOC_LITERAL(78, 4),  // "rect"
        QT_MOC_LITERAL(83, 2)   // "dy"
    },
    "DisassemblyView",
    "updateLineNumberAreaWidth",
    "",
    "newBlockCount",
    "updateLineNumberArea",
    "rect",
    "dy"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_DisassemblyView[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x08,    1 /* Private */,
       4,    2,   29,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QRect, QMetaType::Int,    5,    6,

       0        // eod
};

Q_CONSTINIT const QMetaObject DisassemblyView::staticMetaObject = { {
    QMetaObject::SuperData::link<QPlainTextEdit::staticMetaObject>(),
    qt_meta_stringdata_DisassemblyView.offsetsAndSizes,
    qt_meta_data_DisassemblyView,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_DisassemblyView_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DisassemblyView, std::true_type>,
        // method 'updateLineNumberAreaWidth'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateLineNumberArea'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QRect &, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void DisassemblyView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DisassemblyView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateLineNumberAreaWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->updateLineNumberArea((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *DisassemblyView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DisassemblyView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DisassemblyView.stringdata0))
        return static_cast<void*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int DisassemblyView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
