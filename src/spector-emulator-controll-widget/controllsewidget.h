#ifndef CONTROLLSEWIDGET_H
#define CONTROLLSEWIDGET_H

#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include "../../libs/libqt-searchwidget/include/searchwidget.h"

///
/// \brief The ControllSEWidget class providing input of parameters for calculating the gas spectrum
///
class ControllSEWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControllSEWidget(QWidget *parent = 0);
    ~ControllSEWidget();
    ///
    /// \brief getU - returns the voltage value for a specific scrollbar
    /// \param index - number of scrollbar
    /// \return voltage
    ///
    int getU(int index);
    ///
    /// \brief setModelToSearchWidget - initializes the gas selection widget model
    /// \param model
    ///
    void setModelToSearchWidget(QAbstractItemModel *model);
    ///
    /// \brief getTagsFromSearchWidget returns the model objects that are currently selected
    /// \return
    ///
    QModelIndexList getTagsFromSearchWidget();
    QStringList getTagsFromSearchWidgetAsStringList();
    ///
    /// \brief getUvalues returns the voltages that are currently selected
    /// \return
    ///
    QVector<int> getUvalues();
    ///
    /// \brief setUvalues sets the values of voltage
    /// \param values
    ///
    void setUvalues(QVector<int> values);
    ///
    /// \brief addTags installs in the search widget a list of gases for emulation
    /// \param indexList - list of model objects
    ///
    void addTags(QModelIndexList indexList);
    ///
    /// \brief addTags installs in the search widget a list of gases for emulation
    /// \param gaseList - list of gas names
    ///
    void addTags(QStringList gaseList);
signals:
    ///
    /// \brief Changed signals a widget change
    ///
    void Changed();

private slots:
    ///
    /// \brief updateULables - updates signatures to scroll bars
    ///
    void updateULables(int);

private:
    QString getValueStrForUBar(int uIndex, int value);

    QVector<QSlider*> U;
    QVector<QLabel*> ULable;
    SearchWidget *searchWidget;
};

#endif // CONTROLLSEWIDGET_H
