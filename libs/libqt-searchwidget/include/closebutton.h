#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
//#include <QModelIndex>


class CloseButton : public QWidget
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent = 0);
    ~CloseButton();

    int minimumWidth() const;
    int minimumHeight() const;
    QSize sizeHint() const;

signals:
    void pressed(void);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    void calcSize(void);

private:

    int widgetWidth;
    int widgetHeight;
    QRect closerRect;
    int buttonPadding;
};

#endif // CLOSEBUTTON_H
