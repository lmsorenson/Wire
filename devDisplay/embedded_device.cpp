#include "embedded_device.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QDebug>


embedded_device::embedded_device(char* path, QWidget *parent) : QWidget(parent)
{


    //open the serial port
    _file_descriptor=openSerialPort(path);

    std::string str = std::string(GetArduinoType().toStdString());
    QString name = str.substr(7).c_str();

    if(_file_descriptor!=-1)
        qDebug() << "device connection initialized: '" << name << "'";

    if(_file_descriptor!=-1)
        qDebug() << "device: '" << name << "' initialized successfully.";

    QGridLayout
            * lay = new QGridLayout(),
            * groupLay = new QGridLayout();

    QGroupBox * GroupBox = new QGroupBox(name);

    this->setLayout(lay);
    lay->addWidget(GroupBox);
    GroupBox->setLayout(groupLay);

    slider1 = new QSlider(Qt::Orientation::Horizontal);
        slider1->setMinimumWidth(200);
        slider1->setDisabled(true);

    slider2 = new QSlider(Qt::Orientation::Horizontal);
        slider2->setMinimumWidth(200);
        slider2->setDisabled(true);

    groupLay->addWidget(new QLabel("Steer"),0,0);
    groupLay->addWidget(slider1, 0, 1);

    groupLay->addWidget(new QLabel("Throttle"),1,0);
    groupLay->addWidget(slider2, 1, 1);
}

embedded_device::~embedded_device()
{

    closeSerialPort(_file_descriptor);
}


QString embedded_device::GetArduinoType()
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

int embedded_device::GetArduinoStatus(embedded_device * device)
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

void embedded_device::setSteer(int value)
{
    slider1->setSliderPosition(value);
}

void embedded_device::setThrottle(int value)
{
    slider2->setSliderPosition(value);
}
