#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "embedded_device.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString GetArduinoType();
    int GetArduinoStatus(embedded_device * device);
    void HandleTime();

private:
    Ui::MainWindow *ui;

    embedded_device *device;

    int _file_descriptor;
};
#endif // MAINWINDOW_H
