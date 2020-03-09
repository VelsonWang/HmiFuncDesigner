#include "FX2N.h"



FX2N::FX2N() {

}


/**
 * @brief FX2N::GetDeviceTypeName
 * @details 获取设备类型名称
 * @return
 */
QString FX2N::GetDeviceTypeName() {
    return QString(tr("串口设备"));
}


/**
 * @brief FX2N::GetDeviceSupportProtocol
 * @details 获取设备支持的所有协议
 * @return
 */
QStringList FX2N::GetDeviceSupportProtocol() {
    QStringList list;
    list << "FxProtocol";
    return list;
}

/**
 * @brief FX2N::GetDeviceSupportRegisterArea
 * @details 获取设备支持的所有寄存器区
 * @return
 */
QStringList FX2N::GetDeviceSupportRegisterArea() {
    QStringList list;
    list << tr("X")
         << tr("Y")
         << tr("M")
         << tr("S")
         << tr("D")
         << tr("T")
         << tr("C16")
         << tr("C32");
    return list;
}

/**
 * @brief FX2N::GetDeviceSupportDataType
 * @details 获取设备支持的所有数据类型
 * @return
 */
QStringList FX2N::GetDeviceSupportDataType()
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
 * @brief FX2N::GetRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param areaName 寄存器区名称
 * @param lowerLimit 寄存器区地址下限
 * @param upperLimit 寄存器区地址上限
 */
void FX2N::GetRegisterAreaLimit(const QString &areaName,
                                     quint32 &lowerLimit,
                                     quint32 &upperLimit) {
    lowerLimit = 0;
    upperLimit = 0;
    if(areaName == tr("X")) {
        lowerLimit = 0;
        upperLimit = 177;
    } else if(areaName == tr("Y")) {
        lowerLimit = 0;
        upperLimit = 177;
    } else if(areaName == tr("M")) {
        lowerLimit = 0;
        upperLimit = 1023;
    } else if(areaName == tr("S")) {
        lowerLimit = 9;
        upperLimit = 999;
    } else if(areaName == tr("D")) {
        lowerLimit = 0;
        upperLimit = 0xFFFF;
    } else if(areaName == tr("T")) {
        lowerLimit = 0;
        upperLimit = 511;
    } else if(areaName == tr("C16")) {
        lowerLimit = 0;
        upperLimit = 199;
    } else if(areaName == tr("C32")) {
        lowerLimit = 200;
        upperLimit = 255;
    }
}



///
/// \brief FX2N::getDefaultDeviceProperty
/// \details 获取设备默认属性
/// \param properties
///
void FX2N::getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    properties.clear();
}


///
/// \brief FX2N::getDefaultDevicePropertyDataType
/// \details 获取设备默认属性数据类型
/// \param properties_type
///
void FX2N::getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type)
{
    properties_type.clear();
}


///
/// \brief FX2N::devicePropertiesToString
/// \details 保存属性为字符串
/// \param properties 属性
/// \return 属性字符串
///
QString FX2N::devicePropertiesToString(QVector<QPair<QString, QString>>& properties)
{
    Q_UNUSED(properties)
    return QString();
}


///
/// \brief FX2N::devicePropertiesFromString
/// \details 从字符串加载属性
/// \param szProperty 属性字符串
/// \param properties 属性
///
void FX2N::devicePropertiesFromString(const QString &szProperty, QVector<QPair<QString, QString>>& properties)
{
    Q_UNUSED(szProperty)
    Q_UNUSED(properties)
}


///
/// \brief FX2N::setDeviceProperty
/// \details 设置设备属性
/// \param properties
///
void FX2N::setDeviceProperty(QVector<QPair<QString, QString>>& properties)
{
    m_properties.clear();
    m_properties.append(properties);
}















