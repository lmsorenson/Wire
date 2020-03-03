#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    int GetArduino(Ui::MainWindow * ui);
    void HandleTime();

private:
    Ui::MainWindow *ui;

    int _file_descriptor;
};
#endif // MAINWINDOW_H
