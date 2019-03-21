#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <cstdlib>
#include <mutex>
#include <cassert>
#include "log4qt/consoleappender.h"
#include "log4qt/logger.h"
#include "log4qt/logmanager.h"
#include "log4qt/ttcclayout.h"
#include "log4qt/propertyconfigurator.h"
#include "log4qt/basicconfigurator.h"


#define LogDebug LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).debug
#define LogInfo  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).info
#define LogWarn  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).warn
#define LogError LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).error
#define LogFatal LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).fatal
#define LogQt    LogHelper(__FILE__, __LINE__, Q_FUNC_INFO)

class Log : public QObject
{ 
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    Log(QObject *parent = 0);
    ~Log();

    static Log* instance();

    virtual void debug(const QString &log);
    virtual void info(const QString &log);
    virtual void warn(const QString &log);
    virtual void error(const QString &log);
    virtual void fatal(const QString &log);

};

class LogHelper{
    Q_DISABLE_COPY(LogHelper)

public:
    Q_DECL_CONSTEXPR LogHelper() :
        version_(1),
        line_(0),
        file_(0),
        function_(0) {

    }

    LogHelper(const char *fileName, int lineNumber, const char *functionName);

    enum LogType {
        LGDebugMsg,
        LGInfoMsg,
        LGWarningMsg,
        LGErrorMsg,
        LGFatalMsg
    };

    QString TemplateParameter () {
        return "";
    }

    template <typename T, typename ... Args>
    QString TemplateParameter(T head, Args ... args) {
        return QString("%1 ").arg(head) + TemplateParameter(args...);
    }

    template <typename T, typename ... Args>
    void debug(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LGDebugMsg,logmsg);
    }

    template <typename T, typename ... Args>
    void info(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LGInfoMsg,logmsg);
    }

    template <typename T, typename ... Args>
    void warn(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LGWarningMsg,logmsg);
    }

    template <typename T, typename ... Args>
    void error(T head, Args ... args ){
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LGErrorMsg,logmsg);
    }

    template <typename T, typename ... Args>
    void fatal(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LGFatalMsg,logmsg);
    }

    virtual void writelogToLocal(LogType logtype, const QString& logmsg);
    virtual void copy(const LogHelper &logContext){}

    template <typename T>
    inline LogHelper &operator<<(T logmsg) {
        writelogToLocal(LGInfoMsg, QString("%1").arg(logmsg));
        return *this;
    }

    int version_;
    int line_;
    const char *file_;
    const char *function_;
};


#endif // LOG_H
