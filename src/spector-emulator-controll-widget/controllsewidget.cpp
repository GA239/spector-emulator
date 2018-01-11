#include <QDebug>
#include <QGridLayout>
#include "controllsewidget.h"

#define U_MIN_VALUE 250
#define U_MAX_VALUE 1000

/**
 * @brief Default constructor
 * @param parent
 */
ControllSEWidget::ControllSEWidget(QWidget *parent) : QWidget(parent)
{

    this->U1 = new QScrollBar(Qt::Horizontal,parent);
    this->U1->setRange(U_MIN_VALUE,U_MAX_VALUE);
    this->U1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    QObject::connect(this->U1, SIGNAL(sliderMoved(int)), this, SIGNAL(U1Changed(int)));
    QLabel* labelU1 = new QLabel("<font size=12><b>U1</b></font>",parent);
    labelU1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    this->U2 = new QScrollBar(Qt::Horizontal,parent);
    this->U2->setRange(U_MIN_VALUE,U_MAX_VALUE);
    this->U2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    QObject::connect(this->U2, SIGNAL(sliderMoved(int)), this, SIGNAL(U2Changed(int)));
    QLabel* labelU2 = new QLabel("<font size=12><b>U2</b></font>",parent);
    labelU2->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    // устанавливаем цвет фона
    QPalette Pal(palette());
    Pal.setColor(QPalette::Base, Qt::white);
    this->U2->setAutoFillBackground(true);
    this->U2->setPalette(Pal);
    this->U2->setBackgroundRole(QPalette::Light);


    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(this->U1,0,0);
    layout->addWidget(labelU1,0,1);

    layout->addWidget(this->U2,1,0);
    layout->addWidget(labelU2,1,1);
 }

/**
 * @brief ControllSEWidget::~ControllSEWidget
 */
ControllSEWidget::~ControllSEWidget()
{

}

int ControllSEWidget::getU1()
{
    return this->U1->value();
}

int ControllSEWidget::getU2()
{
    return this->U2->value();
}

/**
 * @brief Action is called when key press widget event ocurs
 * @param event
 */
void ControllSEWidget::keyPressEvent(QKeyEvent *event)
{
     Q_UNUSED(event);
    /*
    switch (event->key()) {
        case Qt::Key_Escape: {
            this->showInWindow();
        } break;
        case Qt::Key_Space:
        case Qt::Key_PageUp: {
            this->showNext();
        } break;
        case Qt::Key_PageDown: {
            this->showPrev();
        } break;
        default: break;
    }*/
    return;
}

/**
 * @brief Action is called when mouse wheel widget event ocurs
 * @param event
 */
void ControllSEWidget::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
    return;
}

