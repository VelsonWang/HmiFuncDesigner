#include "miscutils.h"
#include <QTextStream>
#include <QSysInfo>
#include <QStorageInfo>
#include <QApplication>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>
#include <QEventLoop>

/**
 * @brief PrintSystemInfo 获取系统信息
 * @return 系统信息
 */
QString printSystemInfo()
{
    QString s = "System Info: \r\n";
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

    out << storageCurrent.rootPath()                                             << endl;
    out << "isReadOnly:" << storageCurrent.isReadOnly()                          << endl;
    out << "name:" << storageCurrent.name()                                      << endl;
    out << "fileSystemType:" << storageCurrent.fileSystemType()                  << endl;
    out << "size:" << storageCurrent.bytesTotal()/1000/1000 << "MB"              << endl;
    out << "availableSize:" << storageCurrent.bytesAvailable()/1000/1000 << "MB" << endl;

    return s;
}

/**
 * @brief hexToString 字符数组转为16进制字符串
 * @param buf 字符数组
 * @param size 需要转换的大小 size <= sizeof(buf)
 * @return 16进制字符串
 */
QString hexToString(char *buf, int size)
{
    QString ret = "";
    for(int i=0; i<size; i++){
        ret = ret + QString("%1").arg(buf[i]&0xff, 2, 16, QChar('0')) + " ";
    }
    ret.chop(1);
    return ret.toUpper();
}

/**
 * @brief getIp 获取所有IPv4的ip
 * @param ipList IPv4的ip列表
 */
void getIp(QStringList &ipList)
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list) {
        if (!address.isNull()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol) {
                //qDebug() << "IPv4 Address : " << address.toString();
                ipList << address.toString();
            }
        }
    }
}

/**
 * @brief delayMs 延时msec毫秒
 * @param msec 毫秒
 */
void delayMs(unsigned int msec)
{
    if(msec > 0) {
        QEventLoop eventloop;
        QTimer::singleShot(msec, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
}


QString trimIPv6Address(const QString& addr)
{
    if ( addr.indexOf("::ffff:") == 0 ) {
        return addr.mid( 7 );
    } else {
        return addr;
    }
}
