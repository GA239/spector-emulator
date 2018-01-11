#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QGraphicsView>
#include <QWidget>
#include <QLineEdit>
#include <QStringListModel>
#include <QPaintEngine>
#include <QItemSelectionModel>
#include <QTimer>
#include "searchline.h"
#include "lineeditcompleter.h"
#include "flowlayout.h"
#include "tagcompleteritemdelegate.h"
#include "tagbutton.h"
#include "closebutton.h"


class FlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FlowWidget(QWidget *parent = 0);
    virtual ~FlowWidget();
    void resizeEvent ( QResizeEvent * event );
    int minimumHeight();
    int minimumWidth();
    QSize minimumSize();
    QSize sizeHint();
    //! [2] Custom public methods
    void addTag(const QString &data);
    void addTag(const QModelIndex index);
    void addTagButton(TagButton *tag);
    void deleteTagButton(TagButton *tag);
    TagButton *getTagByIndex(const QModelIndex index);

    //! [2]
    void setModel(QAbstractItemModel *model);
    void setSelectionModel(QItemSelectionModel *selModel);
    QItemSelectionModel* selectionModel() const;
    QModelIndexList tags(void);
    QStringList unfindedTags();
    void setEnableNewTagCreation(bool status);

public slots:
    void onDeleteTagClick(TagButton *tag);
    void onTagSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onCompleterFinished(QModelIndex proxyIndex);
    void onReturnPressed(void);
    void mousePressed(QModelIndex index);
    void onSearchTextChanged(QString text);
    void setSearchText(QString text);
    void changeCurrentIndex(QModelIndex proxyIndex);
    void clear(void);

protected:
    void paintEvent(QPaintEvent *event);

private:
   void calcSize(void);
   void insertSelection(QModelIndex index);
   int calcTagsSpace(void);

private:
    SearchLine *lineEdit;
    CloseButton *closeButton;
    LineEditCompleter *lineEditCompleter;
    QAbstractItemModel *model;
    QItemSelectionModel *selModel;
    TagCompleterItemDelegate *tagCompleterItemDelegate;
    FlowLayout *flowLayout;
    int buttonPadding;
    bool enableNewTagCreation;

    int lineEditWidth;
    int widgetHeight;
    int fixedSpace;
    int tagSpace;
    int tagRowNumber;
    int verticalSpacing;
    int horizontalSpacing;
    int childrenNumber;

    QModelIndex currentIndex;
};

#endif // SEARCH_H
