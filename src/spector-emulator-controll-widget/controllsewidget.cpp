#include <QDebug>
#include <QGridLayout>
#include "controllsewidget.h"

#define U_MIN_VALUE 200
#define U_MAX_VALUE 999
#define U_NUMBER 3

/**
 * @brief Default constructor
 * @param parent
 */
ControllSEWidget::ControllSEWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    for(int i = 0; i < U_NUMBER; ++i){
        // add scroll bar for each gas
        QSlider* sb = new QSlider(Qt::Horizontal,parent);
        this->U.push_back(sb);
        this->U[i]->setRange(U_MIN_VALUE,U_MAX_VALUE);
        this->U[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        QObject::connect(this->U[i], SIGNAL(valueChanged(int)), this, SIGNAL(Changed()));

        // add label bar for each gas
        QLabel* label = new QLabel(getValueStrForUBar(i+1,U_MIN_VALUE),parent);
        this->ULable.push_back(label);
        this->ULable[i]->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        QObject::connect(this->U[i], SIGNAL(valueChanged(int)), this, SLOT(updateULables(int)));

        layout->addWidget(this->U[i],i,0);
        layout->addWidget(this->ULable[i],i,1);
    }

    ///* search widget *///
    this->searchWidget = new SearchWidget();
    this->searchWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    layout->addWidget(this->searchWidget,U_NUMBER,0,10,2);
}

/**
 * @brief ControllSEWidget::~ControllSEWidget
 */
ControllSEWidget::~ControllSEWidget()
{
    delete this->searchWidget;
    for(int i = 0; i < U_NUMBER; ++i){
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
    return  "U" + QString::number(uIndex) + " = " + QString::number(value);
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

QStringList ControllSEWidget::getTagsFromSearchWidgetAsStringList()
{
    QStringList result;
    QModelIndexList tmp = this->searchWidget->tags();
    for(int i = 0; i < tmp.length(); ++i)
        result.append(tmp[i].data().toString());
    return result;
}

QVector<int> ControllSEWidget::getUvalues()
{
    QVector<int> result;
    for(int i = 0; i < U_NUMBER; ++i)
        result.push_back(this->U[i]->value());
    return result;
}

void ControllSEWidget::setUvalues(QVector<int> values)
{
    if(values.size() == U_NUMBER){
        for(int i = 0; i < U_NUMBER; ++i)
            this->U[i]->setValue(values[i]);
    }
    emit Changed();
}

void ControllSEWidget::addTags(QModelIndexList indexList)
{
    this->searchWidget->addTags(indexList);
    emit Changed();
}

void ControllSEWidget::addTags(QStringList gaseList)
{
    this->searchWidget->clearWidget();
    QModelIndexList tmpList = this->searchWidget->modelElements();
    QModelIndexList resultList;

    for(int i = 0; i < gaseList.length(); ++i){
        for(int j = 0; j < tmpList.length(); ++j){
            if(gaseList[i] == tmpList[j].data().toString()){
                resultList.append(tmpList[j]);
                break;
            }
        }
    }
    this->searchWidget->addTags(resultList);
    emit Changed();
}
