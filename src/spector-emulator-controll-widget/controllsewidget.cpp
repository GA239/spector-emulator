#include <QDebug>
#include <QGridLayout>
#include "controllsewidget.h"

#define U_MIN_VALUE 250
#define U_MAX_VALUE 999
#define U_NUMBER 3

/**
 * @brief Default constructor
 * @param parent
 */
ControllSEWidget::ControllSEWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    for(int i = 0; i < U_NUMBER; ++i)
    {
        QScrollBar* sb = new QScrollBar(Qt::Horizontal,parent);
        this->U.push_back(sb);
        this->U[i]->setRange(U_MIN_VALUE,U_MAX_VALUE);
        this->U[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        QObject::connect(this->U[i], SIGNAL(valueChanged(int)), this, SIGNAL(Changed()));

        QLabel* label = new QLabel(getValueStrForUBar(i+1,U_MIN_VALUE),parent);
        this->ULable.push_back(label);
        this->ULable[i]->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        QObject::connect(this->U[i], SIGNAL(valueChanged(int)), this, SLOT(updateULables(int)));

        layout->addWidget(this->U[i],i,0);
        layout->addWidget(this->ULable[i],i,1);
    }

    ///* search widget *///
    this->searchWidget = new SearchWidget();
    this->searchWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    layout->addWidget(this->searchWidget,U_NUMBER,0,1,2);
}

/**
 * @brief ControllSEWidget::~ControllSEWidget
 */
ControllSEWidget::~ControllSEWidget()
{
    delete this->searchWidget;
    for(int i = 0; i < U_NUMBER; ++i)
    {
        delete this->U[i];
        delete this->ULable[i];
    }
}

int ControllSEWidget::getU(int index)
{
    if(index < U_NUMBER)
        return this->U[index]->value();
    return -1;
}
QString ControllSEWidget::getValueStrForUBar(int uIndex, int value)
{
    float font = 12;
    QString result = "<font size=" + QString::number(font) + "><b>U" + QString::number(uIndex) + " = " + QString::number(value) + "</b></font>";
    return result;
}

void ControllSEWidget::updateULables(int value)
{
    Q_UNUSED(value);
    for(int i = 0; i < U_NUMBER; ++i)
        this->ULable[i]->setText(getValueStrForUBar(i+1,this->U[i]->value()));
}

void ControllSEWidget::setModelToSearchWidget(QAbstractItemModel *model)
{
    this->searchWidget->setModel(model);
}

QModelIndexList ControllSEWidget::getTagsFromSearchWidget()
{
    return this->searchWidget->tags();
}

QVector<int> ControllSEWidget::getUvalues()
{
    QVector<int> result;
    for(int i = 0; i < U_NUMBER; ++i)
        result.push_back(this->U[i]->value());
    return result;
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

