/****************************************************************************
** Meta object code from reading C++ file 'videocapturecontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/videocapturecontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videocapturecontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoCaptureController_t {
    QByteArrayData data[19];
    char stringdata0[249];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoCaptureController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoCaptureController_t qt_meta_stringdata_VideoCaptureController = {
    {
QT_MOC_LITERAL(0, 0, 22), // "VideoCaptureController"
QT_MOC_LITERAL(1, 23, 24), // "finishedVideoQuerySignal"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 10), // "resultCode"
QT_MOC_LITERAL(4, 60, 15), // "QList<VideoDao>"
QT_MOC_LITERAL(5, 76, 9), // "videoList"
QT_MOC_LITERAL(6, 86, 17), // "finishVideoSignal"
QT_MOC_LITERAL(7, 104, 7), // "message"
QT_MOC_LITERAL(8, 112, 19), // "errorOccurredSignal"
QT_MOC_LITERAL(9, 132, 9), // "sqlResult"
QT_MOC_LITERAL(10, 142, 10), // "dateSignal"
QT_MOC_LITERAL(11, 153, 12), // "selectedDate"
QT_MOC_LITERAL(12, 166, 4), // "page"
QT_MOC_LITERAL(13, 171, 19), // "getVideoMessageSlot"
QT_MOC_LITERAL(14, 191, 9), // "videoDate"
QT_MOC_LITERAL(15, 201, 3), // "row"
QT_MOC_LITERAL(16, 205, 20), // "sendVideoMessageSlot"
QT_MOC_LITERAL(17, 226, 9), // "SQLRESULT"
QT_MOC_LITERAL(18, 236, 12) // "videoMessage"

    },
    "VideoCaptureController\0finishedVideoQuerySignal\0"
    "\0resultCode\0QList<VideoDao>\0videoList\0"
    "finishVideoSignal\0message\0errorOccurredSignal\0"
    "sqlResult\0dateSignal\0selectedDate\0"
    "page\0getVideoMessageSlot\0videoDate\0"
    "row\0sendVideoMessageSlot\0SQLRESULT\0"
    "videoMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoCaptureController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       6,    2,   49,    2, 0x06 /* Public */,
       8,    1,   54,    2, 0x06 /* Public */,
      10,    2,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    2,   62,    2, 0x08 /* Private */,
      16,    2,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    7,    5,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QDate, QMetaType::Int,   11,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::QDate, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,   17,   18,

       0        // eod
};

void VideoCaptureController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoCaptureController *_t = static_cast<VideoCaptureController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finishedVideoQuerySignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<VideoDao>(*)>(_a[2]))); break;
        case 1: _t->finishVideoSignal((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QList<VideoDao>(*)>(_a[2]))); break;
        case 2: _t->errorOccurredSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->dateSignal((*reinterpret_cast< QDate(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->getVideoMessageSlot((*reinterpret_cast< QDate(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->sendVideoMessageSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QList<VideoDao>(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoCaptureController::*_t)(int , const QList<VideoDao> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCaptureController::finishedVideoQuerySignal)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoCaptureController::*_t)(const QString & , const QList<VideoDao> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCaptureController::finishVideoSignal)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoCaptureController::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCaptureController::errorOccurredSignal)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoCaptureController::*_t)(QDate , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoCaptureController::dateSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject VideoCaptureController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VideoCaptureController.data,
      qt_meta_data_VideoCaptureController,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoCaptureController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoCaptureController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoCaptureController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoCaptureController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void VideoCaptureController::finishedVideoQuerySignal(int _t1, const QList<VideoDao> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoCaptureController::finishVideoSignal(const QString & _t1, const QList<VideoDao> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoCaptureController::errorOccurredSignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoCaptureController::dateSignal(QDate _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
