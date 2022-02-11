#include "ulog.h"
#include <QTextStream>
#include <QStorageInfo>
#include <QThread>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>


QMutex gMutex;
ULog* ULog::m_log = NULL;
QMutex ULog::m_mutex;

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
void ULog::delLogFile(int keepdays)
{
    QDir file;
    if(file.exists(m_logDir)) {
        file.setPath(m_logDir);
        QFileInfoList flist = file.entryInfoList(QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);
        for(int i = 0; i < flist.size(); i++) {
            int year = 0;
            int month = 0;
            int day = 0;
            QString fileName = flist.at(i).fileName();
            getYearMonthDateFromFileName(fileName, year, month, day);
            QDate fileDate = QDate(year, month, day);
            if(fileDate.addDays(keepdays) < QDate::currentDate()) {
                file.remove(fileName);
            }
        }
    }
}

/**
 * @brief getCfgStr
 * @details 读取配置字符串
 * @param sFileName ini文件名称
 * @param sSecName 组名称
 * @param sKey 键名称
 * @param sDefault 缺省值
 * @return 键值对应的字符串
 */
QString getCfgString(QString sFileName,
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

ULog::ULog(QObject *parent) :
    QObject(parent)
{
    QString logConfigFilePath = QCoreApplication::applicationDirPath() + "/log.ini";

    QString logPath = QCoreApplication::applicationDirPath() + "/log";
    ELogLevel level = LOG_DEBUG;

    if(QFile::exists(logConfigFilePath)) {
        logPath = getCfgString(logConfigFilePath, "Log", "Path", logPath);
        level = (ELogLevel)getCfgString(logConfigFilePath, "Log", "Level", QString::number(LOG_DEBUG)).toInt();
    }

    this->setLogLevel(level);
    this->openLogFile(logPath);
}

ULog::~ULog()
{
    if(m_file.isOpen()) {
        m_file.flush();
        m_file.close();
    }
}


/**
 * @brief ULog::openLogFile
 * @details 打开日志文件
 * @param logpath 日志文件路径, 不含斜杠
 * @return 1：成功，0：失败
 */
int ULog::openLogFile(QString logpath)
{
    bool isSuccess = false;

    QString filepath;
    QDir dir;
    try {
        if(!dir.exists(logpath)) {
            dir.mkpath(logpath);
        }
        QDateTime dt = QDateTime::currentDateTime();
        filepath = QString("%1/%2Log.txt").arg(logpath).arg(dt.toString("yyyyMMdd"));
        m_file.setFileName(filepath);
        if(!m_file.isOpen()) {
            if(m_file.open(QIODevice::ReadWrite)) {
                m_curTime = dt;
                m_logDir = logpath;
                isSuccess = true;

                QString title = getSystemInfo();
                QByteArray ba = title.toUtf8();
                m_file.seek(m_file.size());
                m_file.write(ba.data());
                m_file.flush();
            }
        } else {
            isSuccess = true;
        }

        if (isSuccess) {
            delLogFile();
        }
    } catch(...) {
    }

    return isSuccess;
}

/**
 * @brief ULog::addLog
 * @details 添加日志
 * @param level 日志等级
 * @param aValue 日志内容
 * @return 1：成功，0：失败
 */
int ULog::addLog(ELogLevel level, QString aValue)
{
    QString datetime = "";
    QString logtype = "";
    QString loginfo = "";

    QMutexLocker locker(&gMutex);

    if(level < m_logLevel) {
        return 0;
    }

    if (m_curTime.date() != QDateTime::currentDateTime().date()) {
        m_file.close();
        openLogFile(m_logDir);
    }

    if(!m_file.isOpen()) {
        return 0;
    }

    switch(level) {
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
    }

    datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    loginfo = QString("[%1][%2]%3\r\n").arg(datetime).arg(logtype).arg(aValue);
    QByteArray ba = loginfo.toUtf8();
    try {
        m_file.seek(m_file.size());
        m_file.write(ba.data());
        m_file.flush();
    } catch (...) {
        return 0;
    }
    return 1;
}

void ULog::setLogLevel(ELogLevel level)
{
    m_logLevel = level;
}

QString ULog::getSystemInfo()
{

    QString s = QString("");
    s.append("System Info: \n");

    QTextStream out(&s);

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
    out << "size:" << storage.bytesTotal() / 1000 / 1000 << "MB"              << endl;
    out << "availableSize:" << storage.bytesAvailable() / 1000 / 1000 << "MB" << endl;

    // current volumn
    QStorageInfo storageCurrent(qApp->applicationDirPath());

    out << qApp->applicationDirPath()                                            << endl;
    out << storageCurrent.rootPath()                                             << endl;
    out << "isReadOnly:" << storageCurrent.isReadOnly()                          << endl;
    out << "name:" << storageCurrent.name()                                      << endl;
    out << "fileSystemType:" << storageCurrent.fileSystemType()                  << endl;
    out << "size:" << storageCurrent.bytesTotal() / 1000 / 1000 << "MB"              << endl;
    out << "availableSize:" << storageCurrent.bytesAvailable() / 1000 / 1000 << "MB" << endl;

    return s;
}


/////////////////////////////////////////////////////////////////////////////////////

bool QLogHelper::m_showFileFuncLine = false;

QLogHelper::QLogHelper(const char *fileName, int lineNumber, const char *functionName)
    : m_version(1)
    , m_line(lineNumber)
    , m_file(fileName)
    , m_func(functionName)
{
}

void QLogHelper::writelogToLocal(ELogLevel logtype, const QString &log)
{
    QString threadText = QStringLiteral("0x%1").arg(quintptr(QThread::currentThreadId()));
    QString filter = QString("[file(%1)] [func(%2) line(%3) pid(%4)] ")
                     .arg(m_file)
                     .arg(m_func)
                     .arg(m_line)
                     .arg(threadText);

    if(m_showFileFuncLine) {
        ULog::getInstance()->addLog(logtype, filter + log);
    } else {
        ULog::getInstance()->addLog(logtype, log);
    }
}




