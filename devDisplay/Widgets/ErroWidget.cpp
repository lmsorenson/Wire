#include "ErrorWidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QDebug>


ErrorWidget::ErrorWidget(QString error, QWidget *parent) : QWidget(parent)
{
    QGridLayout * lay = new QGridLayout();
    this->setLayout(lay);
    lay->addWidget(new QLabel(error), 0, 0);
}

ErrorWidget::~ErrorWidget()
{
}