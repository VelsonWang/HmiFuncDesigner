#include "S7_200.h"



S7_200::S7_200() {

}


/**
 * @brief S7_200::GetDeviceTypeName
 * @details 获取设备类型名称
 * @return
 */
QString S7_200::GetDeviceTypeName() {
    return QString(tr("串口设备"));
}


/**
 * @brief S7_200::GetDeviceSupportProtocol
 * @details 获取设备支持的所有协议
 * @return
 */
QStringList S7_200::GetDeviceSupportProtocol() {
    QStringList list;
    list << "S7_200_PPI";
    return list;
}

/**
 * @brief S7_200::GetDeviceSupportRegisterArea
 * @details 获取设备支持的所有寄存器区
 * @return
 */
QStringList S7_200::GetDeviceSupportRegisterArea() {
    QStringList list;
    list << tr("V")
         << tr("I")
         << tr("Q")
         << tr("M")
         << tr("T")
         << tr("C")
         << tr("HC")
         << tr("AI")
         << tr("AQ");
    return list;
}

/**
 * @brief S7_200::GetDeviceSupportDataType
 * @details 获取设备支持的所有数据类型
 * @return
 */
QStringList S7_200::GetDeviceSupportDataType()
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
 * @brief S7_200::GetRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param areaName 寄存器区名称
 * @param lowerLimit 寄存器区地址下限
 * @param upperLimit 寄存器区地址上限
 */
void S7_200::GetRegisterAreaLimit(const QString &areaName,
                                     quint32 &lowerLimit,
                                     quint32 &upperLimit) {
    lowerLimit = 0;
    upperLimit = 0;
    if(areaName == tr("V")) {
        lowerLimit = 0;
        upperLimit = 10239;
    } else if(areaName == tr("I")) {
        lowerLimit = 0;
        upperLimit = 15;
    } else if(areaName == tr("Q")) {
        lowerLimit = 0;
        upperLimit = 15;
    } else if(areaName == tr("M")) {
        lowerLimit = 0;
        upperLimit = 31;
    } else if(areaName == tr("T")) {
        lowerLimit = 0;
        upperLimit = 255;
    } else if(areaName == tr("C")) {
        lowerLimit = 0;
        upperLimit = 255;
    } else if(areaName == tr("HC")) {
        lowerLimit = 0;
        upperLimit = 5;
    } else if(areaName == tr("AI")) {
        lowerLimit = 0;
        upperLimit = 62;
    } else if(areaName == tr("AQ")) {
        lowerLimit = 0;
        upperLimit = 62;
    }
}

///
/// \brief S7_200::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void S7_200::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
}


///
/// \brief S7_200::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void S7_200::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();

}


///
/// \brief S7_200::devicePropertiesToString
/// \details 保存属性为字符串
/// \param properties 属性
/// \return 属性字符串
///
QString S7_200::devicePropertiesToString(QVector<QPair<QString, QString>>& properties)
{
    Q_UNUSED(properties)
    return QString();
}


///
/// \brief S7_200::devicePropertiesFromString
/// \details 从字符串加载属性
/// \param szProperty 属性字符串
/// \param properties 属性
///
void S7_200::devicePropertiesFromString(const QString &szProperty, QVector<QPair<QString, QString>>& properties)
{
    Q_UNUSED(szProperty)
    Q_UNUSED(properties)
}


///
/// \brief S7_200::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void S7_200::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
}



