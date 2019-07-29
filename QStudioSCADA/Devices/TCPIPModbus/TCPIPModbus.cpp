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
}
