#include <QApplication>
#include <QDebug>
#include "applauncher.h"
#include "ConfigUtils.h"
#include "Helper.h"

///
/// \brief getApplicationFiles
/// \details 获取配置的执行文件名列表
/// \return 执行文件名列表
///
QStringList getApplicationFiles()
{
    QStringList szListFileName;
    QString iniAppLauncher = Helper::AppDir() + "/applauncher.ini";
    QFile fileCfg(iniAppLauncher);
    if(fileCfg.exists()) {
        QString szDir = ConfigUtils::getCfgStr(iniAppLauncher, "AppDir", "dir", Helper::AppDir() + "/");
        QStringList szlist;
        ConfigUtils::getCfgList(iniAppLauncher, "AppName", "app", szlist);
        foreach(QString szFileName, szlist) {
            szListFileName << (szDir + szFileName);
        }
    }
    return szListFileName;
}

//
// TODO
// 后期加入进程监控...
//

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AppLauncher appLauncher;

    QStringList szListFileName = getApplicationFiles();
    foreach(QString szFileName, szListFileName) {
        appLauncher.execute(szFileName, QStringList());
    }

    return app.exec();
}
