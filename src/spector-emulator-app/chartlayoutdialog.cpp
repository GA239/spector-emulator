#include <QtGui>
#include "chartlayoutdialog.h"

ChartDialog::ChartDialog(QWidget* pwgt/*= 0*/) 
     : QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{   


    QPushButton* pcmdOk     = new QPushButton("&Vert");
    QPushButton* pcmdCancel = new QPushButton("&horizontal");

    connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()), SLOT(accept()));

    //Layout setup
    QGridLayout* ptopLayout = new QGridLayout;
    ptopLayout->addWidget(pcmdOk, 1,0);
    ptopLayout->addWidget(pcmdCancel, 2, 0);
    setLayout(ptopLayout);
}