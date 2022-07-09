#ifndef ULOG_H
#define ULOG_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QDebug>

enum ELogLevel {
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

    static ULog* GetInstance()
    {
        static ULog instance;
        return &instance;
    }

    int OpenLogFile(QString logpath);
    int AddLog(ELogLevel level, QString aValue);
    void SetLogLevel(ELogLevel level);

private:
    QString GetSystemInfo();
    void DelLogFile(int keepdays = 30);

private:
    QString m_szLogDir;
    QDateTime m_curTime;
    ELogLevel m_logLevel;
    QFile m_file;
};


////////////////////////////////////////////////////////////////////////////////

class LogHelper
{

public:
    Q_DECL_CONSTEXPR LogHelper() :
        m_iVersion(1),
        m_line(0),
        m_file(0),
        m_function(0)
    {

    }

    LogHelper(const char *fileName, int lineNumber, const char *functionName);

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
    void Debug(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        WritelogToLocal(LOG_DEBUG, logmsg);
    }

    template <typename T, typename ... Args>
    void Info(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        WritelogToLocal(LOG_INFO, logmsg);
    }

    template <typename T, typename ... Args>
    void Warn(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        WritelogToLocal(LOG_WARN, logmsg);
    }

    template <typename T, typename ... Args>
    void Error(T head, Args ... args )
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        WritelogToLocal(LOG_ERROR, logmsg);
    }

    template <typename T, typename ... Args>
    void Fatal(T head, Args ... args)
    {
        QString logmsg = QString("%1 ").arg(head) + TemplateParameter(args...);
        WritelogToLocal(LOG_FATAL, logmsg);
    }

    template <typename T>
    inline LogHelper &operator<<(T logmsg)
    {
        WritelogToLocal(LOG_INFO, QString("%1").arg(logmsg));
        return *this;
    }

private:
    virtual void WritelogToLocal(ELogLevel logtype, const QString& logmsg);

    virtual void Copy(const LogHelper &logContext)
    {
        Q_UNUSED(logContext)
    }

public:
    static bool m_bShowFileFuncLine;

private:
    int m_iVersion;
    int m_line;
    const char *m_file;
    const char *m_function;


    Q_DISABLE_COPY(LogHelper)
};


////////////////////////////////////////////////////////////////////////////////


#define LogDebug LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).Debug
#define LogInfo  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).Info
#define LogWarn  LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).Warn
#define LogError LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).Error
#define LogFatal LogHelper(__FILE__, __LINE__, Q_FUNC_INFO).Fatal
#define Logqt    LogHelper(__FILE__, __LINE__, Q_FUNC_INFO)

#endif // ULOG_H
