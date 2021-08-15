#include "ulog.h"
#include <QTextStream>
#include <QStorageInfo>
#include <QThread>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <cstdio>
#include <debugapi.h>
#include <QMutexLocker>
#include <QMutex>

QMutex gMutex;


/**
 * @brief getYearMonthDateFromFileName
 * @details 获取日志文件名中的年月日
 * @param fileName 日志文件名
 * @param year 日志文件名中的年
 * @param month 日志文件名中的月
 * @param day 日志文件名中的日
 */
void getYearMonthDateFromFileName(const QString &fileName, int &year, int &month, int &day)
{
    QString dateStr = QString();
    year = month = day = 0;
    if(fileName.right(7).compare("Log.txt") == 0) {
        dateStr = fileName.left(8);
        if(dateStr.length() == 8) {
            year = dateStr.left(4).toInt();
            month = dateStr.mid(4, 2).toInt();
            day = dateStr.right(2).toInt();
        }
    }
}


/**
 * @brief ULog::delLogFile
 * @details 删除日志文件
 * @param keepdays 保存最近keepdays天的日志文件
 */
void ULog::DelLogFile(int keepdays)
{
    QDir file;
    if(file.exists(m_szLogDir))
    {
        file.setPath(m_szLogDir);
        QFileInfoList flist = file.entryInfoList(QDir::Files|QDir::Readable|QDir::Writable|QDir::Hidden|QDir::NoDotAndDotDot,QDir::Name);
        for(int i=0;i < flist.size();i++)
        {
            int year = 0;
            int month = 0;
            int day = 0;
            QString fileName = flist.at(i).fileName();
            getYearMonthDateFromFileName(fileName, year, month, day);
            QDate fileDate = QDate(year, month, day);
            if(fileDate.addDays(keepdays) < QDate::currentDate())
            {
                file.remove(fileName);
            }
        }
    }
}

/**
 * @brief GetCfgStr
 * @details 读取配置字符串
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sDefault 缺省值
 * @return 键值对应的字符串
 */
QString GetCfgString(QString sFileName,
                     QString sSecName,
                     QString sKey,
                     QString sDefault)
{
    QString retStr = QLatin1String("");
    QSettings settings(sFileName, QSettings::IniFormat);
    settings.beginGroup(sSecName);
    retStr = settings.value(sKey, sDefault).toString();
    settings.endGroup();
    return retStr.trimmed();
}

void SelfLogOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &text)
{
    Q_UNUSED(context)
    switch(type) {
        case QtDebugMsg: LogDebug(text); break;
        case QtWarningMsg: LogWarn(text); break;
        case QtCriticalMsg: LogError(text); break;
        case QtFatalMsg: LogFatal(text); break;
        case QtInfoMsg: LogInfo(text); break;
    }
}

int ULog::m_iTriggerCount = 100;

ULog::ULog(QObject *parent) : QObject(parent)
{
    QString logConfigFilePath = QCoreApplication::applicationDirPath() + "/log.ini";

    QString logPath = QCoreApplication::applicationDirPath() + "/log";
    ELogLevel level = LOG_DEBUG;

    if(QFile::exists(logConfigFilePath))
    {
        //logPath = GetCfgString(logConfigFilePath, "Log", "Path", logPath);
        level = (ELogLevel)GetCfgString(logConfigFilePath, "Log", "Level", QString::number(LOG_DEBUG)).toInt();
    }

    this->SetLogLevel(level);
    this->OpenLogFile(logPath);

    qInstallMessageHandler(SelfLogOutputHandler);

    connect(&m_writeLogTmr, SIGNAL(timeout()), this, SLOT(OnPeriodWriteLog()));
    m_writeLogTmr.start(1000);
}

ULog::~ULog()
{
    if(m_file.isOpen()) {
        WriteLogToFile();
        m_file.close();
    }
}


/**
 * @brief ULog::OpenLogFile
 * @details 打开日志文件
 * @param logpath 日志文件路径, 不含斜杠
 * @return 1：成功，0：失败
 */
