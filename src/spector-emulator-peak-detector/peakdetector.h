#ifndef PEAKDETECTOR_H
#define PEAKDETECTOR_H

#include <QObject>
#include <QVector>
#include <QMap>

#define _ERROR_NO_ROOM_ -10
#define _ERROR_WRONG_ARGUMENT_ -9

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
    int estimate(QVector<double> &inputData);

    enum ESTIMATES_NAMES { SIGNALS, AVG_FILTER, STD_FILTER, PEAK,FILTERED_DATA, DIMENSION_ESTIMATES_NAMES };
    static constexpr char const * const estimatesTracksNames[DIMENSION_ESTIMATES_NAMES] = {
        "Signal",       /* -Signal: Выходной вектор, в котором хранится 0, не содержит пика в этом образце и 1 или -1 в противном случае */
        "AvgFilter",    /* -AvgFilter: Выходной вектор, сохраняющий среднее значение, вычисленное через итерации алгоритма */
        "StdFilter",    /* -StdDevFilter: Выходной вектор, сохраняющий стандартное отклонение, вычисленное через итерации алгоритма */
        "Peak",         /* -Peak: Выходной вектор, сохраняющий 1 в положениях, где максимум внутри пика был найден, 0 в противном случае */
        "FilteredData"  /* -FilteredData: Вектор отфильтрованных данных в зависимости от влияния  входного параметра  */
    };
    int getEstimateByName(QString name, QVector<double> &estimatedValues);
signals:

public slots:

private:
    QMap< QString, QVector<double> > estimates;

};

#endif // PEAKDETECTOR_H
