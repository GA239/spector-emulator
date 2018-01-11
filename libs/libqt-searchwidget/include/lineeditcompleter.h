#ifndef CustomCompleter_H
#define CustomCompleter_H

#include <QObject>
#include <QCompleter>


///
/// \brief The CustomCompleter class
///
/// This class will need to be if you want to extend
/// the functionality of autocompletion
///
class LineEditCompleter : public QCompleter
{
    Q_OBJECT
public:
    LineEditCompleter(QAbstractItemModel * model, QObject * parent = 0);
    LineEditCompleter(const QStringList  &list, QObject * parent = 0);
    LineEditCompleter(QObject * parent = 0);
    bool eventFilter(QObject *object, QEvent *event);


signals:
    void completeFinished(QModelIndex index);

private slots:
    void onActivated(QModelIndex index);

private:

protected:
    void timerEvent(QTimerEvent *event);

private:
    int timerId;
    QModelIndex activatedIndex;

    QWidget* wig;

};

#endif // CustomCompleter_H