int ULog::OpenLogFile(QString logpath)
{
    bool isSuccess = false;

    QString filepath;
    QDir dir;
    try
    {
        if(!dir.exists(logpath))
        {
            dir.mkpath(logpath);
        }
        QDateTime dt = QDateTime::currentDateTime();
        filepath = QString("%1/%2Log.txt").arg(logpath).arg(dt.toString("yyyyMMdd"));
        m_file.setFileName(filepath);
        if(!m_file.isOpen())
        {
            if(m_file.open(QIODevice::ReadWrite))
            {
                m_curTime = dt;
                m_szLogDir = logpath;
                isSuccess = true;

                QString title = GetSystemInfo();
                QByteArray ba = title.toUtf8();
                m_file.seek(m_file.size());
                m_file.write(ba.data());
                m_file.flush();
            }
        }
        else
        {
            isSuccess = true;
        }

        if (isSuccess)
        {
            DelLogFile();
        }
    }
    catch(...)
    {
    }

    return isSuccess;
}

/**
 * @brief ULog::AddLog
 * @details 添加日志
 * @param level 日志等级
 * @param aValue 日志内容
 * @return 1：成功，0：失败
 */
int ULog::AddLog(ELogLevel level, QString aValue)
{
    QString datetime="";
    QString logtype="";
    QString loginfo="";

    QMutexLocker locker(&gMutex);

    if(level < m_logLevel)
    {
        return 0;
    }

    if (m_curTime.date() != QDateTime::currentDateTime().date())
    {
        m_file.close();
        OpenLogFile(m_szLogDir);
    }

    if(!m_file.isOpen())
    {
        return 0;
    }

    switch(level)
    {
        case LOG_DEBUG:
            logtype = "DEBUG";
            break;
        case LOG_INFO:
            logtype = "INFO";
            break;
        case LOG_WARN:
            logtype = "WRAN";
            break;
        case LOG_ERROR:
            logtype = "ERROR";
            break;
		case LOG_FATAL:
			logtype = "FATAL";
			break;
    }

    datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    loginfo = QString("[%1][%2]%3\r\n").arg(datetime).arg(logtype).arg(aValue);
    QByteArray msg = loginfo.toUtf8();

	OutputDebugStringA(loginfo.toLocal8Bit().constData());

    m_szLogs.append(loginfo);
    if(m_szLogs.size() >= m_iTriggerCount) WriteLogToFile();

    return 1;
}

void ULog::SetLogLevel(ELogLevel level)
{
    m_logLevel = level;
}

QString ULog::GetSystemInfo()
{

    QString s = QString("");
    s.append("System Info: \n");

    QTextStream out(&s);
    out << " Application Build Time:   " << QString(__DATE__) << " " << QString(__TIME__) << endl;

    QSysInfo systemInfo;
    out << " Windows Version:          " << systemInfo.windowsVersion()         << endl;
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

void ULog::WriteLogToFile()
{
    QStringList tmpLogs;
    while(!m_szLogs.empty())
    {
        tmpLogs.append(m_szLogs.takeFirst());
    }

    QString szTmpLogs = "";
    foreach(QString szLog, tmpLogs)
    {
        szTmpLogs += szLog;
    }

    m_file.seek(m_file.size());
    m_file.write(szTmpLogs.toUtf8().data());
    m_file.flush();
}

void ULog::OnPeriodWriteLog()
{
    if(m_szLogs.size() > 0) WriteLogToFile();
}

/////////////////////////////////////////////////////////////////////////////////////

bool LogHelper::m_bShowFileFuncLine = false;

LogHelper::LogHelper(const char *fileName, int lineNumber, const char *functionName)
       : m_iVersion(1)
       , m_line(lineNumber)
       , m_file(fileName)
       , m_function(functionName)
{
}

void LogHelper::WritelogToLocal(ELogLevel logtype, const QString &log)
{
    QString threadText = QStringLiteral("0x%1").arg(quintptr(QThread::currentThreadId()));
    QString filter = QString("[file(%1)] [func(%2) line(%3) pid(%4)] ")
            .arg(m_file)
            .arg(m_function)
            .arg(m_line)
            .arg(threadText);

    if(m_bShowFileFuncLine)
    {
        ULog::GetInstance()->AddLog(logtype, filter+log);
    }
    else
    {
        ULog::GetInstance()->AddLog(logtype, log);
    }
}




