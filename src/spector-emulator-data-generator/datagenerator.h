#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QVector>
#include <QMap>

///
/// \brief The Params struct - structure with parameters of gas spectrum calculation
///
struct Params
{
    Params(double _x, QVector<int> _M, double _Uis, double _Um1, double _Um2, double _Uik):
           x(_x), M(_M), Uis(_Uis), Um1(_Um1), Um2(_Um2), Uik(_Uik) {}
    double x;
    QVector<int> M;
    double Uis;
    double Um1;
    double Um2;
    double Uik;

};

///
/// \brief The DataGenerator class - class realizing algorithms for calculating the gas sector
///
class DataGenerator: public QObject
{
    Q_OBJECT

public:
    explicit DataGenerator(QObject *parent = 0);
    ~DataGenerator();
    ///
    /// \brief getModelElemnts
    /// \return the elements of the model i.e. names of gases that can be used for calculation
    ///
    QStringList getModelElemnts();
    /// flag responsible for interrupting the calculation process
    static bool stopEstimate;

signals:
    ///
    /// \brief progressChanged - the signal indicates the current stage of the calculation process
    /// \param progress - stage
    ///
    void progressChanged(int progress);
    ///
    /// \brief SendResults - the signal sends the results of calculations
    /// \param results values of gas sppectrum
    ///
    void SendResults(QVector<double> results);
    ///
    /// \brief done - signal indicates completion of calculations
    ///
    void done();

public slots:
    ///
    /// \brief estimateGasSpector - starts the algorithm for calculating the gas spectrum
    /// \param U - input voltages
    /// \param M - mass of gases
    ///
    void estimateGasSpector(QVector<int> U, QModelIndexList M);

private:
    ///
    /// \brief getData - procedure for constructing the resulting gas spectrum
    /// \param u1 - input voltage
    /// \param u2 - input voltage
    /// \param uk - input voltage
    /// \param M - objects of the model, to calculate the mass of gases
    /// \return - total gas spectrum
    ///
    QVector<double> getData(const int u1, const int u2, const int uk, QModelIndexList M);
    ///
    /// \brief estemateData - procedure for constructing a gas spectrum for each gas from an array of fountain objects
    /// \param u1 - input voltage
    /// \param u2 - input voltage
    /// \param uk - input voltage
    /// \param M - objects of the model, to calculate the mass of gases
    /// \return array of spectra for each gas
    ///
    QVector<QVector<double> > estemateData(const int u1, const int u2, const int uk, QVector<int> M);
    ///
    /// \brief createMassesOfGases - calculation of an array of masses of gases for an array of model objects
    /// \param M - array of model objects
    /// \return
    ///
    QVector<int> createMassesOfGases(QList<QModelIndex> M);
    ///
    /// \brief tof - procedure for calculating the flight time of an ion for a given set of parameters
    /// \param param -
    /// \return - a set of ion flight times, depending on the mass of the gas
    ///
    QVector<double> tof(const Params &param);
    QMap<QModelIndex,QPair<int,double> > gases;
    QStringListModel *model;
    QStringList gasesNames;
};

#endif // DATAGENERATOR_H
