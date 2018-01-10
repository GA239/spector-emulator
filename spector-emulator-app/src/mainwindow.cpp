#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->controll =  new ControllSEWidget(parent);

    QGridLayout *layout = new QGridLayout(parent);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(parent);
    horizontalLayout->addWidget(this->controll);
    QObject::connect(this->controll, SIGNAL(U1Changed(int)), this, SLOT(U1ControllChanged(int)));
    QObject::connect(this->controll, SIGNAL(U2Changed(int)), this, SLOT(U2ControllChanged(int)));

    labelU1dem = new QLabel(parent);
    labelU1dem->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    labelU2dem = new QLabel(parent);
    labelU2dem->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    setValueForU1Lable(0);
    setValueForU2Lable(0);

    QVBoxLayout *horizontLayoutdem = new QVBoxLayout(parent);
    horizontLayoutdem->addWidget(labelU1dem);
    horizontLayoutdem->addWidget(labelU2dem);

    layout->addLayout(horizontalLayout,0,0);
    layout->addLayout(horizontLayoutdem,0,1);
    this->ui->centralWidget->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete this->controll;
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
    QString str = "<font size=24><b>U1 = " + QString::number(value) + " </b></font>";
    this->labelU1dem->setText(str);
}

void MainWindow::setValueForU2Lable(int value)
{
    QString str = "<font size=24><b>U2 = " + QString::number(value) + " </b></font>";
    this->labelU2dem->setText(str);
}
