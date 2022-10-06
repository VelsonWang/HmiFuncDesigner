#include "mainwindow.h"
#include "keyboard.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    KeyBoard::instance()->installInputMethod();
}

MainWindow::~MainWindow()
{
    KeyBoard::instance()->unInstallInputMethod();
}

void MainWindow::showEvent(QShowEvent *)
{

}

void MainWindow::hideEvent(QHideEvent *)
{

}
