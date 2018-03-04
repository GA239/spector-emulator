#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QProgressBar>
#include "chartlayoutdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->createAndConfigureElemtsOfWindow();
    this->setwidgetAssembly();
    this->setWindowTitle("spectrum-emulator");
    this->setMinimumSize(0.8 * (qApp->desktop()->width()), (0.8 * (qApp->desktop()->height())));

    QAction *exportConfig = this->ui->mainToolBar->addAction(QIcon(":/images/export.png"), "Export Config");
    connect(exportConfig, SIGNAL(triggered()), this, SLOT(exportConfigSlot()));
    QAction *importConfig = this->ui->mainToolBar->addAction(QIcon(":/images/import.png"), "Import Config");
    connect(importConfig, SIGNAL(triggered()), this, SLOT(importConfigSlot()));
    QAction *saveData = this->ui->mainToolBar->addAction(QIcon(":/images/save.png"), "Save Data");
    connect(saveData, SIGNAL(triggered()), this, SLOT(saveDataSlot()));
    QAction *chartChange = this->ui->mainToolBar->addAction(QIcon(":/images/chart.png"), "Change chart");
    connect(chartChange, SIGNAL(triggered()), this, SLOT(changeChartLayout()));

    QFile styleSheet(":/styles/stylesheet.qss");
    if (!styleSheet.open(QIODevice::ReadOnly)) {
        qWarning("Unable to open stylesheet.qss");
    }
    qApp->setStyleSheet(styleSheet.readAll());
}

MainWindow::~MainWindow()
{
    delete this->controll;
    delete this->plotter;
    delete this-> startEstimateBottom;
    delete this-> progressBar;
    generator->deleteLater();
    delete this-> worker;
    delete ui;
}

void MainWindow::createThreadEstimation(void)
{
    // Connect progress notifications to progress bar
    QObject::connect(generator, SIGNAL(progressChanged(int)), this, SLOT(progressSet(int)));
    // Connect the transfer of results
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(generator, SIGNAL(SendResults(QVector<double>)), this, SLOT(GetResults(QVector<double>)));

    // When the work is done, the thread should end
    QObject::connect(generator, SIGNAL(done()), worker, SLOT(quit()));
    QObject::connect(generator, SIGNAL(done()), this, SLOT(updateProgressbar()));

    // connect the beginning of calculations with the start of the flow
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QModelIndexList >("QModelIndexList");
    QObject::connect(this, SIGNAL(estimateGasSpector(QVector<int>,QModelIndexList)),worker, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));
    QObject::connect(worker, SIGNAL(estimateGasSpectorStarted(QVector<int>,QModelIndexList)), generator, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));

    // transfer the calculations to the thread
    generator->moveToThread(worker);
}

int MainWindow::exportData(QString & resultStr)
{
    resultStr = QFileDialog::getSaveFileName(this, tr("Save gas spector"),"gsData",tr("Text (*.txt)"));
    if(resultStr.isEmpty())
        return -1;

    QFile fileOut(resultStr);
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream writeStream(&fileOut);
        QSharedPointer<QCPGraphDataContainer> data = this->plotter->getData(PlotSEWidget::PLOT_NAMES::SPECTURM);
        if(data == nullptr)
            return _RC_SUCCESS_;
        for(auto iter = data.data()->begin(); iter != data.data()->end(); ++iter)
            writeStream << QString::number(iter->value) << "\n";
        fileOut.close();
    }
    else{
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Can not open the file!");
        messageBox.setFixedSize(500,200);
        return -1;
    }
    return _RC_SUCCESS_;
}

int MainWindow::exportConfigSlot()
{
    if(this->controllChangedFlag)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","The controller values ​​and plot data are dismatch! \nReestimate data!");
        messageBox.setFixedSize(500,200);
        return _ERROR_WRONG_WORKFLOW_;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save gas spector"),"gas-spector.gscfg",tr("Text (*.gscfg)"));
    if(fileName.isEmpty())
        return -1;

    QSettings settings( fileName, QSettings::IniFormat );

    settings.beginGroup( "Controller" );

        QVector<int> controllerValues = this->controll->getUvalues();
        for(int i = 0; i < controllerValues.length(); ++i)
            settings.setValue( "U" + QString::number(i) , controllerValues[i]);

    settings.endGroup();

    settings.beginGroup( "Gases" );

        QStringList gases = this->controll->getTagsFromSearchWidgetAsStringList();
        if(gases.isEmpty())
            return _RC_SUCCESS_;

        settings.setValue( "gasesNumber" , gases.length());
        for(int i = 0; i < gases.length(); ++i)
            settings.setValue( "G" + QString::number(i) , gases[i]);

    settings.endGroup();

    QString dataSrc;
    if(exportData(dataSrc)){
        settings.beginGroup( "Data" );
            settings.setValue( "Path" , dataSrc);
        settings.endGroup();
    }
    return _RC_SUCCESS_;
}

int MainWindow::saveDataSlot()
{
    QString str;
    return exportData(str);
}

