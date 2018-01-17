#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H
#include <QVector>
#include <QMap>

#include <QObject>
#include <QCoreApplication>
#include <QItemSelectionModel>
#include <QStringListModel>

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
    QStringList getModelElemnts();

signals:
    void progressChanged(int progress);
    void SendResults(QVector<double> results);
    void done();

public slots:
    void estimateGasSpector(QVector<int> U, QModelIndexList M);
    //void stopcc();

private:    
    QVector<double> getData(const int u1, const int u2, QModelIndexList M);
    QVector<QVector<double> > estemateData(const int u1, const int u2, QVector<int> M);
    QVector<int> createMassesOfGases(QList<QModelIndex> M);
    QVector<double> tof(const Params &param);

    QMap<QModelIndex,QPair<int,double> > gases;
    QStringListModel *model;
    QStringList gasesNames;
};

#endif // DATAGENERATOR_H
