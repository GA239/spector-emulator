#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H
#include <QVector>
#include <QMap>

typedef struct
{
    double x;
    QVector<int> M;
    double Uis;
    double Um1;
    double Um2;
    double Uik;

}Params;

class DataGenerator
{

public:
    DataGenerator();
    ~DataGenerator();
    QVector<double> getData(const int u1, const int u2, QVector<int> M);

private:    
    QVector<QVector<double> > estemateData(const int u1, const int u2, QVector<int> M);
    QVector<double> tof(const Params &param);
    QMap<int,double> gases;
};

#endif // DATAGENERATOR_H
