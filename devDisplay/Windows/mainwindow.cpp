#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <QSlider>
#include <QTimer>
#include <QTime>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // run a callback every 500 milliseconds.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::HandleTime);
    timer->start(500);

    char *dev_name[5];

    //check for devices.
    qDebug() << "searching device directory.";
    char directory[] = "/dev/";
    int result;

    if ((result = listDevices(directory, dev_name)) != 0)
    {
        qDebug() << "Could not find the /dev/ directory.";
        ui->gridLayout->addWidget(new ErrorWidget("Could not find the /dev/ directory."));
    }

    //populate list of embedded devices.
    qDebug() << "creating embedded devices.";
    for (int i=0; i < 5; i++)
    {
        qDebug() << "Polling device: " << dev_name[i];
        if(strcmp(dev_name[i], "") != 0)
        {
            device_.push_back(new EmbeddedDevice(dev_name[i]));
            ui->gridLayout->addWidget(device_.back());
        }
    }

    if (device_.size() == 0)
        ui->gridLayout->addWidget(new ErrorWidget("No devices found."));

    //free allocated memory.
    qDebug() << "freeing device names.";
    for (int i=0; i<5; i++)
        free(dev_name[i]);
}



void MainWindow::HandleTime()
{
    for(int i=0; i < device_.size(); i++)
    {
        try
        {
            device_[i]->GetArduinoStatus(device_[i]);
        }
        catch (std::exception ex)
        {
            qDebug() << "An exception occurred: " << ex.what();
            ui->gridLayout->removeWidget(device_[i]);
            ui->gridLayout->addWidget(new ErrorWidget(ex.what()));
        }
        catch (...)
        {
            qDebug() << "An unknown exception occurred.";

            ui->gridLayout->removeWidget(device_[i]);
            ui->gridLayout->addWidget(new ErrorWidget("An unknown error occurred."));
        }
    }
}


MainWindow::~MainWindow()
{
    for(int i=0; i < device_.size(); i++)
        device_.at(i)->deleteLater();

    device_.clear();

    delete ui;
}

