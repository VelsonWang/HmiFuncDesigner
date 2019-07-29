
#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H


#include <QStringList>


/*
* 注意本类不要派生自QObject
*/
class IDevicePlugin
{
public:
    virtual ~IDevicePlugin() {}

    // 获取设备类型名称
    virtual QString GetDeviceTypeName() = 0;
    // 获取设备支持的所有协议
    virtual QStringList GetDeviceSupportProtocol() = 0;
    // 获取设备支持的所有寄存器区
    virtual QStringList GetDeviceSupportRegisterArea() = 0;
    // 获取设备支持的所有数据类型
    virtual QStringList GetDeviceSupportDataType() = 0;
    // 获取寄存器区地址的下限和上限
    virtual void GetRegisterAreaLimit(const QString &areaName,
                                      quint32 &lowerLimit,
                                      quint32 &upperLimit) = 0;
};


QT_BEGIN_NAMESPACE

#define DevicePluginInterface_iid "QStudioSCADA.Device.PluginInterface"
Q_DECLARE_INTERFACE(IDevicePlugin, DevicePluginInterface_iid)

QT_END_NAMESPACE

#endif // IDEVICEPLUGIN_H
