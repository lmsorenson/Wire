#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

//#include <wire.h>

extern "C" {
    int openSerialPort(const char *bsdPath);
    int initializeModem(int fileDescriptor);
    void writeSerialPort(int fileDescriptor, char * InString);
    char *readSerialPort(int fileDescriptor);
    int closeSerialPort(int fileDescriptor);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int fileDescriptor;

   //open the serial port
   fileDescriptor=openSerialPort("/dev/cu.usbmodem14601");

   //print data from the serial port.
   QString data = QString((const char*)readSerialPort(fileDescriptor));

   //print data from the serial port.
   qDebug() << "\n\nSerial Port Data\n";
   qDebug() << data;
   qDebug() << "\n";

   //close the serial port
   closeSerialPort(fileDescriptor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

