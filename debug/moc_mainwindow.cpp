/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Dec 19 22:47:21 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      31,   11,   11,   11, 0x0a,
      51,   49,   11,   11, 0x0a,
      65,   11,   11,   11, 0x0a,
      76,   11,   11,   11, 0x0a,
      83,   11,   11,   11, 0x0a,
      94,   11,   11,   11, 0x0a,
     102,   11,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     142,   11,   11,   11, 0x0a,
     160,   49,   11,   11, 0x0a,
     182,   11,   11,   11, 0x0a,
     196,   11,   11,   11, 0x0a,
     203,   11,   11,   11, 0x0a,
     217,   11,   11,   11, 0x0a,
     231,   11,   11,   11, 0x0a,
     238,   11,   11,   11, 0x0a,
     246,   11,   11,   11, 0x0a,
     252,   11,   11,   11, 0x0a,
     258,   11,   11,   11, 0x0a,
     270,   11,   11,   11, 0x08,
     280,   11,   11,   11, 0x08,
     287,   11,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,
     303,   11,   11,   11, 0x08,
     320,   11,   11,   11, 0x08,
     328,   11,   11,   11, 0x08,
     343,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0setSelectionMode()\0"
    "setHandDragMode()\0b\0runFunc(bool)\0"
    "stopFunc()\0step()\0stepNext()\0reset()\0"
    "setComponentCreationMode()\0setCCM_Int()\0"
    "setCCM_SmallInt()\0setViewProperty(bool)\0"
    "fitToWindow()\0zoom()\0setInterval()\0"
    "setStepSize()\0copy()\0paste()\0cut()\0"
    "del()\0selectAll()\0newFile()\0open()\0"
    "save()\0saveAs()\0openRecentFile()\0"
    "about()\0importAction()\0exportAction()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setSelectionMode(); break;
        case 1: setHandDragMode(); break;
        case 2: runFunc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: stopFunc(); break;
        case 4: step(); break;
        case 5: stepNext(); break;
        case 6: reset(); break;
        case 7: setComponentCreationMode(); break;
        case 8: setCCM_Int(); break;
        case 9: setCCM_SmallInt(); break;
        case 10: setViewProperty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: fitToWindow(); break;
        case 12: zoom(); break;
        case 13: setInterval(); break;
        case 14: setStepSize(); break;
        case 15: copy(); break;
        case 16: paste(); break;
        case 17: cut(); break;
        case 18: del(); break;
        case 19: selectAll(); break;
        case 20: newFile(); break;
        case 21: open(); break;
        case 22: save(); break;
        case 23: saveAs(); break;
        case 24: openRecentFile(); break;
        case 25: about(); break;
        case 26: importAction(); break;
        case 27: exportAction(); break;
        default: ;
        }
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
