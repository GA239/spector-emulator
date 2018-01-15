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

    //����� ��� ����������� (������) �������� ������� �����-�����
    DataGenerator generator;

    // �����, � ������� ����� ����������� �������
    CountThread worker;

    // ��������� ���������� �������� � �����
    generator.moveToThread(&worker);

    // ���������� ���������� � ��������� � �������� ����
    QObject::connect(&generator, SIGNAL(progressChanged(int)), &w, SLOT(progressSet(int)));
    // ���������� �������� �����������
    QObject::connect(&generator, SIGNAL(SendResults(QVector<double>)), &w, SLOT(GetResults(QVector<double>)));

    // ����� ���������� ������ ���������� - ����� ������ �����������
    QObject::connect(&generator, SIGNAL(done()), &worker, SLOT(quit()));

    // ��������� ������ �������� � �������� ������
    QObject::connect(&w, SIGNAL(estimateGasSpector(int,int,QVector<int>)),&worker, SLOT(estimateGasSpector(int,int,QVector<int>)));
    QObject::connect(&worker, SIGNAL(estimateGasSpectorStarted(int,int,QVector<int>)), &generator, SLOT(estimateGasSpector(int,int,QVector<int>)));

    // ������������ ���������� ������ �� ������� ������ Stop
    //QObject::connect(&w, SIGNAL(Stop()), &worker, SLOT(stoc()));
    //QObject::connect(&worker, SIGNAL(stopc()), &generator, SLOT(stopcc()));

    return a.exec();
}
