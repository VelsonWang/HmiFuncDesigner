#include "TCPIPModbus.h"



TCPIPModbus::TCPIPModbus()
{

}


/*
* 获取设备类型名称
*/
QString TCPIPModbus::GetDeviceTypeName()
{
    return QString(tr("网络设备"));
}

/*
* 获取设备支持的所有协议
*/
QStringList TCPIPModbus::GetDeviceSupportProtocol()
{
    QStringList list;
    list<<"TCPIPModbus";
    return list;
}

/*
* 获取设备支持的所有寄存器区
*/
QStringList TCPIPModbus::GetDeviceSupportRegisterArea()
{
    QStringList list;
    list << tr("DO线圈")
         << tr("DI离散输入寄存器")
         << tr("AO保持寄存器")
         << tr("AI输入寄存器");
    return list;
}

/*
* 获取设备支持的所有数据类型
*/
QStringList TCPIPModbus::GetDeviceSupportDataType()
{
    QStringList list;
    list << tr("Bit1开关量")
         << tr("Char8位有符号数")
         << tr("Byte8位无符号数")
         << tr("Short16位有符号数")
         << tr("Word16位无符号数")
         << tr("ASCII2个字符")
         << tr("Long32位有符号数")
         << tr("Dword32位无符号数")
         << tr("Float单精度浮点数")
         << tr("String字符串")
         << tr("Double双精度浮点数")
         << tr("BCD");
    return list;
}

/**
 * @brief TCPIPModbus::GetRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param areaName 寄存器区名称
 * @param lowerLimit 寄存器区地址下限
 * @param upperLimit 寄存器区地址上限
 */
void TCPIPModbus::GetRegisterAreaLimit(const QString &areaName,
                                       quint32 &lowerLimit,
                                       quint32 &upperLimit) {
    lowerLimit = 0;
    upperLimit = 0;
    if(areaName == tr("DO线圈")) {
        lowerLimit = 0x0000;
        upperLimit = 0xFFFF;
    } else if(areaName == tr("DI离散输入寄存器")) {
        lowerLimit = 0x0000;
        upperLimit = 0xFFFF;
    } else if(areaName == tr("AO保持寄存器")) {
        lowerLimit = 0x0000;
        upperLimit = 0xFFFF;
    } else if(areaName == tr("AI输入寄存器")) {
        lowerLimit = 0x0000;
        upperLimit = 0xFFFF;
    }
    if(!m_bStartAddrBit0) lowerLimit++;
}

