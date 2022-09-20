#include "mainwindow.h"
#include "keyboardnumber.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    KeyBoardNumber::instance()->installInputMethod();

}

MainWindow::~MainWindow()
{
    KeyBoardNumber::instance()->unInstallInputMethod();

}

void MainWindow::showEvent(QShowEvent *)
{

}

void MainWindow::hideEvent(QHideEvent *)
{

}
