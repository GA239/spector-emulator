#ifndef COUNTTHREAD_H
#define COUNTTHREAD_H

#include <QThread>
#include <QVector>

class CountThread : public QThread
{
    Q_OBJECT
public:
    explicit CountThread(QObject *parent = 0);

signals:
    //void MonteCarloStarted( int num_sims,  double S,  double K, double r,  double v,  double T, bool type);
    void estimateGasSpectorStarted(int u1,  int u2, QVector<int> m);

    void stopc();
public slots:
    //void MonteCarloStart( int num_sims,  double S,  double K, double r,  double v,  double T, bool type);
    void estimateGasSpector(int u1,  int u2, QVector<int> m);

    void stoc();
};

#endif // COUNTTHREAD_H
