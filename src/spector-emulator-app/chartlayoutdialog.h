#ifndef _ChartDialog_h_
#define _ChartDialog_h_

#include <QDialog>

class ChartDialog : public QDialog {
    Q_OBJECT

public:
    ChartDialog(QWidget* pwgt = 0);
	
signals:	
	void buttomNumber(int);

private slots:


};
#endif  //_ChartDialog_h_