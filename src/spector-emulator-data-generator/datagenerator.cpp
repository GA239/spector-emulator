#include "datagenerator.h"

#include <iostream>
#include <string>
#include <random>
#include <QDebug>

bool DataGenerator::bobo;

/**
 * @brief Default constructor
 */
DataGenerator::DataGenerator(QObject *parent):
    QObject(parent)
{
    QVector<QPair<int,double> > mc;
    this->stopEstimate = false;

    gasesNames.append("Item1");
    mc.append(QPair<int,double>(28, 1.0));

    gasesNames.append("Item2");
    mc.append(QPair<int,double>(32, 0.3));

    gasesNames.append("Item3");
    mc.append(QPair<int,double>(14, 0.2));

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

QVector<double> DataGenerator::getData(const int u1, const int u2, QModelIndexList M)
{
    qDebug() << "getData+";

    QList<QModelIndex> tmpIndexList;
    QVector<double> result;

    for(int i = 0; i < M.length(); ++i)
    {
        for(int j = 0; j < gases.keys().length(); ++j)
        {
            if(gases.keys()[j].data() == M[i].data())
            {
                tmpIndexList.append(gases.keys()[j]);
                break;
            }
        }
    }
    if(tmpIndexList.empty())
        return result;

    QVector<QVector<double> > tmp = estemateData(u1,u2,createMassesOfGases(tmpIndexList));
    result.resize(tmp[0].length());
    for(int j = 0; j < result.length(); ++j)
    {
        result[j] = 0;
    }
    for(int i = 0; i < M.length(); ++i)
    {
        for(int j = 0; j < result.length(); ++j)
        {
            result[j]+= tmp[i][j] * gases.value(tmpIndexList[i]).second;
        }
    }
    qDebug() << "getData-";
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
    {
        result.append(gases.value(M[i]).first);
    }
    return result;
}

void DataGenerator::estimateGasSpector(QVector<int> U, QModelIndexList M)
{
    qDebug() << "estimateGasSpector";
    QVector<double> result;
    this->stopEstimate = false;
    bobo = false;
    result = getData(U[0], U[1], M);
    //if(!this->stopEstimate)
    if(!bobo)
        emit SendResults(result);
    emit done();
}

void DataGenerator::stopEstimation()
{
    this->stopEstimate = true;
}

QVector<QVector<double> > DataGenerator::estemateData(const int u1, const int u2, QVector<int> M)
{
    qDebug() << "estemateData+";
    //обеспечивает обработку событий пралельно с выполнением цикла.
    //QCoreApplication::processEvents();

    Params p;
    p.Uik = 216;
    p.Uis = 600;
    p.Um1 = u1;
    p.Um2 = u2;
    p.M = M;

    //generate a normal ion distribution
    double mu = 2.5;
    double sigma = 1;

    const int nrolls=10000;  // number of experiments

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mu,sigma);

    QVector<double> x(nrolls);
    QVector<QVector<double> > ystart;
    QVector<QVector<double> > y;

    ystart.resize(p.M.length());
    y.resize(p.M.length());

    for (int i=0; i<p.M.length(); ++i) {
        ystart[i].reserve(nrolls);
    }

    for (int i=0; i<nrolls; ++i) {
        x[i] = distribution(generator);
    }

    QVector<double> tmp;
    for  (int j=0; j<nrolls; ++j) {

        //estimate the flight time for each ion
        p.x = x[j];
        tmp = tof(p);
        for  (int i=0; i<tmp.length(); ++i) {
            if (tmp[i] != 0){
                y[i].push_back(tmp[i]);
            }
        }
    }
    QVector<double> ydiaposon;
    ydiaposon.resize(5000 + 2);
    // set grid
    for  (int i=0; i<ydiaposon.length(); ++i) {
        ydiaposon[i] = i*2;
    }

    QVector<QVector<double> > result;

    result.resize(p.M.length());

    int progress = 0;
    emit progressChanged(progress);

    for  (int j=0; j<result.length(); ++j)
    {

        result[j].resize(ydiaposon.length()-1);

        //create a histogram of flight time
        for  (int i=0; i<result[j].length(); ++i)
        {
            result[j][i] = 0;
            for(int k=0; k < y[j].length(); ++k)
            {
                if( (y[j][k] > ydiaposon[i]) && (y[j][k] <= ydiaposon[i+1]))
                {
                   result[j][i] = result[j][i] + 1;
                }
            }
            progress = (( i + j*result[j].length() ) * 100)/(result.length() * result[j].length());
            //qDebug() << progress;
            emit progressChanged(progress);
            //if (this->stopEstimate)
            if (bobo)
            {
              break;
            }
        }
    }

    emit progressChanged(100);
/*
    for  (int i=0; i<x.length(); ++i) {
        std::cout <<x[i]<< " ";
    }*/
    qDebug() << "estemateData-";
    return result;
}

QVector<double> DataGenerator::tof(const Params &param)
{
    double Lis = 5.0;
    double Lgap1 = 55.0;
    double Lgap2 = 115.0;
    double Lmirr1 = 5.0;
    double Lmirr2 = 15.0;

    QVector<double> result;
    result.resize(param.M.length());
    for(int i = 0; i < param.M.length(); ++i){
        result[i] = 0;
    }

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
    }
    return result;
}
