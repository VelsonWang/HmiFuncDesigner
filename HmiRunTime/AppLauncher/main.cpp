#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QFile>
#include "applauncher.h"
#include "ConfigUtils.h"
#include "Helper.h"
#include "MessageTransfer.h"

QString g_szAppFileTransferTool = ""; // 工程文件传输工具文件
QString g_szAppHmiRunTimeData = ""; // 数据采集程序文件
QString g_szAppHmiRunTimeView = ""; // 画面视图程序文件

///
/// \brief getApplicationFiles
/// \details 获取配置的执行文件名
///
void getApplicationFiles()
{
    QString iniAppLauncher = Helper::AppDir() + "/applauncher.ini";
    QFile fileCfg(iniAppLauncher);
    if(fileCfg.exists()) {
        QString szDir = ConfigUtils::getCfgStr(iniAppLauncher, "AppDir", "dir", Helper::AppDir() + "/");
        g_szAppFileTransferTool = szDir + ConfigUtils::getCfgStr(iniAppLauncher, "AppName", "FileTransferTool", "FileTransferTool");
        g_szAppHmiRunTimeData = szDir + ConfigUtils::getCfgStr(iniAppLauncher, "AppName", "HmiRunTimeData", "HmiRunTimeData");
        g_szAppHmiRunTimeView = szDir + ConfigUtils::getCfgStr(iniAppLauncher, "AppName", "HmiRunTimeView", "HmiRunTimeView");
#ifdef Q_OS_WIN
		g_szAppFileTransferTool += ".exe";
		g_szAppHmiRunTimeData += ".exe";
		g_szAppHmiRunTimeView += ".exe";
#endif
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AppLauncher appLauncher;

    // 获取配置的执行文件名
    getApplicationFiles();

    // Step-1 启动进程：工程文件传输工具
    appLauncher.execute(g_szAppFileTransferTool, QStringList());

    // Step-2 启动进程：数据采集程
    appLauncher.execute(g_szAppHmiRunTimeData, QStringList());

    // Step-3 启动进程：画面视图
    appLauncher.execute(g_szAppHmiRunTimeView, QStringList());

    // Step-4 开启消息服务
    MessageTransfer server(true);
    QObject::connect(&server, &MessageTransfer::messageReceived, &appLauncher, &AppLauncher::onMessageReceived);

    return app.exec();
}
