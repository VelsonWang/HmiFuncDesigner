#include "ModbusASCII.h"

ModbusASCII::ModbusASCII()
{

}

/*
* 获取设备类型名称
*/
QString ModbusASCII::GetDeviceTypeName()
{
    return QString(tr("串口设备"));
}

/*
* 获取设备支持的所有协议
*/
QStringList ModbusASCII::GetDeviceSupportProtocol()
{
    QStringList list;
    list<<"ModbusASCII";
    return list;
}

/*
* 获取设备支持的所有寄存器区
*/
QStringList ModbusASCII::GetDeviceSupportRegisterArea()
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
QStringList ModbusASCII::GetDeviceSupportDataType()
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
    //<< tr("LongLong64位有符号数");
    //<< tr("DwordDword64位无符号数");
    return list;
}


/**
 * @brief ModbusASCII::GetRegisterAreaLimit
 * @details 获取寄存器区地址的下限和上限
 * @param areaName 寄存器区名称
 * @param lowerLimit 寄存器区地址下限
 * @param upperLimit 寄存器区地址上限
 */
void ModbusASCII::GetRegisterAreaLimit(const QString &areaName,
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

