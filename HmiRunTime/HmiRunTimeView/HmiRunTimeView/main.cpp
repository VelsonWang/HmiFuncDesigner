#include "HmiRunTime.h"
#include "public.h"
#include "VersionInfo.h"
#include "Log.h"
#include "Global.h"
#include "ConfigUtils.h"
#include "edncrypt.h"
#include "MainWindow.h"
#include "MemoryMessageService.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>


QString getSystemInfo()
{
    QString s = "\n";
    s.append(QString("Build:%1 Version:%2\n").arg(GIT_HASH).arg(VER_FILE));
    s.append("System Info: \n");

    QTextStream out(&s);

    QSysInfo systemInfo;
#ifdef Q_OS_WIN
    out << " Windows Version:          " << systemInfo.windowsVersion()         << endl;
#endif
    out << " Build Cpu Architecture:   " << systemInfo.buildCpuArchitecture()   << endl;
    out << " Current Cpu Architecture: " << systemInfo.currentCpuArchitecture() << endl;
    out << " Kernel Type:              " << systemInfo.kernelType()             << endl;
    out << " Kernel Version:           " << systemInfo.kernelVersion()          << endl;
    out << " Machine Host Name:        " << systemInfo.machineHostName()        << endl;
    out << " Product Type:             " << systemInfo.productType()            << endl;
    out << " Product Version:          " << systemInfo.productVersion()         << endl;
    out << " Byte Order:               " << systemInfo.buildAbi()               << endl;
    out << " Pretty ProductName:       " << systemInfo.prettyProductName()      << endl;

    // root
    QStorageInfo storage = QStorageInfo::root();

    out << storage.rootPath()                                             << endl;
    out << "isReadOnly:" << storage.isReadOnly()                          << endl;
    out << "name:" << storage.name()                                      << endl;
    out << "fileSystemType:" << storage.fileSystemType()                  << endl;
    out << "size:" << storage.bytesTotal()/1000/1000 << "MB"              << endl;
    out << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB" << endl;

    // current volumn
    QStorageInfo storageCurrent(qApp->applicationDirPath());

    out << qApp->applicationDirPath()                                            << endl;
    out << storageCurrent.rootPath()                                             << endl;
    out << "isReadOnly:" << storageCurrent.isReadOnly()                          << endl;
    out << "name:" << storageCurrent.name()                                      << endl;
    out << "fileSystemType:" << storageCurrent.fileSystemType()                  << endl;
    out << "size:" << storageCurrent.bytesTotal()/1000/1000 << "MB"              << endl;
    out << "availableSize:" << storageCurrent.bytesAvailable()/1000/1000 << "MB" << endl;

    return s;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString strInput = "";
    QString configPath = "";

    LogInfo(getSystemInfo());
    LogInfo("start SCADARunTime!");

    QString projPath = QCoreApplication::applicationDirPath() + "/RunProject";
    if(argc == 2) {
        projPath = argv[1];
    }

    QDir dir(projPath);
    if (!dir.exists())
    {
        dir.mkpath(projPath);
    }

    HmiRunTime runTime(projPath);
    runTime.Load(DATA_SAVE_FORMAT);
    // 添加脚本函数至脚本引擎
    HmiRunTime::scriptEngine_ = new QScriptEngine();
    addFuncToScriptEngine(HmiRunTime::scriptEngine_);
    runTime.Start();
    g_pHmiRunTime = &runTime;

    QApplication::processEvents();

    QString projSavePath = QCoreApplication::applicationDirPath() + "/Project";
    QDir projSaveDir(projSavePath);
    if(!projSaveDir.exists()) {
        projSaveDir.mkpath(projSavePath);
    }

    return app.exec();
}
