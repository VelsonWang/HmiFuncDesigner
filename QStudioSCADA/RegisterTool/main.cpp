#include <QApplication>

#include "cregisterdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int ret = CRegisterDialog::GeneratorDialog();

    return ret;
}
