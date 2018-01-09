#include <QDebug>
#include <QGridLayout>
#include "controllsewidget.h"

#define U_MIN_VALUE  0
#define U_MAX_VALUE 100

/**
 * @brief Default constructor
 * @param parent
 */
ControllSEWidget::ControllSEWidget(QWidget *parent) : QWidget(parent)
{

    this->U1 = new QScrollBar(Qt::Horizontal,parent);
    this->U2 = new QScrollBar(Qt::Horizontal,parent);
    this->U1->setRange(U_MIN_VALUE,U_MAX_VALUE);
    this->U2->setRange(U_MIN_VALUE,U_MAX_VALUE);

    QObject::connect(this->U1, SIGNAL(sliderMoved(int)), this, SLOT(U1ChangedPrivateSlot(int)));
    QObject::connect(this->U2, SIGNAL(sliderMoved(int)), this, SLOT(U2ChangedPrivateSlot(int)));

    QLabel* labelU1 = new QLabel("<font size=24><b>U1</b></font>",parent);
    labelU1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QLabel* labelU2 = new QLabel("<font size=24><b>U2</b></font>",parent);
    labelU2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    this->U1->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    this->U2->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    QGridLayout *layout = new QGridLayout(this);

    QHBoxLayout *horizontLayoutU1 = new QHBoxLayout(parent);
    horizontLayoutU1->addWidget(this->U1);
    horizontLayoutU1->addWidget(labelU1);

    QHBoxLayout *horizontLayoutU2 = new QHBoxLayout(parent);
    horizontLayoutU2->addWidget(this->U2);
    horizontLayoutU2->addWidget(labelU2);

    layout->addLayout(horizontLayoutU1,0,0);
    layout->addLayout(horizontLayoutU2,1,0);

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

void ControllSEWidget::U1ChangedPrivateSlot(int value)
{
    emit this->U1Changed(value);
}

void ControllSEWidget::U2ChangedPrivateSlot(int value)
{
    emit this->U2Changed(value);
}
