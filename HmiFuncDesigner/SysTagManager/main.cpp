#include "SysTag.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SysTag win;
    win.show();

    return app.exec();
}
