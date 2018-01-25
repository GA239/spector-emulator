#ifndef CONTROLLSEWIDGET_H
#define CONTROLLSEWIDGET_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include "../../libs/libqt-searchwidget/include/searchwidget.h"

class ControllSEWidget : public QWidget
{
    Q_OBJECT

public:
    ControllSEWidget(QWidget *parent = 0);
    ~ControllSEWidget();
    int getU(int index);
    void setModelToSearchWidget(QAbstractItemModel *model);
    QModelIndexList getTagsFromSearchWidget();
    QStringList getTagsFromSearchWidgetAsStringList();
    QVector<int> getUvalues();
    void setUvalues(QVector<int> values);
    void addTags(QModelIndexList indexList);
    void addTags(QStringList gaseList);

signals:
    void Changed();

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void updateULables(int);

private:

    QString getValueStrForUBar(int uIndex, int value);

    QVector<QScrollBar*> U;
    QVector<QLabel*> ULable;
    SearchWidget *searchWidget;

};

#endif // CONTROLLSEWIDGET_H
