#ifndef PLOTSEWIDGET_H
#define PLOTSEWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

class PlotSEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotSEWidget(QWidget *parent = 0, int numberOfPlots = DIMENSION_PLOT_NAMES);
    ~PlotSEWidget();

    enum PLOT_NAMES { SPECTURM, SIGNALS, PEAK, AVG_FILTER, STD_FILTER, FILTERED_DATA, DIMENSION_PLOT_NAMES };
    enum LAYOUTS_TYPES { HORIZONTAL, VERTICAL};

    ///
    /// \brief setPlotData sets values for rendering
    /// \param values - ordinates of the gas spectrum
    ///
    void setPlotData(QVector<double> values, PLOT_NAMES plotName);
    ///
    /// \brief getData returns the ordinates of the gas spectrum
    ///
    QSharedPointer<QCPGraphDataContainer> getData(PLOT_NAMES plotName) const;
    void cleanData();
public slots:
    void changeLayoutSlot(int);
private:
    void changeLayout(LAYOUTS_TYPES ltype);
    void interpolation(QVector<double> &x, QVector<double> &y, QVector<double> &rx, QVector<double> &ry);

    QVector<QCustomPlot*> customPlotArray;
    QGridLayout *layout;
};

#endif // PLOTSEWIDGET_H
