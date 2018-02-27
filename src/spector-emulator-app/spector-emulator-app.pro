#-------------------------------------------------
#
# Project created by QtCreator 2018-01-09T09:24:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = spector-emulator
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            countthread.cpp \
    chartlayoutdialog.cpp

HEADERS  += mainwindow.h \
            countthread.h \
    chartlayoutdialog.h

FORMS    += mainwindow.ui

CONFIG(debug, debug|release) {
    LIBS+= -L../spector-emulator-controll-widget/debug -lControllSEWidget
    LIBS+= -L../spector-emulator-plot-widget/debug -lPlotSEWidget
    LIBS+= -L../spector-emulator-data-generator/debug -lDataGeneratorLib
    LIBS+= -L../spector-emulator-peak-detector/debug -lPeakDetectorLib
    LIBS+= -L../../../spector-emulator/libs/libqt-searchwidget/debug -lsearchwidget

}

CONFIG(release, debug|release) {
    LIBS+= -L../spector-emulator-controll-widget/release -lControllSEWidget
    LIBS+= -L../spector-emulator-plot-widget/release -lPlotSEWidget
    LIBS+= -L../spector-emulator-data-generator/release -lDataGeneratorLib
    LIBS+= -L../spector-emulator-peak-detector/release -lPeakDetectorLib
    LIBS+= -L../../../../spector-emulator/libs/libqt-searchwidget/release -lsearchwidget

}
RC_FILE     = resources.rc