int MainWindow::estimatePeaksSlot()
{
    // get spectrum for estimation
    QSharedPointer<QCPGraphDataContainer> data = this->plotter->getData(PlotSEWidget::PLOT_NAMES::SPECTURM);
    if(data == nullptr){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Data Is Not Avaliable!");
        messageBox.setFixedSize(500,200);
        return _ERROR_WRONG_WORKFLOW_;
    }

    // prepare input data
    int counter = 0;
    QVector<double> input(data.data()->size(),0), output;
    for(auto iter = data.data()->begin(); iter != data.data()->end(); ++iter){
        input[counter] = iter->value;
        counter++;
    }

    // estimation
    PeakDetector pd;
    int rc = pd.estimate(input);

    if(rc != _RC_SUCCESS_)
        return rc;

    /* get results */
    rc =pd.getEstimateByName(PeakDetector::estimatesTracksNames[PeakDetector::ESTIMATES_NAMES::SIGNALS], output);
    if(rc != _RC_SUCCESS_)
        return rc;
    this->plotter->setPlotData(output, PlotSEWidget::PLOT_NAMES::SIGNALS);

    rc =pd.getEstimateByName(PeakDetector::estimatesTracksNames[PeakDetector::ESTIMATES_NAMES::PEAK], output);
    if(rc != _RC_SUCCESS_)
        return rc;
    this->plotter->setPlotData(output, PlotSEWidget::PLOT_NAMES::PEAK);

    // update plots
    this->update();
    return _RC_SUCCESS_;
}

void MainWindow::controllChanged()
{
    this->controllChangedFlag = true;
}

void MainWindow::changeChartLayout()
{

    ChartDialog dlg;
    connect(&dlg, SIGNAL(buttomNumber(int)) , this->plotter, SLOT(changeLayoutSlot(int)) );
    dlg.exec();
}

int MainWindow::importConfigSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Select file"),
                                QDir::currentPath(),
                                "Text (*.gscfg)");
    if(fileName.isEmpty())
        return -1;

    QSettings settings(fileName, QSettings::IniFormat );
    settings.beginGroup( "Controller" );

    QVector<int> controllerValues = this->controll->getUvalues();
    for(int i = 0; i < controllerValues.length(); ++i){
        controllerValues[i] = settings.value( "U" + QString::number(i), -1 ).toInt();
        if(controllerValues[i] < 0){
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Incorrect input!");
            messageBox.setFixedSize(500,200);
            return -1;
        }
    }
    this->controll->setUvalues(controllerValues);
    settings.endGroup();

    settings.beginGroup( "Gases" );
    int gsize = settings.value( "gasesNumber", -1 ).toInt();
    if(gsize == 0)
        return _RC_SUCCESS_;

    if(gsize < 0){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Incorrect input!");
        messageBox.setFixedSize(500,200);
        return -1;
    }
    QStringList gases;
    for(int i = 0; i < gsize; ++i)
        gases.append(settings.value( "G" + QString::number(i), "" ).toString());

    this->controll->addTags(gases);
    settings.endGroup();

    settings.beginGroup( "Data" );
    QString dataPath = settings.value( "Path", "" ).toString();
    settings.endGroup();

    if(dataPath.isEmpty())
        return -1;

    QFile file(dataPath);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Can not open the file!");
        messageBox.setFixedSize(500,200);
        return -1;
    }

    QTextStream fileStream(&file);
    QVector<double> values;
    while(!fileStream.atEnd()){
        values.push_back(fileStream.readLine().toDouble());
    }
    file.close();
    this->GetResults(values);
    return _RC_SUCCESS_;
}

void MainWindow::createAndConfigureElemtsOfWindow()
{
    ///* data generator *///
    //class for parallel (heavy) estimations
    DataGenerator::stopEstimate = false;
    this->generator =  new DataGenerator() ;

    //The flow in which the estimation will be performed
    this->worker = new CountThread();

    //connect dataestimation with additional thread
    this->createThreadEstimation();

    ///* controll widget *///
    this->controll =  new ControllSEWidget();
    QStringListModel *model = new QStringListModel(this);
    model->setStringList(this->generator->getModelElemnts());
    this->controll->setModelToSearchWidget(model);
    this->controllChangedFlag = false;
    QObject::connect(this->controll, SIGNAL(Changed()), this, SLOT(controllChanged()));

    ///* plot widget *///
    this->plotter =  new PlotSEWidget();

    ///* estimation status and controll widgets *///
    this->progressBar = new QProgressBar();
    this->progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->progressBar->setRange(0, 100);
    this->progressBar->hide();

    this->startEstimateBottom = new QPushButton();
    this->startEstimateBottom->setText("Spectrum");
    this->startEstimateBottom->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));

    this->pikiEstimateBottom = new QPushButton();
    this->pikiEstimateBottom->setText("Peaks");
    this->pikiEstimateBottom->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    QObject::connect(this->pikiEstimateBottom, SIGNAL(clicked()), this, SLOT(estimatePeaksSlot()));
}

void MainWindow::setwidgetAssembly()
{
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(this->controll,0,0,2,1);
    layout->addWidget(this->plotter,0,1,2,3);
    layout->addWidget(this->startEstimateBottom,2,0);
    layout->addWidget(this->pikiEstimateBottom,2,1);
    layout->addWidget(this->progressBar,2,2,1,2);
    this->ui->centralWidget->setLayout(layout);
}


void MainWindow::progressSet(int value)
{
    this->progressBar->setValue(value);
}

void MainWindow::GetResults(QVector<double> results)
{
    this->plotter->cleanData();
    this->plotter->setPlotData(results, PlotSEWidget::PLOT_NAMES::SPECTURM);
    this->controllChangedFlag = false;
    this->update();
}

void MainWindow::stopEstimate()
{
    DataGenerator::stopEstimate = true;
}

void MainWindow::updateProgressbar()
{
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Spectrum");
    this->progressBar->hide();
}


void MainWindow::EstimateBottomPressed()
{
    // We provide interruption of the flow by pressing the Stop button
    QObject::disconnect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Stop");
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(stopEstimate()));

    this->progressBar->show();
    emit estimateGasSpector(this->controll->getUvalues(),this->controll->getTagsFromSearchWidget());
}
