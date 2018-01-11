#include "datagenerator.h"

#include <iostream>
#include <string>
#include <random>
/**
 * @brief Default constructor
 */
DataGenerator::DataGenerator()
{

 }

/**
 * @brief DataGenerator::~DataGenerator
 */
DataGenerator::~DataGenerator()
{

}

QVector<double> DataGenerator::estemateData(int u1, int u2)
{
    int M = 28;
    int Uis = 600;
    int Uik	= 216;

    //generate a normal ion distribution
    double mu = 2.5;
    double sigma = 1;

    const int nrolls=10000;  // number of experiments

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);

    QVector<double> x(nrolls);
    QVector<double> ystart(nrolls);
    QVector<double> y;

    for (int i=0; i<nrolls; ++i) {
        x[i] = distribution(generator);
    }

    int noZeroTofIndex = 0;

    for  (int i=0; i<nrolls; ++i) {

        //estimate the flight time for each ion
        ystart[i] = tof( x[i], M, Uis, u1, u2, Uik );

        if (ystart[i] != 0){
            //y[noZeroTofIndex] = ystart[i];
            y.push_back(ystart[i]);
            noZeroTofIndex = noZeroTofIndex + 1;
        }
    }
    QVector<double> ydiaposon(5000);
    QVector<double> result(5000 - 1);

    for  (int i=0; i<5000 - 1; ++i) {
        ydiaposon[i] = i*2;
        //result = zeros(1,length(ydiaposon)-1);
        result[i] = 0;
    }
    ydiaposon[5000 - 1] = (5000 - 1)*2;

    //create a histogram of flight time
    for  (int i=0; i<result.length(); ++i) {
        for  (int j=0; j < y.length(); ++j) {
            if( (y[j] > ydiaposon[i]) && (y[j] <= ydiaposon[i+1])){
               result[i] = result[i] + 1;
            }
        }
    }
    for  (int i=0; i<x.length(); ++i) {
        std::cout <<x[i]<< " ";
    }
    return result;
}

double DataGenerator::tof(const double x, const int M, const double Uis, const double Um1, const double Um2, const double Uik)
{
    double Lis = 5.0;
    double Lgap1 = 55.0;
    double Lgap2 = 115.0;
    double Lmirr1 = 5.0;
    double Lmirr2 = 15.0;

    if ((x <= 0) || (x > Lis))
        return 0;

    double Uion = x / Lis * Uis;
    if (Uion < Uik)
        return 0;

    if ((Uion > Um1) && (Uion > Um2))
        return 0;

    double k = 1.46472222752782E-02;
    double Vis = k * sqrt(Uion / M);
    // tof ion guide  v0 = 0
    double Ti = 2 * x / Vis;
    double Tg1 = Lgap1 / Vis;
    double Tg2 = Lgap2 / Vis;

    int Tmirr1,Tmirr2,Vmrr;

    if (Uion <= Um1){
        Tmirr1 = 2 * (2 * (Uion / Um1 * Lmirr1) / Vis);
        Tmirr2 = 0;
    }
    else{
        Vmrr = k * sqrt((Uion - Um1) / M);
        Tmirr1 = 2 * (2 * (Uion / Um1 * Lmirr1) / (Vis + Vmrr));
        Tmirr2 = 2 * (2 * ((Uion - Um1) / Um2 * Lmirr2) / Vmrr);
    }

    return Ti + Tg1 + Tg2 + Tmirr1 + Tmirr2;
}
