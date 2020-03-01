#include "mainwindow.h"

extern "C"{
    #include <wire.h>
}


#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{

    int fileDescriptor;

    //open the serial port
    fileDescriptor=openSerialPort("/dev/cu.usbmodem14601");

    if(fileDescriptor != -1)
    {

        char* data = (char*)malloc(1024*sizeof(char));

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

        //close the serial port
        closeSerialPort(fileDescriptor);
    }


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
