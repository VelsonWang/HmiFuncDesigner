#include "Log.h"
#include <QCoreApplication>
#include <QThread>

Log::Log(QObject *parent) : QObject(parent)
{
    QString configPath = QCoreApplication::applicationDirPath() + "/log4j.properties";
    Log4Qt::PropertyConfigurator::configure(configPath);
}

Log::~Log()
{

}

Log* Log::instance()
{
    static Log log;
    return &log;
}

void Log::debug(const QString &log)
{
    logger()->debug(log);
}

void Log::info(const QString &log)
{
    logger()->info(log);
}

void Log::warn(const QString &log)
{
    logger()->warn(log);
}

void Log::error(const QString &log)
{
    logger()->error(log);
}

void Log::fatal(const QString &log)
{
    logger()->fatal(log);
}


bool LogHelper::showFileFuncLine_ = false;

LogHelper::LogHelper(const char *fileName, int lineNumber, const char *functionName)
       : version_(1)
       , line_(lineNumber)
       , file_(fileName)
       , function_(functionName)
{
}

void LogHelper::writelogToLocal(LogHelper::LogType logtype, const QString &log)
{
    QString threadText = QStringLiteral("0x%1").arg(quintptr(QThread::currentThreadId()));
    QString filter = QString("[file(%1)] [func(%2) line(%3) pid(%4)] ")
            .arg(file_)
            .arg(function_)
            .arg(line_)
            .arg(threadText);

    if(showFileFuncLine_) {
        switch (logtype) {
        case LGDebugMsg:
            Log::instance()->debug(filter+log);
            break;
        case LGInfoMsg:
            Log::instance()->info(filter+log);
            break;
        case LGWarningMsg:
            Log::instance()->warn(filter+log);
            break;
        case LGErrorMsg:
            Log::instance()->error(filter+log);
            break;
        case LGFatalMsg:
            Log::instance()->fatal(filter+log);
            break;
        default:
            Log::instance()->info(filter+log);
            break;
        }
    } else {
        switch (logtype) {
        case LGDebugMsg:
            Log::instance()->debug(log);
            break;
        case LGInfoMsg:
            Log::instance()->info(log);
            break;
        case LGWarningMsg:
            Log::instance()->warn(log);
            break;
        case LGErrorMsg:
            Log::instance()->error(log);
            break;
        case LGFatalMsg:
            Log::instance()->fatal(log);
            break;
        default:
            Log::instance()->info(log);
            break;
        }
    }
}


