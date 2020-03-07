#include "embedded_device.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>

embedded_device::embedded_device(QString name, QWidget *parent) : QWidget(parent)
{
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


void embedded_device::setSteer(int value)
{
    slider1->setSliderPosition(value);
}

void embedded_device::setThrottle(int value)
{
    slider2->setSliderPosition(value);
}
