#ifndef CONTROLLSEWIDGET_H
#define CONTROLLSEWIDGET_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>

class ControllSEWidget : public QWidget
{
    Q_OBJECT

public:
    ControllSEWidget(QWidget *parent = 0);
    ~ControllSEWidget();
    int getU1(void);
    int getU2(void);

signals:
    void U1Changed(int position);
    void U2Changed(int position);

public slots:

protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:

private:
    QScrollBar* U1;
    QScrollBar* U2;


};

#endif // CONTROLLSEWIDGET_H
