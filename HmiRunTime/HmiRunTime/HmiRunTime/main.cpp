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

/**
 * @brief getProjectName
 * @details 获取工程名称
 * @param szProjectPath
 * @return
 */
QString getProjectName(const QString &szProjectPath)
{
    QFileInfo srcFileInfo(szProjectPath);
    QString szProjName = "";

    if(srcFileInfo.isDir()) {
        QDir sourceDir(szProjectPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        foreach(const QString &fileName, fileNames) {
            if(fileName.endsWith(".pdt")) {
                QFileInfo info(fileName);
                szProjName = info.baseName();
            }
        }
    }

    return szProjName;
}

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
//    LogInit();
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

    // find project infomation file
    QString szProjName = getProjectName(szRunProjPath);

    if(szProjName == "") {
        qCritical() << "project config file not found!";
        return -1;
    } else {
        qRegisterWidgets();
        QString szProjFile = szRunProjPath + "/" + szProjName + ".pdt";

        ///////////////////////////////////////////////////////////////////////////
        /// 启动定时任务
        TimerTask tmrTask;

        if(QRunningManager::instance()->load(szProjFile)) {
            HmiRunTime runTime(QRunningManager::instance()->projCore());
            runTime.Load();
            runTime.Start();
            g_pHmiRunTime = &runTime;
            QRunningManager::instance()->start();
        }
        return app.exec();
    }
    return -1;
}
