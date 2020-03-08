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

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::HandleTime);
    timer->start(500);



    char *dev_name[5];


    //check for devices.
    qDebug() << "checking directory.";
    char directory[] = "/dev/";
    listDevices(directory, dev_name);


    //populate list of embedded devices.
    qDebug() << "creating embedded devices.";
    for (int i=0; i<5; i++)
    {
        qDebug() << "Polling device: " << dev_name[i];
        if(strcmp(dev_name[i], "")!=0)
        {
            _device.push_back(new embedded_device(dev_name[i]));
            ui->gridLayout->addWidget(_device.back());
        }
    }



    //free allocated memory.
    qDebug() << "freeing device names.";
    for (int i=0; i<5; i++)
    {
        free(dev_name[i]);
    }
}



void MainWindow::HandleTime()
{
    for(int i=0; i<_device.size(); i++)
    {
        _device[i]->GetArduinoStatus(_device[i]);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

