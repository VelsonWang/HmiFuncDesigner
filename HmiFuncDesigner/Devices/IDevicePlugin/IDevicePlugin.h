#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H

#include <QStringList>
#include <QVector>
#include <QPair>

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


    // 获取设备默认属性
    virtual void getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) = 0;
    // 获取设备默认属性数据类型
    virtual void getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) = 0;
    // 保存属性为字符串
    virtual QString devicePropertiesToString(QVector<QPair<QString, QString>>& properties) = 0;
    // 从字符串加载属性
    virtual void devicePropertiesFromString(const QString &szProperty, QVector<QPair<QString, QString>>& properties) = 0;

    QString getValue2ByValue1(const QString &szVal1, QVector<QPair<QString, QString>>& properties) {
        for (int i = 0; i < properties.size(); ++i) {
            QPair<QString, QString> pair = properties[i];
            if (pair.first == szVal1)
                return pair.second;
        }
        return "";
    }
};


QT_BEGIN_NAMESPACE

#define DevicePluginInterface_iid "QStudioSCADA.Device.PluginInterface"
Q_DECLARE_INTERFACE(IDevicePlugin, DevicePluginInterface_iid)

QT_END_NAMESPACE

#endif // IDEVICEPLUGIN_H
