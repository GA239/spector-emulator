#include "countthread.h"

CountThread::CountThread(QObject *parent) :
    QThread(parent)
{
}
/* ������ ��������
void CountThread::MonteCarloStart(int num_sims,  double S,  double K, double r,  double v,  double T, bool type)
{
    this->start();
    emit MonteCarloStarted(num_sims, S, K, r, v, T, type);
}//*/

void CountThread::estimateGasSpector(int u1, int u2, QVector<int> m)
{
    this->start();
    emit estimateGasSpectorStarted(u1,u2,m);
}
// ��������� ������
void CountThread::stoc()
{
    emit stopc();
}
