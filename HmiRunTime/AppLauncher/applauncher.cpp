#include "applauncher.h"
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <QThread>
#include <QDebug>


AppLauncher::AppLauncher(QObject *parent) :
    QObject(parent)
{
    connect(&m_tmrCheckProcess, &QTimer::timeout, this, &AppLauncher::timeToCheckProcess);
    m_tmrCheckProcess.start(1000);
}

AppLauncher::~AppLauncher()
{
    QMap<qint64, RunningApp>::Iterator it;
    for (it = runningApps.begin(); it!= runningApps.end(); ++it) {
        RunningApp app = it.value();
        app.proc->kill();
        app.proc->waitForFinished(5000);
        if(app.proc != Q_NULLPTR) {
            delete app.proc;
            app.proc = Q_NULLPTR;
        }
    }
    runningApps.clear();
}

///
/// \brief AppLauncher::isRunning
/// \param szAppName 程序文件名
/// \return true-运行, false-非运行
///
bool AppLauncher::isRunning(const QString &szAppName)
{
    QMap<qint64, RunningApp>::ConstIterator it;
    for(it = runningApps.begin(); it != runningApps.end(); ++it) {
        RunningApp app = it.value();
        if (app.name == szAppName) {
            return app.proc && app.proc->state() != QProcess::NotRunning;
        }
    }

    return false;
}

///
/// \brief AppLauncher::execute
/// \param szAppName 程序文件名
/// \param args 附加参数
/// \return true-执行, false-非执行
///
bool AppLauncher::execute(const QString &szAppName, const QStringList &args)
{
    if (isRunning(szAppName)) return true;

    bool bStarted = false;
    QFileInfo fileInfo(szAppName);
    if(fileInfo.exists() && fileInfo.isFile()) {
        QString szAppPath = fileInfo.path();
        //qDebug() << "szAppPath: " << szAppPath;
        QProcess *proc = new QProcess;
        QObject::connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                         this, &AppLauncher::processTerminated);
        proc->setWorkingDirectory(szAppPath);
        if(args.size()) {
            proc->start(szAppName, args);
        } else {
            proc->start(szAppName);
        }
        bStarted = proc->waitForStarted(5000);
        if (!bStarted) {
            qDebug() << "Unable to start application" << szAppName;
            delete proc;
        } else {
            appLaunched(proc, szAppName);
        }
    } else {
        QMessageBox::information(Q_NULLPTR, tr("提示"), QString(tr("执行文件：%1 有误！")).arg(szAppName));
    }

    return bStarted;
}

void AppLauncher::appLaunched(QProcess *proc, const QString &szAppName)
{
    RunningApp app;
    app.name = szAppName;
    app.proc = proc;
    qint64 pid = proc->processId();
    runningApps[pid] = app;
    runningList.removeOne(szAppName);
    runningList.prepend(szAppName);
    emit launched(pid, szAppName);
}

///
/// \brief AppLauncher::kill
/// \details 结束进程
/// \param pid 进程ID
///
void AppLauncher::kill(qint64 pid)
{
    QMap<qint64, RunningApp>::Iterator it;
    for (it = runningApps.begin(); it!= runningApps.end(); ++it) {
        RunningApp app = it.value();
        if (it.key() == pid) {
            app.proc->kill();
            break;
        }
    }
}


///
/// \brief AppLauncher::pidForName
/// \param szAppName 程序文件名
/// \return 进程ID
///
qint64 AppLauncher::pidForName(const QString &szAppName)
{
    QMap<qint64, RunningApp>::Iterator it;
    for (it = runningApps.begin(); it!= runningApps.end(); ++it) {
        RunningApp app = it.value();
        if (app.name == szAppName) {
            return it.key();
        }
    }

    return -1;
}

void AppLauncher::processTerminated(int exitCode, QProcess::ExitStatus exitStatus)
{
    // get the process that exited.
    QProcess *proc = (QProcess *)sender();
    if (!proc) {
        qDebug("Internal error NULL proc");
        return;
    }

    const QString binary = QDir::toNativeSeparators(proc->program());
    if (exitStatus != QProcess::NormalExit) {
        qDebug("%s: crashed.", qPrintable(binary));
    } else if (exitCode != 0) {
        qDebug("%s: terminated with exit code %d.", qPrintable(binary), exitCode);
    }

    bool found = false;
    qDebug() << "Removing application " << binary;

    // Search for the app to find its PID
    QMap<qint64, RunningApp>::Iterator it;
    for(it = runningApps.begin(); it != runningApps.end(); ++it) {
        RunningApp app = it.value();
        if (app.name == binary){
            found = true;
            break;
        }
    }

    if (found) {
        RunningApp app = it.value();
        runningList.removeOne(app.name);
        emit terminated(it.key(), app.name);
        runningApps.remove(it.key());
        if(app.proc != Q_NULLPTR) {
            delete app.proc;
            app.proc = Q_NULLPTR;
        }
    } else {
        qDebug() << QString("Internal error application %1 not listed as running").arg(binary);
    }
}

void AppLauncher::criticalKill(const QString &szAppName)
{   
    qint64 pid = pidForName(szAppName);
    kill(pid);
    emit terminated(pid, szAppName);
}

void AppLauncher::timeToCheckProcess()
{
    if(runningApps.size() < 1) return;
    QMap<qint64, RunningApp>::Iterator it;
    for(it = runningApps.begin(); it != runningApps.end(); ++it) {
        RunningApp app = it.value();
        qDebug() << QString("process [id: %1 name: %2]").arg(it.key(), 8, 10, QChar('0')).arg(app.name);
    }
    qDebug() << "----------------------------------------";
}

extern QString g_szAppFileTransferTool; // 工程文件传输工具文件
extern QString g_szAppHmiRunTimeData; // 数据采集程序文件
extern QString g_szAppHmiRunTimeView; // 画面视图程序文件

void AppLauncher::onMessageReceived(const QString &szMsg)
{
    qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__ << "messageReceived: " << szMsg;
    if(szMsg.toLower() == "download_project") {
        criticalKill(g_szAppHmiRunTimeData);
        int iCnt = 0;
        while (isRunning(g_szAppHmiRunTimeData)) {
            QThread::msleep(100);
            iCnt++;
            if(iCnt > 50) {
                qDebug() << "stop application: " << g_szAppHmiRunTimeData << " fail!";
                break;
            }
        }
        if(!isRunning(g_szAppHmiRunTimeData)) {
            execute(g_szAppHmiRunTimeData, QStringList());
        }

        //----------------------------------------------------------------------

        criticalKill(g_szAppHmiRunTimeView);
        iCnt = 0;
        while (isRunning(g_szAppHmiRunTimeView)) {
            QThread::msleep(100);
            iCnt++;
            if(iCnt > 50) {
                qDebug() << "stop application: " << g_szAppHmiRunTimeView << " fail!";
                break;
            }
        }
        if(!isRunning(g_szAppHmiRunTimeView)) {
            execute(g_szAppHmiRunTimeView, QStringList());
        }
    }
}
