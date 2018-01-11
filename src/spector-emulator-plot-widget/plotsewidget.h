#ifndef PLOTSEWIDGET_H
#define PLOTSEWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

class PlotSEWidget : public QWidget
{
    Q_OBJECT

public:
    PlotSEWidget(QWidget *parent = 0);
    ~PlotSEWidget();

    void setPlotData(QVector<double> values);

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:

private:

    QCustomPlot* customPlot;

};

#endif // PLOTSEWIDGET_H
