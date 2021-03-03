#pragma once

#include "Widgets/EmbeddedDevice.h"
#include "Widgets/ErrorWidget.h"

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

    std::vector<EmbeddedDevice*> device_;

};
