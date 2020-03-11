#ifndef TCPIPMODBUS_H
#define TCPIPMODBUS_H

#include <QObject>
#include "../IDevicePlugin/IDevicePlugin.h"


class TCPIPModbus : public QObject, IDevicePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DevicePluginInterface_iid FILE "TCPIPModbus.json")
    Q_INTERFACES(IDevicePlugin)

public:
    TCPIPModbus();

    // 获取设备类型名称
    QString GetDeviceTypeName();
    // 获取设备支持的所有协议
    QStringList GetDeviceSupportProtocol() Q_DECL_OVERRIDE;
    // 获取设备支持的所有寄存器区
    QStringList GetDeviceSupportRegisterArea() Q_DECL_OVERRIDE;
    // 获取设备支持的所有数据类型
    QStringList GetDeviceSupportDataType() Q_DECL_OVERRIDE;
    // 获取寄存器区地址的下限和上限
    void GetRegisterAreaLimit(const QString &areaName,
                              quint32 &lowerLimit,
                              quint32 &upperLimit) Q_DECL_OVERRIDE;

    // 获取设备默认属性
    void getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 获取设备默认属性数据类型
    void getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) Q_DECL_OVERRIDE;
    // 保存属性为字符串
    QString devicePropertiesToString(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 从字符串加载属性
    void devicePropertiesFromString(const QString &szProperty, QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;

private:
    QVector<QPair<QString, QString>> m_properties; // 插件私有属性
};

#endif // TCPIPMODBUS_H
