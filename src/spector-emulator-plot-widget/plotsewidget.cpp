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
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(0, 100);

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
    QVector<double> x(100);
    for(unsigned int i = 0; i < 100; ++i)
        x[i] = i;

    customPlot->graph(0)->setData(x, values);
    // give the axes some labels:
    //customPlot->xAxis->setLabel("x");
    //customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    //customPlot->xAxis->setRange(0, 100);
    //customPlot->yAxis->setRange(0, 1);
    customPlot->replot();

}

/**
 * @brief Action is called when key press widget event ocurs
 * @param event
 */
void PlotSEWidget::keyPressEvent(QKeyEvent *event)
{
     Q_UNUSED(event);
    /*
    switch (event->key()) {
        case Qt::Key_Escape: {
            this->showInWindow();
        } break;
        case Qt::Key_Space:
        case Qt::Key_PageUp: {
            this->showNext();
        } break;
        case Qt::Key_PageDown: {
            this->showPrev();
        } break;
        default: break;
    }*/
    return;
}

/**
 * @brief Action is called when mouse wheel widget event ocurs
 * @param event
 */
void PlotSEWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
    return;
}

