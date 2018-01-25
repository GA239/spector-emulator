#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->createAndConfigureElemtsOfWindow();
    this->setwidgetAssembly();
    this->setWindowTitle("spector-emulator");
    //this->setStyleSheet( " background-color: white; " );

    QAction *exportConfig = this->ui->mainToolBar->addAction(QIcon("../../../../spector-emulator/resourses/export.png"), "Export Config");
    connect(exportConfig, SIGNAL(triggered()), this, SLOT(exportConfigSlot()));
    QAction *importConfig = this->ui->mainToolBar->addAction(QIcon("../../../../spector-emulator/resourses/import.png"), "Import Config");
    connect(importConfig, SIGNAL(triggered()), this, SLOT(importConfigSlot()));
    QAction *saveData = this->ui->mainToolBar->addAction(QIcon("../../../../spector-emulator/resourses/save.png"), "Save Data");
    connect(saveData, SIGNAL(triggered()), this, SLOT(saveDataSlot()));

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

    // ����� ���������� ������ ���������� - ����� ������ �����������
    QObject::connect(generator, SIGNAL(done()), worker, SLOT(quit()));
    QObject::connect(generator, SIGNAL(done()), this, SLOT(updateProgressbar()));

    // ��������� ������ �������� � �������� ������
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QModelIndexList >("QModelIndexList");
    QObject::connect(this, SIGNAL(estimateGasSpector(QVector<int>,QModelIndexList)),worker, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));
    QObject::connect(worker, SIGNAL(estimateGasSpectorStarted(QVector<int>,QModelIndexList)), generator, SLOT(estimateGasSpector(QVector<int>,QModelIndexList)));
    //����������
    QObject::connect(worker, SIGNAL(stopEstimationSignal()), generator, SLOT(stopEstimation()));
    //QObject::connect(worker, SIGNAL(stopEstimationSignal()), generator, SLOT(stopEstimation()));

    // transfer the calculations to the thread
    generator->moveToThread(worker);
}

int MainWindow::exportData(QString & resultStr)
{
    resultStr = QFileDialog::getSaveFileName(this, tr("Save gas spector"),"gsData",tr("Text (*.txt)"));
    if(resultStr.isEmpty())
        return -1;
    QFile fileOut(resultStr);
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&fileOut);
        QSharedPointer<QCPGraphDataContainer> data = this->plotter->getData();
        for(auto iter = data.data()->begin(); iter != data.data()->end(); ++iter)
            writeStream << QString::number(iter->value) << "\n";
        fileOut.close();
    }
    else
    {
        //error message
    }
    return _RC_SUCCES_;
}

int MainWindow::exportConfigSlot()
{
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
        return _RC_SUCCES_;

    settings.setValue( "gasesNumber" , gases.length());
    for(int i = 0; i < gases.length(); ++i)
        settings.setValue( "G" + QString::number(i) , gases[i]);
    settings.endGroup();

    QString dataSrc;
    if(exportData(dataSrc))
    {
        settings.beginGroup( "Data" );
        settings.setValue( "Path" , dataSrc);
        settings.endGroup();
    }
    return _RC_SUCCES_;
}

int MainWindow::saveDataSlot()
{
    QString str;
    return exportData(str);
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
    for(int i = 0; i < controllerValues.length(); ++i)
    {
        controllerValues[i] = settings.value( "U" + QString::number(i), -1 ).toInt();
        if(controllerValues[i] < 250)
            return -1;
    }
    this->controll->setUvalues(controllerValues);
    settings.endGroup();

    settings.beginGroup( "Gases" );
    int gsize = settings.value( "gasesNumber", -1 ).toInt();
    if(gsize == 0)
        return _RC_SUCCES_;

    if(gsize < 0)
        return -1;

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
    if (!file.open(QIODevice::ReadOnly))
       return -1;

    QTextStream fileStream(&file);
    QVector<double> values;
    while(!fileStream.atEnd()) {
        values.push_back(fileStream.readLine().toDouble());
    }
    file.close();
    this->GetResults(values);
    return _RC_SUCCES_;
}

void MainWindow::createAndConfigureElemtsOfWindow()
{
    ///* data generator *///
    //class for parallel (heavy) estimations
    DataGenerator::bobo = false;
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
    //QObject::connect(this->controll, SIGNAL(Changed()), this, SLOT(ControllChanged()));

    ///* plot widget *///
    this->plotter =  new PlotSEWidget();

    ///* estimation status and controll widgets *///
    this->progressBar = new QProgressBar();
    this->progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    this->progressBar->setRange(0, 100);
    this->progressBar->hide();

    this->startEstimateBottom = new QPushButton();
    QFont font( "Times" );
    font.setPointSize( 22 );
    font.setWeight( QFont::Bold );
    this->startEstimateBottom->setFont(font );
    this->startEstimateBottom->setText("Estimate");
    this->startEstimateBottom->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
}

void MainWindow::setwidgetAssembly()
{
    QGridLayout *layout = new QGridLayout();
    layout->addWidget(this->controll,0,0);
    layout->addWidget(this->plotter,0,1,2,3);
    layout->addWidget(this->startEstimateBottom,2,0);
    layout->addWidget(this->progressBar,2,1,1,3);

    this->ui->centralWidget->setLayout(layout);
}


void MainWindow::progressSet(int value)
{
    this->progressBar->setValue(value);
}

void MainWindow::GetResults(QVector<double> results)
{
    this->plotter->setPlotData(results);
    this->update();
}

void MainWindow::bobotreu()
{
    DataGenerator::bobo = true;
}

void MainWindow::updateProgressbar()
{
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Estimate");
    QObject::disconnect(this->startEstimateBottom, SIGNAL(clicked()), worker, SLOT(stopEstimation()));
    this->progressBar->hide();

}


void MainWindow::EstimateBottomPressed()
{
    // ������������ ���������� ������ �� ������� ������ Stop
    QObject::disconnect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(EstimateBottomPressed()));
    this->startEstimateBottom->setText("Stop");
    //QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), worker, SLOT(stopEstimation()));
    QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), this, SLOT(bobotreu()));

    //QObject::connect(this->startEstimateBottom, SIGNAL(clicked()), generator, SLOT(stopEstimation()));
    this->progressBar->show();
    emit estimateGasSpector(this->controll->getUvalues(),this->controll->getTagsFromSearchWidget());
}
