#ifndef PLOTSEWIDGET_H
#define PLOTSEWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

class PlotSEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotSEWidget(QWidget *parent = 0);
    ~PlotSEWidget();

    ///
    /// \brief setPlotData sets values for rendering
    /// \param values - ordinates of the gas spectrum
    ///
    void setPlotData(QVector<double> values);
    ///
    /// \brief getData returns the ordinates of the gas spectrum
    ///
    QSharedPointer<QCPGraphDataContainer> getData() const;

private:

    QCustomPlot* customPlot;

};

#endif // PLOTSEWIDGET_H
