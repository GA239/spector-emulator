#include "datagenerator.h"

#include <iostream>
#include <string>
#include <random>
#include <QDebug>

QVector<double> DataGenerator::timeOfFly(const Params &param){
    // function of calculating the flight time of the ion from Dubensky
    QVector<double> result(param.M.length(),0);
    double d1 = 0.004;
    double d3 = 0.004;
    double d4 = 0.022;
    double L = 0.136;
    double mH = 1.647E-27;
    double g = 1.602E-19;
    double x = 0.004;
    double U1 = 600.0;

    double ionMass;
    double thirdSqrt,secondSqrt,firstSqrt;
    double thirdCoefficient,secondCoefficient,firstCoefficient;

    for(int i = 0; i < param.M.length(); ++i){
        ionMass = mH * param.M[i];
        thirdSqrt = U1*(x/d1) - param.Um1;
        thirdSqrt = thirdSqrt < 0.0 ? 0.0 : sqrt(thirdSqrt);
        secondSqrt = sqrt(U1*(x/d1));
        firstSqrt = sqrt(ionMass/(2*g));
        firstCoefficient = (2*d1)/U1;
        secondCoefficient = (4*d3)/param.Um1;
        thirdCoefficient = (4*d4)/(param.Um2-param.Um1);
        result[i] = firstSqrt * ((firstCoefficient * secondSqrt) + secondCoefficient * (secondSqrt - thirdSqrt) + thirdCoefficient * thirdSqrt + L/secondSqrt);
        result[i] = result[i] < 0.0 ? 0.0 : result[i]*100*1000000;
        qDebug() << "Mass = " << param.M[i] << "Time = " << result[i];
    }
    return result;
}
