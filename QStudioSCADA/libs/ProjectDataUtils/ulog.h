#ifndef ULOG_H
#define ULOG_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutexLocker>
#include <QMutex>
#include <stdlib.h>


enum ELogLevel{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

class ULog : public QObject
{
    Q_OBJECT

public:
    explicit ULog(QObject *parent = 0);
    ~ULog();

    static ULog* getInstance(QObject *parent = 0)
    {
        QMutexLocker locker(&mutex_);
        if(log_ == NULL) {
            log_ = new ULog(parent);
            ::atexit(deleteInstance);
        }
        return log_;
    }

    static void deleteInstance()
    {
        if(log_ != nullptr) {
            delete log_;
            log_ = nullptr;
        }
    }

    int openLogFile(QString logpath);
    int addLog(ELogLevel level, QString aValue);
    void setLogLevel(ELogLevel level);

private:
    QString getSystemInfo();
    void delLogFile(int keepdays = 30);

signals:

public slots:

private:
    QString logDir_;
    QDateTime curTime_;
    ELogLevel logLevel_;
    QFile file_;
    static ULog* log_;
    static QMutex mutex_;
};


////////////////////////////////////////////////////////////////////////////////

class LogHelper
{

public:
    Q_DECL_CONSTEXPR LogHelper() :
        version_(1),
        line_(0),
        file_(0),
        function_(0) {

    }

    LogHelper(const char *fileName, int lineNumber, const char *functionName);

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
        writelogToLocal(LOG_DEBUG,logmsg);
    }

    template <typename T, typename ... Args>
    void info(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_INFO,logmsg);
    }

    template <typename T, typename ... Args>
    void warn(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_WARN,logmsg);
    }

    template <typename T, typename ... Args>
    void error(T head, Args ... args ){
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_ERROR,logmsg);
    }

    template <typename T, typename ... Args>
    void fatal(T head, Args ... args) {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_FATAL,logmsg);
    }

    template <typename T>
    inline LogHelper &operator<<(T logmsg) {
        writelogToLocal(LOG_INFO, QString("%1").arg(logmsg));
        return *this;
    }

private:
    virtual void writelogToLocal(ELogLevel logtype, const QString& logmsg);

    virtual void copy(const LogHelper &logContext){
        Q_UNUSED(logContext)
    }

private:
    int version_;
    int line_;
    const char *file_;
    const char *function_;

    Q_DISABLE_COPY(LogHelper)
};


////////////////////////////////////////////////////////////////////////////////


#define LogDebug LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).debug
#define LogInfo  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).info
#define LogWarn  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).warn
#define LogError LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).error
#define LogFatal LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).fatal
#define Logqt    LogHelper(__FILE__, __LINE__, Q_FUNC_INFO)

#endif // ULOG_H
