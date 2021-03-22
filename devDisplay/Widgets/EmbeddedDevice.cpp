#include "EmbeddedDevice.h"

#include <Widgets/ErrorWidget.h>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <exception>


EmbeddedDevice::EmbeddedDevice(char* path, QWidget *parent) : QWidget(parent)
{
    char * err = static_cast<char*>(malloc(1024*sizeof(char)));

    QGridLayout
            * lay = new QGridLayout(),
            * groupLay = new QGridLayout();

    this->setLayout(lay);

    //open the serial port
    if ((file_descriptor_ = openSerialPort(path, err)) == -1)
    {
        qDebug() << "Could not open the serial port: " << err;
        lay->addWidget(new ErrorWidget(err));
        free(err);
        return;
    }

    std::string str = std::string(GetArduinoType().toStdString());
    QString name = str.substr(7).c_str();

    if(file_descriptor_ != -1)
        qDebug() << "device connection initialized: '" << name << "'";

    if(file_descriptor_ != -1)
        qDebug() << "device: '" << name << "' initialized successfully.";

    QGroupBox * GroupBox = new QGroupBox(name);
    lay->addWidget(GroupBox);
    GroupBox->setLayout(groupLay);

    slider1_ = new QSlider(Qt::Orientation::Horizontal);
    slider1_->setMinimumWidth(200);
    slider1_->setDisabled(true);

    slider2_ = new QSlider(Qt::Orientation::Horizontal);
    slider2_->setMinimumWidth(200);
    slider2_->setDisabled(true);

    groupLay->addWidget(new QLabel("Steer"),0,0);
    groupLay->addWidget(slider1_, 0, 1);

    groupLay->addWidget(new QLabel("Throttle"),1,0);
    groupLay->addWidget(slider2_, 1, 1);

    free(err);
}

EmbeddedDevice::~EmbeddedDevice()
{
    if (slider1_) slider1_->deleteLater();
    if (slider2_) slider2_->deleteLater();
    closeSerialPort(file_descriptor_);
}


QString EmbeddedDevice::GetArduinoType()
{
    if(file_descriptor_ != -1)
    {
        char* data = static_cast<char*>(malloc(1024*sizeof(char)));
        char * message = static_cast<char*>(malloc(6*sizeof(char)));
        char * err = static_cast<char*>(malloc(1024*sizeof(char)));

        message = "~Type";

        if (writeSerialPort(file_descriptor_, message, err) != 0)
        {
            qDebug() << err;
            throw std::runtime_error(err);
        }

        if(readSerialPort(file_descriptor_, data, err) != 0)
        {
            qDebug() << err;
            throw std::runtime_error(err);
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
        free(err);
    }

    return "none found";
}

int EmbeddedDevice::GetArduinoStatus(EmbeddedDevice * device)
{
    if(file_descriptor_ != -1)
    {
        char* data = static_cast<char*>(malloc(1024*sizeof(char)));
        char * err = static_cast<char*>(malloc(1024*sizeof(char)));

        if(writeSerialPort(file_descriptor_, "~Status", err) != 0)
        {
            qDebug() << err;
            throw std::runtime_error(err);
        }

        if(readSerialPort(file_descriptor_, data, err) != 0)
        {
            qDebug() << err;
            throw std::runtime_error(err);
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
        steer_slider_value = ( (GetDescriptorValue(data, 'S') + 1000) / 20 ),
        throttle_slider_value = ( GetDescriptorValue(data, 'T') / 10);

        device->SetSteer(steer_slider_value);
        device->SetThrottle(throttle_slider_value);

        free(err);
        free(data);
    }

    return 0;
}

void EmbeddedDevice::SetSteer(int value)
{
    slider1_->setSliderPosition(value);
}

void EmbeddedDevice::SetThrottle(int value)
{
    slider2_->setSliderPosition(value);
}
