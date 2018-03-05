#include <QtGui>
#include <QGridLayout>
#include <QPushButton.h>
#include "chartlayoutdialog.h"


ChartDialog::ChartDialog(QWidget* pwgt/*= 0*/) 
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{   

    this->setWindowTitle(tr("select chart layout"));
    this->setMinimumSize(200, 150);

    QPushButton* vertbtm = new QPushButton(QIcon(":/images/vertical.png"),"", this);
    vertbtm->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    connect(vertbtm, SIGNAL(clicked()), SLOT(accept()));
    connect(vertbtm, SIGNAL(clicked()), this, SLOT(verBtmPressed()));

    QPushButton* horbtm = new QPushButton(QIcon(":/images/horizontal.png"),"", this);
    horbtm->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    connect(horbtm, SIGNAL(clicked()), SLOT(accept()));
    connect(horbtm, SIGNAL(clicked()), this, SLOT(horBtmPressed()));

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout(this);
    ptopLayout->addWidget(vertbtm, 1,0);
    ptopLayout->addWidget(horbtm, 2,0);
    setLayout(ptopLayout);
}

void ChartDialog::horBtmPressed()
{
    emit buttomNumber(0);
}

void ChartDialog::verBtmPressed()
{
    emit buttomNumber(1);
}
