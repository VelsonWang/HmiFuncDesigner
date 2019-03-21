#include <QApplication>
#include "MainWindow.h"
#include "configutils.h"
#include "DrawListUtils.h"
#include "PluginManager.h"

/**
 * @brief qMain
 * @param argc
 * @param argv argv[0]--application name,
 *             argv[1]--project path,
 *             argv[2]--graph page name,
 *             argv[3]--new, open,
 *             argv[4]--graph page width
 *             argv[5]--graph page height
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString strProjPath = "";
    QString strGraphPageName = "";
    QString operate = "";
    int width = 0;
    int height = 0;

    if(argc == 6) {
        strProjPath = argv[1];
        strGraphPageName = argv[2];
        operate = argv[3];
        width = QString(argv[4]).toInt();
        height = QString(argv[5]).toInt();
    } else {
        QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
        strProjPath = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "/");

        int last = 0;
        QStringList DrawList;
        DrawListUtils::LoadDrawList(strProjPath, DrawList);
        last = DrawListUtils::GetMaxDrawPageNum("draw", DrawList);
        strGraphPageName = QString("draw%1").arg(last);
    }

    // 加载元素插件
    PluginManager::getInstance()->loadPlugin();

    MainWindow mainWin(strProjPath, strGraphPageName);
    if(operate.toLower() == "new") {
        mainWin.addAndSaveGraphPage(strProjPath, strGraphPageName, width, height);
        return 0;
    } else if(operate.toLower() == "open") {

        mainWin.show();
    }

    int ret = a.exec();

    // 释放插件
    PluginManager::getInstance()->releasePlugin();

    return ret;
}
