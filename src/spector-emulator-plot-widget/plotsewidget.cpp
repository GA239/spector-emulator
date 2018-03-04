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
    layout = new QGridLayout(this);

    for(int index = 0; index < customPlotArray.length(); ++index){
        customPlotArray[index] = new QCustomPlot(this);
        layout->addWidget(customPlotArray[index],index,0);
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
    QVector<double> rx = x;
    QVector<double> ry = values;

//    interpolation(x,values,rx,ry);

    // set axes ranges, so we see all data:
    customPlotArray[plotName]->xAxis->setRange(0, rx.length());
    customPlotArray[plotName]->yAxis->setRange(0, maximum);

    customPlotArray[plotName]->graph(0)->setData(rx, ry);
    customPlotArray[plotName]->replot();
    customPlotArray[plotName]->show();
}

QSharedPointer<QCPGraphDataContainer> PlotSEWidget::getData(PLOT_NAMES plotName) const
{
    /*
    if(customPlotArray.length() <= plotName)
        return nullptr;
    if(customPlotArray[plotName]->isHidden())
        return nullptr;
    */
    return customPlotArray[plotName]->graph(0)->data();
}

void PlotSEWidget::cleanData()
{
    for(int index = 0; index < customPlotArray.length(); ++index){
        customPlotArray[index]->graph(0)->data()->clear();
        customPlotArray[index]->hide();
    }
}

void PlotSEWidget::changeLayoutSlot(int value)
{
    changeLayout((LAYOUTS_TYPES)value);
}

void PlotSEWidget::changeLayout(LAYOUTS_TYPES ltype)
{
    for(int index = 0; index < customPlotArray.length(); ++index){
        layout->removeWidget(customPlotArray[index]);
        if(ltype == LAYOUTS_TYPES::HORIZONTAL)
            layout->addWidget(customPlotArray[index],index,0);
        if(ltype == LAYOUTS_TYPES::VERTICAL)
            layout->addWidget(customPlotArray[index],0,index);
    }
}

void PlotSEWidget::interpolation(QVector<double> &x, QVector<double> &y, QVector<double> &rx, QVector<double> &ry)
{
    int N = x.length() - 1;

    QVector<double> h = QVector<double>(N+1,0);
    QVector<double> l = QVector<double>(N+1,0);
    QVector<double> delta = QVector<double>(N+1,0);
    QVector<double> lambda = QVector<double>(N+1,0);
    QVector<double> c = QVector<double>(N+1,0);
    QVector<double> d = QVector<double>(N+1,0);
    QVector<double> b = QVector<double>(N+1,0);
    int k = 0;
    for(k=1; k<=N; k++){
        h[k] = x[k] - x[k-1];
        if(h[k]==0){
            return;
        }
        l[k] = (y[k] - y[k-1])/h[k];
    }

    delta[1] = - h[2]/(2*(h[1]+h[2]));
    lambda[1] = 1.5*(l[2] - l[1])/(h[1]+h[2]);
    for(k=3; k<=N; k++){
       delta[k-1] = - h[k]/(2*h[k-1] + 2*h[k] + h[k-1]*delta[k-2]);
       lambda[k-1] = (3*l[k] - 3*l[k-1] - h[k-1]*lambda[k-2]) /
             (2*h[k-1] + 2*h[k] + h[k-1]*delta[k-2]);
    }
    c[0] = 0;
    c[N] = 0;
    for(k=N; k>=2; k--){
       c[k-1] = delta[k-1]*c[k] + lambda[k-1];
    }
    for(k=1; k<=N; k++){
       d[k] = (c[k] - c[k-1])/(3*h[k]);
       b[k] = l[k] + (2*c[k]*h[k] + h[k]*c[k-1])/3;
    }


    double start = x[0];
    double end = x[N];
    double step = (x[N] - x[0]) / (N*2);
    for(double s = start; s<=end; s+= step){
        //find k, where s in [x_k-1; x_k]
        k = 1;
        for(k=1; k<=N; k++){
            if(s>=x[k-1] && s<=x[k]){
                break;
            }
        }
        double F = y[k] + b[k]*(s-x[k]) + c[k]*pow(s-x[k], 2) + d[k]*pow(s-x[k], 3);
        ry.push_back(F);
        rx.push_back(s);
    }
}
