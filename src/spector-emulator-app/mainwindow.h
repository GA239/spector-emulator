#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../spector-emulator-controll-widget/controllsewidget.h"
#include "../spector-emulator-plot-widget/plotsewidget.h"
#include "../spector-emulator-data-generator/datagenerator.h"
#include "../spector-emulator-peak-detector/peakdetector.h"
#include "countthread.h"

namespace Ui {
class MainWindow;
}

///
/// \brief The MainWindow class - main application window
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    ///
    /// \brief estimateGasSpector - signal triggering calculation of the gas spectrum
    /// \param U - voltages
    /// \param M - model objects
    ///
    void estimateGasSpector(QVector<int> U, QModelIndexList M);

private slots:
    ///
    /// \brief EstimateBottomPressed start estimate gas spectrum
    ///
    void EstimateBottomPressed();
    ///
    /// \brief progressSet set value to progress bar
    /// \param value
    ///
    void progressSet(int value);
    ///
    /// \brief GetResults get results from generator and set it to plotter
    /// \param results
    ///
    void GetResults(QVector<double> results);
    ///
    /// \brief stopEstimate terminate estimation
    ///
    void stopEstimate();
    void updateProgressbar();
    ///
    /// \brief exportConfigSlot expotrt controll params and data to file
    /// \return
    ///
    int exportConfigSlot();
    ///
    /// \brief importConfigSlot expotrt controll params and data to file
    /// \return
    ///
    int importConfigSlot();
    ///
    /// \brief saveDataSlot save data to file;
    /// \return
    ///
    int saveDataSlot();
    int estimatePikiSlot();
private:

    void createAndConfigureElemtsOfWindow(void);
    void setwidgetAssembly(void);
    void createThreadEstimation(void);
    int exportData(QString &resultStr);

    Ui::MainWindow *ui;
    ControllSEWidget* controll;
    PlotSEWidget* plotter;
    QPushButton* startEstimateBottom;
    QPushButton* pikiEstimateBottom;

    QProgressBar* progressBar;

    //class for parallel (heavy) estimations
    DataGenerator* generator;

    //The flow in which the estimation will be performed
    CountThread* worker;
};

#endif // MAINWINDOW_H
