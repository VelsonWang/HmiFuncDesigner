#include "HmiRunTime.h"
//#include "public.h"
#include "VersionInfo.h"
//#include "Global.h"
//#include "ConfigUtils.h"
//#include "edncrypt.h"
//#include "MainWindow.h"
//#include "MemoryMessageService.h"
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
#include <QScriptEngine>
#include "qsoftcore.h"
#include "qprojectcore.h"
#include "qrunningmanager.h"
#include "../../libs/shared/qbaseinit.h"
#include "realtimedb.h"
#include "httpserver.h"
#include "TimerTask.h"

#ifdef USE_SOAP_SERVICE
    #include "gSOAPServer.h"
#endif

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
    LogInit();
    qDebug() << "start hmiruntime.";
    //////////////////////////////////////////////////////////////////////////////
    ///  启动http服务
    HttpServer httpServer;
    httpServer.init(60000);
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
        qRegisterBaseProperty();
        qRegisterBaseHost();
        QRunningManager runningMgr;
        QString szProjFile = szRunProjPath + "/" + szProjName + ".pdt";

        ///////////////////////////////////////////////////////////////////////////
        /// 启动定时任务
        TimerTask *pTimerTask = new TimerTask();

        if(runningMgr.load(szProjFile)) {
            HmiRunTime runTime(runningMgr.projCore());
            runTime.Load();
            // 添加脚本函数至脚本引擎
            HmiRunTime::scriptEngine_ = new QScriptEngine();
            //addFuncToScriptEngine(HmiRunTime::scriptEngine_);
            runTime.Start();
            g_pHmiRunTime = &runTime;
            runningMgr.start();
        }

        int ret = app.exec();

        delete pTimerTask;

        return ret;
    }

#if 0
    //////////////////////////////////////////////////////////////////////////////
    ///  启动http服务
    HttpServer httpServer;
    httpServer.init(60000);
    //////////////////////////////////////////////////////////////////////////////
    QString szProjPath = QCoreApplication::applicationDirPath() + "/RunProject";

    if(argc == 2) {
        szProjPath = argv[1];
    }

    QDir dir(szProjPath);

    if(!dir.exists()) {
        dir.mkpath(szProjPath);
    }

    HmiRunTime runTime(szProjPath);
    runTime.Load(DATA_SAVE_FORMAT);
    runTime.Start();
    g_pHmiRunTime = &runTime;
    QApplication::processEvents();
    QString projSavePath = QCoreApplication::applicationDirPath() + "/Project";
    QDir projSaveDir(projSavePath);

    if(!projSaveDir.exists()) {
        projSaveDir.mkpath(projSavePath);
    }

    ///////////////////////////////////////////////////////////////////////////
    /// 启动SOAP服务
#ifdef USE_SOAP_SERVICE
    SOAPServer gSOAPServer("0.0.0.0", 60002);
#endif

#ifdef USE_SOAP_SERVICE
    gSOAPServer.exitService();
#endif

    // 释放插件对象
    VendorPluginManager::getInstance()->releasePlugin();
#endif
    return -1;
}
