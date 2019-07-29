#include "SysTag.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SysTag win;
    win.show();

    return a.exec();
}
