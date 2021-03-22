#pragma once

#include <QSlider>
#include <QWidget>
#include <QString>

extern "C"
{
    #include <wire.h>
}

class EmbeddedDevice : public QWidget
{
    Q_OBJECT
public:
    explicit EmbeddedDevice(char* path, QWidget *parent = nullptr);
    ~EmbeddedDevice();

    QString GetArduinoType();
    int GetArduinoStatus(EmbeddedDevice * device);

    QSlider * slider1_;
    QSlider * slider2_;

    void SetSteer(int value);
    void SetThrottle(int value);

    int file_descriptor_;

signals:

};
