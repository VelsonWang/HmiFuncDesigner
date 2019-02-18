#include <QCoreApplication>
#include <QApplication>
#include "tcpserver.h"
#include "SCADARunTime.h"
#include "public.h"
#include "TimerTask.h"
#include "httpserver.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <iostream>


#include "log4qt/consoleappender.h"
#include "log4qt/logger.h"
#include "log4qt/logmanager.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/basicconfigurator.h"


#ifndef Q_OS_WIN
#include "eventdispatcher_libev/eventdispatcher_libev.h"
#endif

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,const QString & msg)
{
        QString txt;
        switch (type) {
        //调试信息提示
        case QtDebugMsg:
//            txt = QString("%1: Debug: at:%2,%3 on %4; %5").arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
//                    .arg(context.file).arg(context.line).arg(context.function).arg(msg);
            txt = msg;
                break;

        //一般的warning提示
        case QtWarningMsg:
                txt = QString("%1:Warning: at:%2,%3 on %4; %5").arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                        .arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
        //严重错误提示
        case QtCriticalMsg:
                txt = QString("%1:Critical: at:%2,%3 on %4; %5").arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                        .arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
        //致命错误提示
        case QtFatalMsg:
                txt = QString("%1:Fatal: at:%2,%3 on %4; %5").arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                        .arg(context.file).arg(context.line).arg(context.function).arg(msg);
                abort();
        }
        QFile outFile(QString("%1/%2.txt").arg(QDir::currentPath()).arg(QDate::currentDate().toString("yyyy-MM-dd")));
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    //    qInstallMessageHandler(customMessageHandler);
#ifndef Q_OS_WIN
    //QCoreApplication::setEventDispatcher(new EventDispatcherLibEv());
#endif
    //QCoreApplication a(argc, argv);
    QApplication  a(argc, argv);
    //std::cout << "???" << std::endl;

    Log4Qt::BasicConfigurator::configure();
    Log4Qt::PropertyConfigurator::configure(QCoreApplication::applicationDirPath() + "/log4j.properties");

    Log4Qt::Logger *a1 = Log4Qt::Logger::logger("Run_Logger");
    a1->info("start SCADARunTime!");

    HttpServer server;
    server.init(60000);

    QString projPath = QCoreApplication::applicationDirPath() + "/RunProject";
    QDir dir(projPath);
    if (!dir.exists())
    {
        dir.mkpath(projPath);
    }
    SCADARunTime runTime(projPath);
    runTime.Load(DATA_SAVE_FORMAT);
    runTime.Start();

    QString projSavePath = QCoreApplication::applicationDirPath() + "/Project";
    QDir projSaveDir(projSavePath);
    if(!projSaveDir.exists())
    {
        projSaveDir.mkpath(projSavePath);
    }
    TcpServer ser(&runTime);
    ser.listen(QHostAddress::Any, 6000);

    TimerTask *pTimerTask = new TimerTask();

    ret = a.exec();

    delete pTimerTask;

    return ret;
}
