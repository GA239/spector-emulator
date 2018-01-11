#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T09:24:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spector-emulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG(debug, debug|release) {
    LIBS+= -L../spector-emulator-controll-widget/debug -lControllSEWidget
    LIBS+= -L../spector-emulator-plot-widget/debug -lPlotSEWidget

    LIBS+= -L../../../../spector-emulator/libs/libqt-searchwidget/debug -lsearchwidget
}

CONFIG(release, debug|release) {
    LIBS+= -L../spector-emulator-controll-widget/release -lControllSEWidget
    LIBS+= -L../spector-emulator-plot-widget/release -lPlotSEWidget

    LIBS+= -L../../../../spector-emulator/libs/libqt-searchwidget/release -lsearchwidget

}
