#include "MainWindow.h"
#include "configutils.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication a(argc, argv);

    QString sProjectPath = "";
    QString sJsFileName = "";

    if(argc == 3)
    {
        sProjectPath = argv[1];
        sJsFileName = argv[2];
    }

    if(sProjectPath == "")
    {
        QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
        sProjectPath = ConfigUtils::GetCfgStr(strFile, "PathInfo", "Path", "C:/");
    }

    if(sProjectPath == "")
        qApp->exit();

    //QMessageBox::information(0, "参数：", "argv[1]  " + sProjectPath + "\nargv[2]  " + sJsFileName);

    MainWindow w(sProjectPath, sJsFileName);
    w.show();

    return a.exec();
}
