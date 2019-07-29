#include "ulog.h"
#include <QTextStream>
#include <QStorageInfo>
#include <QThread>
#include <QFile>
#include <QSettings>
#include <QCoreApplication>


QMutex gMutex;
ULog* ULog::log_ = nullptr;
QMutex ULog::mutex_;

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
    if(file.exists(logDir_)) {
        file.setPath(logDir_);
        QFileInfoList flist = file.entryInfoList(QDir::Files|QDir::Readable|QDir::Writable|QDir::Hidden|QDir::NoDotAndDotDot,QDir::Name);
        for(int i=0;i < flist.size();i++) {
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
    if(file_.isOpen()) {
        file_.flush();
        file_.close();
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
        file_.setFileName(filepath);
        if(!file_.isOpen()) {
            if(file_.open(QIODevice::ReadWrite)) {
                curTime_ = dt;
                logDir_ = logpath;
                isSuccess = true;

                QString title = getSystemInfo();
                QByteArray ba = title.toUtf8();
                file_.seek(file_.size());
                file_.write(ba.data());
                file_.flush();
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
    QString datetime="";
    QString logtype="";
    QString loginfo="";

    QMutexLocker locker(&gMutex);

    if(level < logLevel_) {
        return 0;
    }

    if (curTime_.date() != QDateTime::currentDateTime().date()) {
        file_.close();
        openLogFile(logDir_);
    }

    if(!file_.isOpen()) {
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
    }

    datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    loginfo = QString("[%1][%2]%3\r\n").arg(datetime).arg(logtype).arg(aValue);
    QByteArray ba = loginfo.toUtf8();
    try {
        file_.seek(file_.size());
        file_.write(ba.data());
        file_.flush();
    } catch (...) {
        return 0;
    }
    return 1;
}

void ULog::setLogLevel(ELogLevel level)
{
    logLevel_ = level;
}

QString ULog::getSystemInfo() {

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


/////////////////////////////////////////////////////////////////////////////////////


QLogHelper::QLogHelper(const char *fileName, int lineNumber, const char *functionName)
       : version_(1)
       , line_(lineNumber)
       , file_(fileName)
       , function_(functionName)
{
}

void QLogHelper::writelogToLocal(ELogLevel logtype, const QString &log)
{
    QString threadText = QStringLiteral("0x%1").arg(quintptr(QThread::currentThreadId()));
    QString filter = QString("[file(%1)] [func(%2) line(%3) pid(%4)] ")
            .arg(file_)
            .arg(function_)
            .arg(line_)
            .arg(threadText);

    ULog::getInstance()->addLog(logtype, filter+log);
}




