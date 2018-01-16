#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("spector-emulator");

    this->createAndConfigureElemtsOfWindow();
    this->setwidgetAssembly();
}

MainWindow::~MainWindow()
{
    delete this->controll;
    delete this->plotter;
    delete this-> startEstimateBottom;
    delete this-> progressBar;
    delete this-> generator;
    delete this-> worker;
    delete ui;
}

void MainWindow::createThreadEstimation(void)
{
    // transfer the calculations to the thread
    generator->moveToThread(worker);

    // Connect progress notifications to progress bar
    QObject::connect(generator, SIGNAL(progressChanged(int)), this, SLOT(progressSet(int)));
    // Connect the transfer of results
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(generator, SIGNAL(SendResults(QVector<double>)), this, SLOT(GetResults(QVector<double>)));

    //  огда выполнение работы закончитс€ - поток должен завершитьс€
    QObject::connect(generator, SIGNAL(done()), worker, SLOT(quit()));

    // соедин€ем начало расчЄтов с запуском потока
    qRegisterMetaType<QVector<int> >("QVector<int>");
    QObject::connect(this, SIGNAL(estimateGasSpector(int,int,QVector<int>)),worker, SLOT(estimateGasSpector(int,int,QVector<int>)));
    QObject::connect(worker, SIGNAL(estimateGasSpectorStarted(int,int,QVector<int>)), generator, SLOT(estimateGasSpector(int,int,QVector<int>)));

    // ќбеспечиваем прерывание потока по нажатию кнопки Stop
    //QObject::connect(&w, SIGNAL(Stop()), &worker, SLOT(stoc()));
    //QObject::connect(&worker, SIGNAL(stopc()), &generator, SLOT(stopcc()));
}

void MainWindow::createAndConfigureElemtsOfWindow()
{
    ///* controll widget *///
    this->controll =  new ControllSEWidget();
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(QStringList() << "Item1" << "Item2" << "Item3" << "C" << "C++" << "C#" << "C++" << "php" << "qt");
    this->controll->setModelToSearchWidget(model);
    //QObject::connect(this->controll, SIGNAL(Changed()), this, SLOT(ControllChanged()));

    ///* plot widget *///
    this->plotter =  new PlotSEWidget();

    ///* data generator *///
    //class for parallel (heavy) estimations
    this->generator =  new DataGenerator();

    //The flow in which the estimation will be performed
    this->worker = new CountThread();

    //connect dataestimation with additional thread
    this->createThreadEstimation();

    ///* estimation status and controll widgets *///
    this->progressBar = new QProgressBar();
    this->progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->progressBar->setRange(0, 100);

    this->startEstimateBottom = new QPushButton();
    this->startEstimateBottom->setText("Estimate");
    this->startEstimateBottom->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
}

void MainWindow::setwidgetAssembly()
{
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(this->controll,0,0);
    layout->addWidget(this->plotter,0,1,2,3);
    layout->addWidget(this->progressBar,2,0,1,2);
    layout->addWidget(this->startEstimateBottom,2,2);

    this->ui->centralWidget->setLayout(layout);
}


void MainWindow::progressSet(int value)
{
    this->progressBar->setValue(value);
}

void MainWindow::GetResults(QVector<double> results)
{
    this->plotter->setPlotData(results);
    this->update();
}


void MainWindow::EstimateBottomPressed()
{
    QVector<int> M;
    M.push_back(28);
    M.push_back(32);
    M.push_back(14);
    emit estimateGasSpector(this->controll->getU1(),this->controll->getU2(),M);
}
