#ifndef PEAKDETECTOR_H
#define PEAKDETECTOR_H

#include <QObject>
#include <QVector>

#define _ERROR_NO_ROOM_ -10
#define _RC_SUCCESS_ 1

///
/// \brief The PeakDetector class - class realizing algorithms for an algorithm for peak detection based on zero-score algorithm.
/// This algorithm is particularized for the case of having the whole set of points in which the peak needs to be found.
/// A real time version (needed to be actualized in every new sample) can be easily
/// derived by introducing a lag period in which there is no output. 
///
class PeakDetector: public QObject
{
    Q_OBJECT

public:
    explicit PeakDetector(QObject *parent = 0);
    ~PeakDetector();
    int estimate(QVector<double> &inputData, QVector<double> &outputData);

signals:

public slots:

private:

};

#endif // PEAKDETECTOR_H
