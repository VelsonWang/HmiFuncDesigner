#include "SerialPortReMapping.h"
#include "Helper.h"
#include "ConfigUtils.h"
#include <QFile>

SerialPortReMapping::SerialPortReMapping()
{
    // 串口名称重映射
    // 例如：
    // Windows COM1 = COM1
    // Linux COM1 = /dev/ttyS0
    //
    QString szFileSerialCfg = Helper::AppDir() + "/SerialCfg.ini";
    bool bExists = QFile::exists(szFileSerialCfg);

    mapSerialPortName_.clear();

    // 配置文件存在，读取端口映射配置
    if(bExists) {
        for(int i=0; i<32; i++) {
            QString szPortName = QString("COM%1").arg(QString::number(i+1));
#ifdef Q_OS_LINUX
            QString szDefaultPortName = QString("/dev/ttyS%1").arg(QString::number(i));
#elif defined (Q_OS_WIN)
            QString szDefaultPortName = QString("COM%1").arg(QString::number(i+1));
#endif
            mapSerialPortName_[szPortName] = ConfigUtils::getCfgStr(szFileSerialCfg,
                                                                    "SerialPort",
                                                                    szPortName,
                                                                    szDefaultPortName);
        }
    } else {
        for(int i=0; i<32; i++) {
            QString szPortName = QString("COM%1").arg(QString::number(i+1));
#ifdef Q_OS_WIN
            QString szDefaultPortName = QString("COM%1").arg(QString::number(i+1));
#elif defined (Q_OS_LINUX)
            QString szDefaultPortName = QString("/dev/ttyS%1").arg(QString::number(i));
#endif
            mapSerialPortName_[szPortName] = szDefaultPortName;
            ConfigUtils::setCfgStr(szFileSerialCfg, "SerialPort", szPortName, szDefaultPortName);
        }
    }
}

SerialPortReMapping::~SerialPortReMapping()
{

}


/**
 * @brief SerialPortReMapping::getSerialPortName
 * @details 获取重映射后的串口名称
 * @param szOldName 重映射前的串口名称
 * @return 重映射后的串口名称
 */
QString SerialPortReMapping::getSerialPortName(const QString &szOldName) const
{
    return mapSerialPortName_[szOldName];
}


