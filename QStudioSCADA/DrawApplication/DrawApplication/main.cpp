#include <QApplication>
#include <QMessageBox>
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
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString szProjPath = "";
    QString szGraphPageName = "";

    if(argc == 3) {
        szProjPath = argv[1];
        szGraphPageName = argv[2];
    } else {
        QMessageBox::information(Q_NULLPTR, "提示", "画面编辑程序只能由工程管理器调用！");
        return 0;
    }

    // 加载元素插件
    PluginManager::getInstance()->loadPlugin();

    MainWindow mainWin(szProjPath, szGraphPageName);
    mainWin.openGraphPage(szProjPath, szGraphPageName);
    mainWin.show();

    int ret = a.exec();

    // 释放插件
    PluginManager::getInstance()->releasePlugin();

    return ret;
}
