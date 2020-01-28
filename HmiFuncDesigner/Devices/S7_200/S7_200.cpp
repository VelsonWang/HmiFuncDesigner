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
