#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H
#include <QVector>
#include <QMap>

#include <QObject>
#include <QCoreApplication>
//#include <QApplication>

#include <QMutex>

typedef struct
{
    double x;
    QVector<int> M;
    double Uis;
    double Um1;
    double Um2;
    double Uik;

}Params;

class DataGenerator: public QObject
{

    Q_OBJECT

public:
    DataGenerator();
    ~DataGenerator();
    QVector<double> getData(const int u1, const int u2, QVector<int> M);

signals:
    void progressChanged(int progress);
    void SendResults(QVector<double> results);
    void done();

public slots:
    void estimateGasSpector(int u1, int u2, QVector<int> m);
    //void stopcc();

private:    
    QVector<QVector<double> > estemateData(const int u1, const int u2, QVector<int> M);
    QVector<double> tof(const Params &param);
    QMap<int,double> gases;
};

#endif // DATAGENERATOR_H
