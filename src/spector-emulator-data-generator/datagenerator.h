#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H
#include <QVector>

class DataGenerator
{

public:
    DataGenerator();
    ~DataGenerator();
    QVector<double> estemateData(int u1, int u2);

private:    
    double tof(const double x, const int M, const double Uis, const double Um1, const double Um2, const double Uik);
};

#endif // DATAGENERATOR_H
