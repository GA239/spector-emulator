#ifndef TAGCOMPLETERITEMDELEGATE_H
#define TAGCOMPLETERITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class TagCompleterItemDelegate : public QStyledItemDelegate
{

    Q_OBJECT

public:
    TagCompleterItemDelegate(QObject *parent=0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

signals:
    void mouseOverIndex(QString text) const;

private:
};

#endif // TAGCOMPLETERITEMDELEGATE_H
