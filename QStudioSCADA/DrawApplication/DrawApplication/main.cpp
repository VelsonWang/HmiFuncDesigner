#include <QApplication>
#include "MainWindow.h"
#include "configutils.h"
#include "DrawListUtils.h"
#include "PluginManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString strProjPath = "";
    QString strgraphPageName = "";
    if(argc == 3) {
        strProjPath = argv[1];
        strgraphPageName = argv[2];
    } else {
        QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
        strProjPath = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "/");

        int last = 0;
        QStringList DrawList;
        DrawListUtils::LoadDrawList(strProjPath, DrawList);
        last = DrawListUtils::GetMaxDrawPageNum("draw", DrawList);
        strgraphPageName = QString("draw%1").arg(last);
    }

    // 加载元素插件
    PluginManager::getInstance()->loadPlugin();

    MainWindow mainWin(strProjPath, strgraphPageName);
    mainWin.show();

    int ret = a.exec();

    // 释放插件
    PluginManager::getInstance()->releasePlugin();

    return ret;
}
