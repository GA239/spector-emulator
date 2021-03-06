#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T22:04:14
#
#-------------------------------------------------

QT       += core gui widgets sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ControllSEWidget
TEMPLATE = lib


SOURCES += \ 
    controllsewidget.cpp

HEADERS  += \ 
    controllsewidget.h \

CONFIG(debug, debug|release) {

    LIBS+= -L../../../spector-emulator/libs/libqt-searchwidget/debug -lsearchwidget
}

CONFIG(release, debug|release) {

    LIBS+= -L../../../spector-emulator/libs/libqt-searchwidget/release -lsearchwidget

}
