#include <QDebug>
#include <stdio.h> /*memset*/
#include "peakdetector.h"
#include "algoritm.h"

/**
 * @brief Default constructor
 */
PeakDetector::PeakDetector(QObject *parent):
    QObject(parent)
{

}

/**
 * @brief PeakDetector::~PeakDetector
 */
PeakDetector::~PeakDetector()
{

}


int PeakDetector::estimate(QVector<double> &inputData){
    /* входные переменные */

    /* Количество элементов во входных данных */
    size_t nElements = inputData.length();

    /* входной вектор данных с  n Элементами */
    double *DataRe = new (std::nothrow) double[nElements];
    if (!DataRe){
        return _ERROR_NO_ROOM_;
    }
    memset(DataRe, 0, nElements * sizeof(double));
    for(size_t i = 0; i < nElements; ++i)
       DataRe[i] = inputData[i];

    /* Количество выборок в фильтрах среднего и стандартного отклонения */
    double lag = 50;
    // threshold = mxGetScalar(prhs[2]);   /* Количество случаев, когда std текущего образца должна превосходить средний */
    double threshold = 5;
    // influence = mxGetScalar(prhs[3]);   /* Влияние образца пика на вычисление среднего и std */
    double influence = 0;

    /* выходные переменные */

    /* m_signals = 1 x nElements */
    double *m_signals = new (std::nothrow) double[nElements];
    if (!m_signals){
        delete [] DataRe;
        return _ERROR_NO_ROOM_;
    }
    memset(m_signals, 0, nElements * sizeof(double));

    /* avgFilter =  1 x nElements */
    double *avgFilter = new (std::nothrow) double[nElements];
    if (!avgFilter){
        delete [] DataRe;
        delete [] m_signals;
        return _ERROR_NO_ROOM_;
    }
    memset(avgFilter, 0, nElements * sizeof(double));

    /* stdFilter =  1 x nElements */
    double *stdFilter = new (std::nothrow) double[nElements];
    if (!stdFilter) {
        delete [] DataRe;
        delete [] m_signals;
        delete [] avgFilter;
        return _ERROR_NO_ROOM_;
    }
    memset(stdFilter, 0, nElements * sizeof(double));

    /* peak =  1 x nElements */
    double *peak = new (std::nothrow) double[nElements];
    if (!peak){
        delete [] DataRe;
        delete [] m_signals;
        delete [] avgFilter;
        delete [] stdFilter;
        return _ERROR_NO_ROOM_;
    }
    memset(peak, 0, nElements * sizeof(double));

    /* фильтрованная версия входных данных для вычисления стандартного и среднего */
    double * filteredData = new (std::nothrow) double[nElements];
    if (!filteredData){
        delete [] DataRe;
        delete [] m_signals;
        delete [] avgFilter;
        delete [] stdFilter;
        delete [] peak;
        return _ERROR_NO_ROOM_;
    }
    memset(filteredData, 0, nElements * sizeof(double));

    /* Функциональность */
    peak_detection(nElements, DataRe, lag, threshold, influence, m_signals, avgFilter, stdFilter, peak, filteredData);

    QVector<double> resultSignnals(nElements, 0);
    QVector<double> resultAvgFilter(nElements, 0);
    QVector<double> resultStdFilter(nElements, 0);
    QVector<double> resultPeak(nElements, 0);
    QVector<double> resultFilteredData(nElements, 0);

    for(size_t i = 0; i < nElements; ++i){
        resultSignnals[i] = m_signals[i];
        resultAvgFilter[i] = avgFilter[i];
        resultStdFilter[i] = stdFilter[i];
        resultPeak[i] = peak[i];
        resultFilteredData[i] = filteredData[i];
    }

    estimates[QString(estimatesTracksNames[ESTIMATES_NAMES::SIGNALS])] = resultSignnals;
    estimates[QString(estimatesTracksNames[ESTIMATES_NAMES::AVG_FILTER])] = resultAvgFilter;
    estimates[QString(estimatesTracksNames[ESTIMATES_NAMES::STD_FILTER])] = resultStdFilter;
    estimates[QString(estimatesTracksNames[ESTIMATES_NAMES::PEAK])] = resultPeak;
    estimates[QString(estimatesTracksNames[ESTIMATES_NAMES::FILTERED_DATA])] = resultFilteredData;

    delete [] DataRe;
    delete [] m_signals;
    delete [] avgFilter;
    delete [] stdFilter;
    delete [] peak;
    delete [] filteredData;

    return _RC_SUCCESS_;
}

int PeakDetector::getEstimateByName(QString name, QVector<double> &estimatedValues)
{
    if (estimates.contains(name)){
        estimatedValues = estimates.value(name);
        return _RC_SUCCESS_;
    }
    return _ERROR_WRONG_ARGUMENT_;
}
