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

    this->controll =  new ControllSEWidget(parent);
    this->plotter =  new PlotSEWidget(parent);

    QGridLayout *layout = new QGridLayout(parent);

    //QObject::connect(this->controll, SIGNAL(UChanged()), this, SLOT(ControllChanged()));

    ///* search widget *///
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(QStringList() << "Item1" << "Item2" << "Item3" << "C" << "C++" << "C#" << "C++" << "php" << "qt");

    QListView *listView = new QListView();
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listView->setModel(model);

    this->searchWidget = new SearchWidget();
    this->searchWidget->setModel(model);
    this->searchWidget->setSelectionModel(listView->selectionModel());
    this->searchWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    this->progressBar = new QProgressBar(parent);
    this->progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->progressBar->setRange(0, 100);

    this->startEstimateBottom = new QPushButton(parent);
    this->startEstimateBottom->setText("Estimate");
    this->startEstimateBottom->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));

    layout->addWidget(this->controll,0,0);
    layout->addWidget(this->searchWidget,1,0);
    layout->addWidget(this->plotter,0,1,2,3);
    layout->addWidget(this->progressBar,2,0,1,2);
    layout->addWidget(this->startEstimateBottom,2,2);

    this->ui->centralWidget->setLayout(layout);

    //this->plotter->setPlotData(estemateData(this->controll->getU1(),this->controll->getU2()));

}

MainWindow::~MainWindow()
{
    delete this->controll;
    delete this->plotter;
    delete this->searchWidget;
    delete ui;
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

void MainWindow::estemateNewValues()
{
    this->plotter->setPlotData(estemateData(this->controll->getU1(),this->controll->getU2()));
    this->update();
}

QVector<double> MainWindow::estemateData(int u1, int u2)
{
    QVector<int> M;
    M.push_back(28);
    M.push_back(32);
    M.push_back(14);
    QVector<double> result(10);
    result.fill(1.0);
    return result;
    //return this->generator->getData(u1,u2,M);
}
