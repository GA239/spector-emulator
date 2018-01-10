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
    LIBS+= -L../../spector-emulator-controll-widget/build-spector-emulator-controll-widget-Desktop/debug -lControllSEWidget
    LIBS+= -L../../spector-emulator-plot-widget/build-spector-emulator-plot-widget-Desktop/debug -lPlotSEWidget
}

CONFIG(release, debug|release) {
    LIBS+= -L../../spector-emulator-controll-widget/build-spector-emulator-controll-widget-Desktop/release -lControllSEWidget
    LIBS+= -L../../spector-emulator-plot-widget/build-spector-emulator-plot-widget-Desktop/release -lPlotSEWidget
}
