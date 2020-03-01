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

    this->GetArduino(ui);

}

int MainWindow::GetArduino(Ui::MainWindow * ui)
{
    int fileDescriptor;

    //open the serial port
    fileDescriptor=openSerialPort("/dev/cu.usbmodem14601");

    if(fileDescriptor != -1)
    {

        char* data = static_cast<char*>(malloc(1024*sizeof(char)));

        if(readSerialPort(fileDescriptor, data)!=0)
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

        //close the serial port
        closeSerialPort(fileDescriptor);
    }

    return 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

