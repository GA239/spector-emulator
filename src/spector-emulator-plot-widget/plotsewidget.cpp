#include <QDebug>
#include <QGridLayout>
#include "plotsewidget.h"

/**
 * @brief Default constructor
 * @param parent
 */
PlotSEWidget::PlotSEWidget(QWidget *parent, int numberOfPlots) : QWidget(parent)
{
    customPlotArray.resize(numberOfPlots);
    QVBoxLayout *layout = new QVBoxLayout(this);

    for(int index = 0; index < customPlotArray.length(); ++index){
        customPlotArray[index] = new QCustomPlot(this);
        layout->addWidget(customPlotArray[index]);
        // create graph and assign data to it:
        customPlotArray[index]->addGraph();

        // give the axes some labels:
        //customPlotArray[index]->xAxis->setLabel("x");
        //customPlotArray[index]->yAxis->setLabel("y");
        customPlotArray[index]->hide();
    }
}

/**
 * @brief PlotSEWidget::~PlotSEWidget
 */
PlotSEWidget::~PlotSEWidget()
{
}

void PlotSEWidget::setPlotData(QVector<double> values, PLOT_NAMES plotName)
{
    if(customPlotArray.length() <= plotName)
        return;

    if(values.empty())
        return;
    QVector<double> x(values.length());
    double maximum = values[0];
    //calculate the abscissas and maximum of the data
    for(int i = 0; i < values.length(); ++i){
        x[i] = i;
        if(values[i] > maximum)
            maximum = values[i];
    }

    // set axes ranges, so we see all data:
    customPlotArray[plotName]->xAxis->setRange(0, values.length());
    customPlotArray[plotName]->yAxis->setRange(0, maximum);

    customPlotArray[plotName]->graph(0)->setData(x, values);
    customPlotArray[plotName]->replot();
    customPlotArray[plotName]->show();
}

QSharedPointer<QCPGraphDataContainer> PlotSEWidget::getData(PLOT_NAMES plotName) const
{
    if(customPlotArray.length() <= plotName)
        return nullptr;
    if(customPlotArray[plotName]->isHidden())
        return nullptr;
    return customPlotArray[plotName]->graph(0)->data();
}

void PlotSEWidget::cleanData()
{
    for(int index = 0; index < customPlotArray.length(); ++index){
        customPlotArray[index]->graph(0)->data()->clear();
        customPlotArray[index]->hide();
    }
}
