#ifndef EMBEDDED_DEVICE_H
#define EMBEDDED_DEVICE_H

#include <QSlider>
#include <QWidget>
#include <QString>

extern "C"
{
    #include <wire.h>
}

class embedded_device : public QWidget
{
    Q_OBJECT
public:
    explicit embedded_device(char* path, QWidget *parent = nullptr);
    ~embedded_device();

    QString GetArduinoType();
    int GetArduinoStatus(embedded_device * device);

    QSlider * slider1;
    QSlider * slider2;

    void setSteer(int value);
    void setThrottle(int value);

    int _file_descriptor;

signals:

};

#endif // EMBEDDED_DEVICE_H
