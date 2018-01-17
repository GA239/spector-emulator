#ifndef COUNTTHREAD_H
#define COUNTTHREAD_H

#include <QThread>
#include <QVector>
#include <QItemSelectionModel>

class CountThread : public QThread
{
    Q_OBJECT
public:
    explicit CountThread(QObject *parent = 0);

signals:
    void estimateGasSpectorStarted(QVector<int> U, QModelIndexList M);

    void stopc();
public slots:
    void estimateGasSpector(QVector<int> U, QModelIndexList M);

    void stoc();
};

#endif // COUNTTHREAD_H
