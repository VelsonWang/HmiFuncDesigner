#include <QApplication>
#include "EDncryptToolDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EDncryptToolDialog dlg;
    int ret = dlg.exec();

    return ret;
}
