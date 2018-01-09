#ifndef CONTROLLSEWIDGET_H
#define CONTROLLSEWIDGET_H

#include <QWidget>
#include <QScrollBar>

class ControllSEWidget : public QWidget
{
    Q_OBJECT

public:
    ControllSEWidget(QWidget *parent = 0);
    ~ControllSEWidget();

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
private:

    QScrollBar* U1;
};

#endif // CONTROLLSEWIDGET_H
