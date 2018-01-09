#include <QDebug>
#include <QVBoxLayout>

#include "controllsewidget.h"

/**
 * @brief Default constructor
 * @param parent
 */
ControllSEWidget::ControllSEWidget(QWidget *parent) : QWidget(parent)
{
    /*
    this->imageLabel = new QLabel(this);
    this->imageLabel->setBackgroundRole(QPalette::Base);
    this->imageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->imageLabel->setScaledContents(true);
    this->imageLabel->setAlignment(Qt::AlignCenter);
    this->imageLabel->setFocus();

    this->setFrameStyle(QFrame::Plain);
    this->setLineWidth(0);
    this->setBackgroundRole(QPalette::Window);
    this->setWidget(this->imageLabel);
    this->setAlignment(Qt::AlignCenter);

    this->scaleMode = this->SCALEMODE_FIT_TO_WINDOW;
    this->scaleFactor = 1.;
    this->modelPathRole = Qt::DisplayRole;

    */
    this->U1 = new QScrollBar(Qt::Horizontal,parent);
    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);
    this->layout()->addWidget(this->U1);

 }

/**
 * @brief ControllSEWidget::~ControllSEWidget
 */
ControllSEWidget::~ControllSEWidget()
{
    delete this->U1;
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
    /*
    if(event->delta() > 0) {
        this->showNext();
    } else {
        this->showPrev();
    }*/
    return;
}
