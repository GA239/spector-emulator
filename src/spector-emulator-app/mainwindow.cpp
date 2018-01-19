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
    generator->deleteLater();
    delete this-> worker;
    delete ui;
}

void MainWindow::createThreadEstimation(void)
{
    // Connect progress notifications to progress bar
    QObject::connect(generator, SIGNAL(progressChanged(int)), this, SLOT(progressSet(int)));
    // Connect the transfer of results
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(generator, SIGNAL(SendResults(QVector<double>)), this, SLOT(GetResults(QVector<double>)));

    //  огда выполнение работы закончитс€ - поток должен завершитьс€
    QObject::connect(generator, SIGNAL(done()), worker, SLOT(quit()));
    QObject::connect(generator, SIGNAL(done()), this, SLOT(updateProgressbar()));

    // соедин€ем начало расчЄтов с запуском потока
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QModelIndexList >("QModelIndexList");
    QObject::connect(this, SIGNAL(estimateGasSpector(QVector<int>,QModelIndexList)),worker, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));
    QObject::connect(worker, SIGNAL(estimateGasSpectorStarted(QVector<int>,QModelIndexList)), generator, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));
    //прерывание
    QObject::connect(worker, SIGNAL(stopEstimationSignal()), generator, SLOT(stopEstimation()));
    //QObject::connect(worker, SIGNAL(stopEstimationSignal()), generator, SLOT(stopEstimation()));

    // transfer the calculations to the thread
    generator->moveToThread(worker);
}

void MainWindow::createAndConfigureElemtsOfWindow()
{
    ///* data generator *///
    //class for parallel (heavy) estimations
    DataGenerator::bobo = false;
    this->generator =  new DataGenerator() ;

    //The flow in which the estimation will be performed
    this->worker = new CountThread();

    //connect dataestimation with additional thread
    this->createThreadEstimation();

    ///* controll widget *///
    this->controll =  new ControllSEWidget();
    QStringListModel *model = new QStringListModel(this);
    //model->setStringList(QStringList() << "Item1" << "Item2" << "Item3" << "C" << "C++" << "C#" << "C++" << "php" << "qt");
    model->setStringList(this->generator->getModelElemnts());
    this->controll->setModelToSearchWidget(model);
    //QObject::connect(this->controll, SIGNAL(Changed()), this, SLOT(ControllChanged()));

    ///* plot widget *///
    this->plotter =  new PlotSEWidget();

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

void MainWindow::bobotreu()
{
    DataGenerator::bobo = true;
}

void MainWindow::updateProgressbar()
{
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Estimate");
    QObject::disconnect(this->startEstimateBottom, SIGNAL(clicked()), worker, SLOT(stopEstimation()));

}


void MainWindow::EstimateBottomPressed()
{
    // ќбеспечиваем прерывание потока по нажатию кнопки Stop
    QObject::disconnect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Stop");
    //QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), worker, SLOT(stopEstimation()));
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(bobotreu()));

    //QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), generator, SLOT(stopEstimation()));

    emit estimateGasSpector(this->controll->getUvalues(),this->controll->getTagsFromSearchWidget());
}
