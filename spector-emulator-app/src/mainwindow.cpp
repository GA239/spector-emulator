#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->controll =  new ControllSEWidget();
    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->ui->centralWidget->setLayout(verticalLayout);
    this->ui->centralWidget->layout()->addWidget(this->controll);

}

MainWindow::~MainWindow()
{
    delete this->controll;
    delete ui;
}
