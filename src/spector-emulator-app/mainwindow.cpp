#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("spector-emulator");

    /*// устанавливаем цвет фона
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);/*/

    this->controll =  new ControllSEWidget(parent);
    this->plotter =  new PlotSEWidget(parent);
    this->plotter->setPlotData(estemateData(this->controll->getU1() + 2,this->controll->getU2() + 2));
    this->generator = new DataGenerator();

    QGridLayout *layout = new QGridLayout(parent);

    QObject::connect(this->controll, SIGNAL(U1Changed(int)), this, SLOT(U1ControllChanged(int)));
    QObject::connect(this->controll, SIGNAL(U2Changed(int)), this, SLOT(U2ControllChanged(int)));

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

    layout->addWidget(this->controll,0,0,1,1);
    layout->addWidget(this->searchWidget,1,0,1,1);
    layout->addWidget(this->plotter,0,1,2,2);
    this->ui->centralWidget->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete this->controll;
    delete this->plotter;
    delete this->generator;
    delete this->searchWidget;
    delete ui;
}

void MainWindow::U1ControllChanged(int value)
{
    setValueForU1Lable(value);
}

void MainWindow::U2ControllChanged(int value)
{
    setValueForU2Lable(value);
}

void MainWindow::setValueForU1Lable(int value)
{
    this->plotter->setPlotData(estemateData(value,this->controll->getU2()));
}

void MainWindow::setValueForU2Lable(int value)
{
    this->plotter->setPlotData(estemateData(this->controll->getU1(),value));
    this->update();
}

QVector<double> MainWindow::estemateData(int u1, int u2)
{
    return this->generator->estemateData(u1,u2);
}
