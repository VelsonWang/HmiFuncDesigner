#include "SysVariable.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SysVariable w;
    w.show();

    return a.exec();
}
