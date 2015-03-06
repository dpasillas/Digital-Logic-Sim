# -------------------------------------------------
# Project created by QtCreator 2011-03-04T14:44:05
# -------------------------------------------------
QT += opengl gui declarative widgets
TARGET = LogicSim
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    circuitboard.cpp \
    connection.cpp \
    binarytree.cpp \
    node.cpp \
    logicgate.cpp \
    pin.cpp \
    namelabel.cpp \
    logicsource.cpp \
    logicevent.cpp \
    bulbcomponent.cpp \
    compositeitem.cpp \
    led_component.cpp
HEADERS += mainwindow.h \
    circuitboard.h \
    connection.h \
    component.h \
    binarytree.h \
    node.h \
    binarysearchtree.h \
    logicgate.h \
    direction.h \
    pin.h \
    logicstate.h \
    namelabel.h \
    logicevent.h \
    logicsource.h \
    operatingcomponent.h \
    bulbcomponent.h \
    gradientitem.h \
    compositeitem.h \
    led_component.h
FORMS += mainwindow.ui
RESOURCES += resources.qrc
RC_FILE = LogicSim.rc
