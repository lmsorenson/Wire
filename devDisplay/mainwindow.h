#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "embedded_device.h"

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void HandleTime();

private:
    Ui::MainWindow *ui;

    std::vector<embedded_device*> _device;

};
#endif // MAINWINDOW_H
