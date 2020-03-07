#ifndef EMBEDDED_DEVICE_H
#define EMBEDDED_DEVICE_H

#include <QSlider>
#include <QWidget>
#include <QString>

class embedded_device : public QWidget
{
    Q_OBJECT
public:
    explicit embedded_device(QString name, QWidget *parent = nullptr);


    QSlider * slider1;
    QSlider * slider2;

    void setSteer(int value);
    void setThrottle(int value);

signals:

};

#endif // EMBEDDED_DEVICE_H
