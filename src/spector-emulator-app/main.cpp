#include "mainwindow.h"
#include "countthread.h"
#include "../spector-emulator-data-generator/datagenerator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QVector<double> >("QVector<double>");

    MainWindow w;
    w.show();

    //класс для паралельных (тяжёлых) расчётов методом Монте-Карло
    DataGenerator generator;

    // Поток, в котором будут выполняться расчёты
    CountThread worker;

    // Переносим выполнение расчётов в поток
    generator.moveToThread(&worker);

    // Подключить оповещения о прогрессе к прогресс бару
    QObject::connect(&generator, SIGNAL(progressChanged(int)), &w, SLOT(progressSet(int)));
    // Подключить передачу результатов
    QObject::connect(&generator, SIGNAL(SendResults(QVector<double>)), &w, SLOT(GetResults(QVector<double>)));

    // Когда выполнение работы закончится - поток должен завершиться
    QObject::connect(&generator, SIGNAL(done()), &worker, SLOT(quit()));

    // соединяем начало расчётов с запуском потока
    QObject::connect(&w, SIGNAL(estimateGasSpector(int,int,QVector<int>)),&worker, SLOT(estimateGasSpector(int,int,QVector<int>)));
    QObject::connect(&worker, SIGNAL(estimateGasSpectorStarted(int,int,QVector<int>)), &generator, SLOT(estimateGasSpector(int,int,QVector<int>)));

    // Обеспечиваем прерывание потока по нажатию кнопки Stop
    //QObject::connect(&w, SIGNAL(Stop()), &worker, SLOT(stoc()));
    //QObject::connect(&worker, SIGNAL(stopc()), &generator, SLOT(stopcc()));

    return a.exec();
}
