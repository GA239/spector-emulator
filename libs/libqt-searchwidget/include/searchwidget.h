#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QScrollBar>
#include <QScrollArea>
#include "flowwidget.h"

class SearchWidget : public QScrollArea
{
    Q_OBJECT

public:
    SearchWidget();
    void setModel(QAbstractItemModel *model);
    void setSelectionModel(QItemSelectionModel *selModel);
    QStringList unfindedTags();
    QModelIndexList tags();
    QItemSelectionModel* selectionModel() const;
    void setEnableNewTagCreation(bool status);
    TagButton *getTagByIndex(const QModelIndex index);

    void addTags(QModelIndexList indexList);
    void addTags(QStringList list);
    QModelIndexList modelElements();
    void clearWidget();


public slots:
    void moveScrollBarToBottom(int min, int max);

private:
    FlowWidget *flowWidget;
};

#endif // SEARCHWIDGET_H
