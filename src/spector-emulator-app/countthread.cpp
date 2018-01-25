#include "countthread.h"

CountThread::CountThread(QObject *parent) :
    QThread(parent)
{
}
void CountThread::estimateGasSpector(QVector<int> U, QModelIndexList M)
{
    this->start();
    emit estimateGasSpectorStarted(U,M);
}
