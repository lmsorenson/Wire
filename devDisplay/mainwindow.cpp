#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <QSlider>
#include <QTimer>
#include <QTime>

extern "C"
{
    #include <wire.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char* dev_name = static_cast<char*>(malloc(1024 * sizeof(char)));
    char directory[] = "/dev/";
    listDevices(directory, dev_name);

    //open the serial port
    _file_descriptor=openSerialPort(dev_name);


    std::string str = std::string(GetArduinoType().toStdString());
    QString name = str.substr(7).c_str();

    ui->gridLayout->addWidget((device = new embedded_device(name)));
    ui->gridLayout->addWidget(new embedded_device(name));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::HandleTime);
    timer->start(500);

    free(dev_name);
}

QString MainWindow::GetArduinoType()
{
    if(_file_descriptor != -1)
    {

        char* data = static_cast<char*>(malloc(1024*sizeof(char)));
        char * message = static_cast<char*>(malloc(6*sizeof(char)));

        message = "~Type";

        writeSerialPort(_file_descriptor, message);

        if(readSerialPort(_file_descriptor, data)!=0)
        {
        }
        else
        {
            //print data from the serial port.
            qDebug() << "Serial Port Data - Type\n";
            qDebug() << data;

            return data;
        }


        free(message);
        free(data);
    }

    return "none found";
}

int MainWindow::GetArduinoStatus(embedded_device * device)
{
    if(_file_descriptor != -1)
    {

        char* data = static_cast<char*>(malloc(1024*sizeof(char)));

        writeSerialPort(_file_descriptor, "~Status");

        if(readSerialPort(_file_descriptor, data)!=0)
        {
        }
        else
        {
            //print data from the serial port.
            qDebug() << "Serial Port Data\n";
            qDebug() << "B: " << GetDescriptorValue(data, 'B') << "\n";
            qDebug() << "T: " << GetDescriptorValue(data, 'T') << "\n";
            qDebug() << "S: " << GetDescriptorValue(data, 'S') << "\n";
        }

        int
        steer_slider_value=( (GetDescriptorValue(data, 'S')+1000) / 20 ),
        throttle_slider_value=( GetDescriptorValue(data, 'T') / 10 );

        device->setSteer(steer_slider_value);
        device->setThrottle(throttle_slider_value);

        free(data);
    }

    return 0;
}

void MainWindow::HandleTime()
{
    this->GetArduinoStatus(device);
}


MainWindow::~MainWindow()
{
    closeSerialPort(_file_descriptor);
    delete ui;
}

