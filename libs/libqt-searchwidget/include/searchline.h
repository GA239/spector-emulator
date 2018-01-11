#ifndef SEARCHLINE_H
#define SEARCHLINE_H
#include <QLineEdit>


class SearchLine : public QLineEdit
{
public:
    SearchLine();
    ~SearchLine();
    int minimumWidth() const;
    int minimumHeight() const;
    QSize sizeHint() const;

private:
    void calcSize(void);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int widgetWidth;
    int widgetHeight;
    int buttonPadding;
};

#endif // SEARCHLINE_H
