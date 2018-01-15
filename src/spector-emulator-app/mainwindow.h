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
    void estimateGasSpector(int u1, int u2, QVector<int> m);

public slots:
    void progressSet(int value);
    void GetResults(QVector<double> results);

private slots:
    void EstimateBottomPressed();


private:

    void estemateNewValues();
    QVector<double> estemateData(int u1, int u2);

    Ui::MainWindow *ui;
    ControllSEWidget* controll;
    PlotSEWidget* plotter;
    SearchWidget *searchWidget;
    QPushButton* startEstimateBottom;
    QProgressBar* progressBar;
};

#endif // MAINWINDOW_H
