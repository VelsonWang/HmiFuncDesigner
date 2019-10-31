#include "MainWindow.h"
#include "ConfigUtils.h"

#include <QApplication>
#include <QString>

#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString szProjectPath = "";
    if(argc == 2) {
        szProjectPath = argv[1];
    }

    if (szProjectPath == "") {
        QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
        szProjectPath = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
    }

    if (szProjectPath == "")
        qApp->exit();

    MainWindow w(szProjectPath);
    w.show();
    w.ShowFirstPage();

    return a.exec();
}
