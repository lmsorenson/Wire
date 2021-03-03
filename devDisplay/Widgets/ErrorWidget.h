#pragma once

#include <QWidget>
#include <QString>

class ErrorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorWidget(QString error, QWidget *parent = nullptr);
    ~ErrorWidget();
};
