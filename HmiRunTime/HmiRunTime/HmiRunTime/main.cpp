#include "HmiRunTime.h"
#include "VersionInfo.h"
#include "ulog.h"
#include <QApplication>
#include <QStorageInfo>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include <QMainWindow>
#include <QMessageBox>
#include <QScopedPointer>
#include "qprojectcore.h"
#include "qrunningmanager.h"
#include "shared/qbaseinit.h"
#include "realtimedb.h"
#include "TimerTask.h"
#include "tcpserver.h"

/**
 * @brief LogInit
 * @details 日志记录初始化
 */
void LogInit()
{
    // 日志是否记录文件名，函数名称，代码行信息
    // true-记录, false-不记录
    LogHelper::m_bShowFileFuncLine = false;
    // 初始化日志配置
    ULog::GetInstance();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 日志记录初始化
    LogInit();
    qDebug() << "start hmiruntime.";

    QString strInput = "";
    QString configPath = "";
    QString szRunProjPath = QCoreApplication::applicationDirPath() + "/RunProject";

    if(argc == 2) {
        szRunProjPath = argv[1];
    }

    QDir dir(szRunProjPath);

    if(!dir.exists()) {
        dir.mkpath(szRunProjPath);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// 工程文件传输服务
    ///
    TcpServer server;
    server.listen(QHostAddress::Any, 6000);

    // find project infomation file
    QString szProjName = getProjectName(szRunProjPath);

    ///////////////////////////////////////////////////////////////////////////
    /// 启动定时任务
    TimerTask tmrTask;

    if(szProjName == "") {
        qCritical() << "project config file not found!";
    } else {
        qRegisterWidgets();
        QString szProjFile = szRunProjPath + "/" + szProjName + ".pdt";
        if(QRunningManager::instance()->load(szProjFile)) {
            HmiRunTime::instance()->setProjectCore(QRunningManager::instance()->projCore());
            HmiRunTime::instance()->Load();
            HmiRunTime::instance()->Start();
            QRunningManager::instance()->start();
        }
    }
    return app.exec();
}
