#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <QSlider>

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

    this->GetArduino(ui);

    free(dev_name);
}

int MainWindow::GetArduino(Ui::MainWindow * ui)
{
    if(_file_descriptor != -1)
    {

        char* data = static_cast<char*>(malloc(1024*sizeof(char)));

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
        steer_slider_value=((GetDescriptorValue(data, 'S')+1000)/20),
        throttle_slider_value=(GetDescriptorValue(data, 'T')/10);

        ui->steer_slider->setSliderPosition(steer_slider_value);
        ui->throttle_slider->setSliderPosition(throttle_slider_value);

        free(data);
    }

    return 0;
}

MainWindow::~MainWindow()
{
    closeSerialPort(_file_descriptor);
    delete ui;
}

