#include <QDebug>
#include <QGridLayout>
#include "plotsewidget.h"

/**
 * @brief Default constructor
 * @param parent
 */
PlotSEWidget::PlotSEWidget(QWidget *parent) : QWidget(parent)
{
    customPlot = new QCustomPlot(this);
    QGridLayout *layout = new QGridLayout(this);

    QHBoxLayout *horizontLayoutU1 = new QHBoxLayout(parent);
    horizontLayoutU1->addWidget(customPlot);

    layout->addLayout(horizontLayoutU1,0,0);

    // create graph and assign data to it:
    customPlot->addGraph();

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 5000);
    customPlot->yAxis->setRange(0, 50);
    customPlot->replot();
}

/**
 * @brief PlotSEWidget::~PlotSEWidget
 */
PlotSEWidget::~PlotSEWidget()
{
}

void PlotSEWidget::setPlotData(QVector<double> values)
{
    if(values.empty())
        return;
    //calculate the abscissas of the gas spectrum
    QVector<double> x(values.length());
    for(int i = 0; i < values.length(); ++i)
        x[i] = i;

    customPlot->graph(0)->setData(x, values);
    customPlot->replot();
}

QSharedPointer<QCPGraphDataContainer> PlotSEWidget::getData() const
{
     return customPlot->graph(0)->data();
}
