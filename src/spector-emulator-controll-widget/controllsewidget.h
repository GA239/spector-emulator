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
    int getU1(void);
    int getU2(void);
    void setModelToSearchWidget(QAbstractItemModel *model);

signals:
    void Changed();

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:

private:
    QScrollBar* U1;
    QScrollBar* U2;
    SearchWidget *searchWidget;

};

#endif // CONTROLLSEWIDGET_H
