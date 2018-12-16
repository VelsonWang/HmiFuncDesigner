#include "ModbusRTU.h"



ModbusRTU::ModbusRTU()
{

}


/*
* 获取设备类型名称
*/
QString ModbusRTU::GetDeviceTypeName()
{
    return QString(tr("串口设备"));
}

/*
* 获取设备支持的所有协议
*/
QStringList ModbusRTU::GetDeviceSupportProtocol()
{
    QStringList list;
    list<<"ModbusRTU";
    return list;
}

/*
* 获取设备支持的所有寄存器区
*/
QStringList ModbusRTU::GetDeviceSupportRegisterArea()
{
    QStringList list;
    list<<"DO线圈";
    list<<"DI离散输入寄存器";
    list<<"AO保持寄存器";
    list<<"AI输入寄存器";
    return list;
}

/*
* 获取设备支持的所有数据类型
*/
QStringList ModbusRTU::GetDeviceSupportDataType()
{
    QStringList list;
    list<<"Bit1开关量";
    list<<"Char8位有符号数";
    list<<"Byte8位无符号数";
    list<<"Short16位有符号数";
    list<<"Word16位无符号数";
    list<<"ASCII2个字符";
    list<<"Long32位有符号数";
    list<<"Dword32位无符号数";
    list<<"Float单精度浮点数";
    list<<"String字符串";
    list<<"Double双精度浮点数";
    list<<"BCD";
	//list<<"LongLong64位有符号数";
    //list<<"DwordDword64位无符号数";
    return list;
}


