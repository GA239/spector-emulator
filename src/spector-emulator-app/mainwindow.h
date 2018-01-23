#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../spector-emulator-controll-widget/controllsewidget.h"
#include "../spector-emulator-plot-widget/plotsewidget.h"
#include "../spector-emulator-data-generator/datagenerator.h"
#include "countthread.h"

#define _RC_SUCCES_ 1

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
    void estimateGasSpector(QVector<int> U, QModelIndexList M);


public slots:

private slots:
    void EstimateBottomPressed();
    void progressSet(int value);
    void GetResults(QVector<double> results);
    void bobotreu();
    void updateProgressbar();
    int saveConfig();
    int exportDataSlot();
    int loadDataSlot();
private:

    void createAndConfigureElemtsOfWindow(void);
    void setwidgetAssembly(void);
    void createThreadEstimation(void);
    int exportData(QString &resultStr);

    Ui::MainWindow *ui;
    ControllSEWidget* controll;
    PlotSEWidget* plotter;
    QPushButton* startEstimateBottom;
    QProgressBar* progressBar;

    //class for parallel (heavy) estimations
    DataGenerator* generator;

    //The flow in which the estimation will be performed
    CountThread* worker;

};

#endif // MAINWINDOW_H
