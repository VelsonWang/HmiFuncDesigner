#include "MainWindow.h"
#include "configutils.h"
#include "DrawListUtils.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString strProjPath = "";
    QString strDrawPageName = "";
    if(argc == 3)
    {
        strProjPath = argv[1];
        strDrawPageName = argv[2];
    }
    else
    {
        QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
        strProjPath = ConfigUtils::GetCfgStr(strFile, "PathInfo", "Path", "/");

        int last = 0;
        QStringList DrawList;
        DrawListUtils::LoadDrawList(strProjPath, DrawList);
        last = DrawListUtils::GetMaxDrawPageNum("draw", DrawList);
        strDrawPageName = QString("draw%1").arg(last);
    }

    MainWindow mainWin(strProjPath, strDrawPageName);
    mainWin.show();

    return a.exec();
}
