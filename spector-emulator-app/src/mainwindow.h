#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../spector-emulator-controll-widget/src/controllsewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    //void test(int position);


private slots:
    void U1ControllChanged(int value);
    void U2ControllChanged(int value);

private:

    void setValueForU1Lable(int value);
    void setValueForU2Lable(int value);

    Ui::MainWindow *ui;
    ControllSEWidget* controll;
    QLabel* labelU1dem;
    QLabel* labelU2dem;
};

#endif // MAINWINDOW_H
