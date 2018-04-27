#-------------------------------------------------
#
# Project created by QtCreator 2017-12-25T16:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = leadFire
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    model/bGeo.cpp \
    model/Robot.cpp \
    model/ScoutCar.cpp \
    qdraw.cpp \
    maindata.cpp \
    obmap.cpp \
    planner2d.cpp \
    connect/Agent.cpp \
    reeds_shepp.cpp \
    taskplanner.cpp \
    robotthread.cpp

HEADERS += \
        mainwindow.h \
    model/bGeo.hpp \
    model/Robot.h \
    model/ScoutCar.h \
    qdraw.h \
    maindata.h \
    obmap.h \
    stadfx.h \
    planner2d.h \
    connect/Agent.hpp \
    reeds_shepp.h \
    taskplanner.h \
    robotthread.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += $${PWD}/model

include (../common.pri)

message($${TARGET}  "- Include "    $${INCLUDEPATH})
message($${TARGET}  "- Linking "    $${LIBS})
message($${TARGET}  "- SourceD "    $${PWD})
message($${TARGET}  "- OutputD "    $${OUT_PWD})
message($${TARGET}  "- DestDir "    $${DESTDIR_BASE})
message($${TARGET} "- SOURCESs" $${SOURCES})
