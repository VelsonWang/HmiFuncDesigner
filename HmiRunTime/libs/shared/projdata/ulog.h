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
#include "../sharedlibglobal.h"

enum ELogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

class SHAREDLIB_EXPORT ULog : public QObject
{
    Q_OBJECT

public:
    explicit ULog(QObject *parent = 0);
    ~ULog();

    static ULog* getInstance(QObject *parent = 0)
    {
        QMutexLocker locker(&m_mutex);
        if(m_log == NULL) {
            m_log = new ULog(parent);
            ::atexit(deleteInstance);
        }
        return m_log;
    }

    static void deleteInstance()
    {
        if(m_log != NULL) {
            delete m_log;
            m_log = NULL;
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
    QString m_logDir;
    QDateTime m_curTime;
    ELogLevel m_logLevel;
    QFile m_file;
    static ULog* m_log;
    static QMutex m_mutex;
};


////////////////////////////////////////////////////////////////////////////////

class SHAREDLIB_EXPORT QLogHelper
{

public:
    Q_DECL_CONSTEXPR QLogHelper() :
        m_version(1),
        m_line(0),
        m_file(0),
        m_func(0)
    {

    }

    QLogHelper(const char *fileName, int lineNumber, const char *functionName);

    QString TemplateParameter ()
    {
        return "";
    }

    template <typename T, typename ... Args>
    QString TemplateParameter(T head, Args ... args)
    {
        return QString("%1 ").arg(head) + TemplateParameter(args...);
    }

    template <typename T, typename ... Args>
    void debug(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_DEBUG, logmsg);
    }

    template <typename T, typename ... Args>
    void info(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_INFO, logmsg);
    }

    template <typename T, typename ... Args>
    void warn(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_WARN, logmsg);
    }

    template <typename T, typename ... Args>
    void error(T head, Args ... args )
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_ERROR, logmsg);
    }

    template <typename T, typename ... Args>
    void fatal(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        writelogToLocal(LOG_FATAL, logmsg);
    }

    template <typename T>
    inline QLogHelper &operator<<(T logmsg)
    {
        writelogToLocal(LOG_INFO, QString("%1").arg(logmsg));
        return *this;
    }

private:
    virtual void writelogToLocal(ELogLevel logtype, const QString& logmsg);

    virtual void copy(const QLogHelper &logContext)
    {
        Q_UNUSED(logContext)
    }

public:
    // 日志是否记录文件名，函数名称，代码行信息
    // true-记录, false-不记录
    static bool m_showFileFuncLine;

private:
    int m_version;
    int m_line;
    const char *m_file;
    const char *m_func;

    Q_DISABLE_COPY(QLogHelper)
};


////////////////////////////////////////////////////////////////////////////////


#define LogDebug QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO).debug
#define LogInfo  QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO).info
#define LogWarn  QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO).warn
#define LogError QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO).error
#define LogFatal QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO).fatal
#define Logqt    QLogHelper(__FILE__, __LINE__, Q_FUNC_INFO)

#endif // ULOG_H