///
/// \brief TCPIPModbus::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void TCPIPModbus::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
    properties.append(qMakePair(tr("通信失败重试次数n次"), QString("3")));
    properties.append(qMakePair(tr("通信超时时间n毫秒"), QString("1000")));
    properties.append(qMakePair(tr("尝试恢复通讯间隔时间n毫秒"), QString("15000")));
    properties.append(qMakePair(tr("内存地址起始位为0"), QString("true")));
    properties.append(qMakePair(tr("写线圈功能码为15"), QString("false")));
    properties.append(qMakePair(tr("写寄存器功能码为16"), QString("false")));
    properties.append(qMakePair(tr("8位逆序"), QString("false")));
    properties.append(qMakePair(tr("16位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("32位低字节在前高字节在后"), QString("false")));
    properties.append(qMakePair(tr("64位低字节在前高字节在后"), QString("false")));
}


///
/// \brief TCPIPModbus::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void TCPIPModbus::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();
    properties_type.append(qMakePair(tr("通信失败重试次数n次"), QString("int")));
    properties_type.append(qMakePair(tr("通信超时时间n毫秒"), QString("int")));
    properties_type.append(qMakePair(tr("尝试恢复通讯间隔时间n毫秒"), QString("int")));
    properties_type.append(qMakePair(tr("内存地址起始位为0"), QString("bool")));
    properties_type.append(qMakePair(tr("写线圈功能码为15"), QString("bool")));
    properties_type.append(qMakePair(tr("写寄存器功能码为16"), QString("bool")));
    properties_type.append(qMakePair(tr("8位逆序"), QString("bool")));
    properties_type.append(qMakePair(tr("16位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("32位低字节在前高字节在后"), QString("bool")));
    properties_type.append(qMakePair(tr("64位低字节在前高字节在后"), QString("bool")));
}



///
/// \brief TCPIPModbus::devicePropertiesToString
/// \details 保存属性为字符串
/// \param properties 属性
/// \return 属性字符串
///
QString TCPIPModbus::devicePropertiesToString(QVector<QPair<QString, QString>>& properties)
{
    QStringList szListProperties;
    szListProperties << QString(tr("retryTimes=%1")).arg(getValue2ByValue1(tr("通信失败重试次数n次"), properties));
    szListProperties << QString(tr("commTimeout=%1")).arg(getValue2ByValue1(tr("通信超时时间n毫秒"), properties));
    szListProperties << QString(tr("commResumeTime=%1")).arg(getValue2ByValue1(tr("尝试恢复通讯间隔时间n毫秒"), properties));
    szListProperties << QString(tr("startAddrBit=%1")).arg(getValue2ByValue1(tr("内存地址起始位为0"), properties));
    szListProperties << QString(tr("writeCoilFn=%1")).arg(getValue2ByValue1(tr("写线圈功能码为15"), properties));
    szListProperties << QString(tr("writeRegFn=%1")).arg(getValue2ByValue1(tr("写寄存器功能码为16"), properties));
    szListProperties << QString(tr("addr8=%1")).arg(getValue2ByValue1(tr("8位逆序"), properties));
    szListProperties << QString(tr("addr16=%1")).arg(getValue2ByValue1(tr("16位低字节在前高字节在后"), properties));
    szListProperties << QString(tr("addr32=%1")).arg(getValue2ByValue1(tr("32位低字节在前高字节在后"), properties));
    szListProperties << QString(tr("addr64=%1")).arg(getValue2ByValue1(tr("64位低字节在前高字节在后"), properties));
    return szListProperties.join("|");
}


///
/// \brief TCPIPModbus::devicePropertiesFromString
/// \details 从字符串加载属性
/// \param szProperty 属性字符串
/// \param properties 属性
///
void TCPIPModbus::devicePropertiesFromString(const QString &szProperty, QVector<QPair<QString, QString>>& properties)
{
    properties.clear();

    QStringList szListProperties = szProperty.split('|');
    foreach(QString szProp, szListProperties) {
        QStringList szListKeyVal = szProp.split('=');
        if(szListKeyVal.size() == 2) {
            if(szListKeyVal.at(0) == "retryTimes") {
                properties.append(qMakePair(tr("通信失败重试次数n次"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "commTimeout") {
                properties.append(qMakePair(tr("通信超时时间n毫秒"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "commResumeTime") {
                properties.append(qMakePair(tr("尝试恢复通讯间隔时间n毫秒"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "startAddrBit") {
                properties.append(qMakePair(tr("内存地址起始位为0"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "writeCoilFn") {
                properties.append(qMakePair(tr("写线圈功能码为15"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "writeRegFn") {
                properties.append(qMakePair(tr("写寄存器功能码为16"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "addr8") {
                properties.append(qMakePair(tr("8位逆序"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "addr16") {
                properties.append(qMakePair(tr("16位低字节在前高字节在后"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "addr32") {
                properties.append(qMakePair(tr("32位低字节在前高字节在后"), szListKeyVal.at(1)));
            } else if(szListKeyVal.at(0) == "addr64") {
                properties.append(qMakePair(tr("64位低字节在前高字节在后"), szListKeyVal.at(1)));
            }
        }
    }
}


///
/// \brief TCPIPModbus::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void TCPIPModbus::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
    QString szVal = getValue2ByValue1(tr("内存地址起始位为0"), m_properties);
    m_bStartAddrBit0 = (szVal.toLower() == "true") ? true : false;
}


