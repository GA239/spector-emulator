#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../spector-emulator-controll-widget/controllsewidget.h"
#include "../spector-emulator-plot-widget/plotsewidget.h"
#include "../../libs/libqt-searchwidget/include/searchwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //void test(int position);


private slots:
    void U1ControllChanged(int value);
    void U2ControllChanged(int value);

private:

    void setValueForU1Lable(int value);
    void setValueForU2Lable(int value);
    QVector<double> estemateData(int u1, int u2);

    Ui::MainWindow *ui;
    ControllSEWidget* controll;
    PlotSEWidget* plotter;
    SearchWidget *searchWidget;
};

#endif // MAINWINDOW_H
