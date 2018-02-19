#include "datagenerator.h"

#include <iostream>
#include <string>
#include <random>
#include <QDebug>

bool DataGenerator::stopEstimate;

/**
 * @brief Default constructor
 */
DataGenerator::DataGenerator(QObject *parent):
    QObject(parent)
{
    QVector<QPair<int,double> > mc;

    gasesNames.append("N2");
    mc.append(QPair<int,double>(28, 1.0));

    gasesNames.append("O2");
    mc.append(QPair<int,double>(32, 0.3));

    gasesNames.append("N");
    mc.append(QPair<int,double>(14, 0.2));

    gasesNames.append("O");
    mc.append(QPair<int,double>(16, 0.07));

    gasesNames.append("Ar");
    mc.append(QPair<int,double>(40, 0.05));

    gasesNames.append("CO2");
    mc.append(QPair<int,double>(44, 0.01));

    model = new QStringListModel(this);
    model->setStringList(gasesNames);

    for(int i=0;i<model->rowCount();i++){
      QModelIndex index = model->index(i,0);
      gases.insert(index,mc[i]);
    }
}

/**
 * @brief DataGenerator::~DataGenerator
 */
DataGenerator::~DataGenerator()
{
    delete model;
}
QVector<double> DataGenerator::getData(const int u1, const int u2, const int uk, QModelIndexList M)
{
    QList<QModelIndex> tmpIndexList;
    QVector<double> empty;
    for(int i = 0; i < M.length(); ++i){
        for(int j = 0; j < gases.keys().length(); ++j){
            if(gases.keys()[j].data() == M[i].data()){
                tmpIndexList.append(gases.keys()[j]);
                break;
            }
        }
    }
    if(tmpIndexList.empty())
        return empty;

    QVector<QVector<double> > tmp = estemateData(u1,u2,uk,createMassesOfGases(tmpIndexList));
    QVector<double> result(tmp[0].length(),0);
    for(int i = 0; i < M.length(); ++i){
        for(int j = 0; j < result.length(); ++j)
            result[j]+= tmp[i][j] * gases.value(tmpIndexList[i]).second;
    }
    return result;
}

QStringList DataGenerator::getModelElemnts()
{
    return gasesNames;
}

QVector<int> DataGenerator::createMassesOfGases(QList<QModelIndex> M)
{
    QVector<int> result;
    for (int i = 0; i < M.length(); i++)
        result.append(gases.value(M[i]).first);
    return result;
}

void DataGenerator::estimateGasSpector(QVector<int> U, QModelIndexList M)
{
    QVector<double> result;
    stopEstimate = false;
    result = getData(U[0], U[1], U[2], M);
    if(!stopEstimate)
        emit SendResults(result);
    emit done();
}

QVector<QVector<double> > DataGenerator::estemateData(const int u1, const int u2, const int uk, QVector<int> M)
{
    Params p(0,M,600,u1,u2,uk);

    // normal dist params
    double mu = 2.5;
    double sigma = 1;
    const int nrolls=10000;  // number of experiments

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);

    QVector<double> x(nrolls,0);

    QVector<QVector<double> > ystart;
    ystart.resize(p.M.length());

    QVector<QVector<double> > y;
    y.resize(p.M.length());

    for (int i=0; i<p.M.length(); ++i)
        ystart[i].reserve(nrolls);

    //generate a normal ion distribution
    for (int i=0; i<nrolls; ++i)
        x[i] = distribution(generator);

    QVector<double> tmp;
    for(int j=0; j<nrolls; ++j){
        //estimate the flight time for each ion
        p.x = x[j];
        //tmp = tof(p);
        tmp = timeOfFly(p);
        for  (int i=0; i<tmp.length(); ++i){
            if (tmp[i] != 0)
                y[i].push_back(tmp[i]);
        }
    }
    // set grid
    QVector<double> ydiaposon(5000 + 2, 0);
    for  (int i=0; i<ydiaposon.length(); ++i){
        ydiaposon[i] = i*2;
    }

    QVector<QVector<double> > result;
    result.resize(p.M.length());

    int progress = 0;
    emit progressChanged(progress);

    for(int j=0; j<result.length(); ++j){
        result[j].resize(ydiaposon.length()-1);

        //create a histogram of flight time
        for  (int i=0; i<result[j].length(); ++i){
            result[j][i] = 0;
            for(int k=0; k < y[j].length(); ++k){
                if( (y[j][k] > ydiaposon[i]) && (y[j][k] <= ydiaposon[i+1])){
                   result[j][i] = result[j][i] + 1;
                }
            }
            progress = (( i + j*result[j].length() ) * 100)/(result.length() * result[j].length());
            emit progressChanged(progress);
            if (stopEstimate)
                break;
        }
    }
    emit progressChanged(100);
    return result;
}

QVector<double> DataGenerator::tof(const Params &param)
{
    // function of calculating the flight time of the ion from Oleg
    double Lis = 5.0;
    double Lgap1 = 55.0;
    double Lgap2 = 115.0;
    double Lmirr1 = 5.0;
    double Lmirr2 = 15.0;

    QVector<double> result(param.M.length(),0);

    if ((param.x <= 0) || (param.x > Lis))
        return result;

    double Uion = param.x / Lis * param.Uis;
    if (Uion < param.Uik)
        return result;

    if ((Uion > param.Um1) && (Uion > param.Um2))
        return result;

    double k = 1.46472222752782E-02;

    double Vis,Ti,Tg1,Tg2;
    double Tmirr1,Tmirr2,Vmrr;

    for(int i = 0; i < param.M.length(); ++i){
        Vis = k * sqrt(Uion / param.M[i]);
        // tof ion guide  v0 = 0
        Ti = 2 * param.x / Vis;
        Tg1 = Lgap1 / Vis;
        Tg2 = Lgap2 / Vis;

        if (Uion <= param.Um1){
            Tmirr1 = 2 * (2 * (Uion / param.Um1 * Lmirr1) / Vis);
            Tmirr2 = 0;
        }
        else{
            Vmrr = k * sqrt((Uion - param.Um1) / param.M[i]);
            Tmirr1 = 2 * (2 * (Uion / param.Um1 * Lmirr1) / (Vis + Vmrr));
            Tmirr2 = 2 * (2 * ((Uion - param.Um1) / param.Um2 * Lmirr2) / Vmrr);
        }
        result[i] = Ti + Tg1 + Tg2 + Tmirr1 + Tmirr2;
        qDebug() << "Mass = " << param.M[i] << "Time = " << result[i];
    }
    return result;
}
