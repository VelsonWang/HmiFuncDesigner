#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QObject>
#include "../IDevicePlugin/IDevicePlugin.h"


class ModbusRTU : public QObject, IDevicePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DevicePluginInterface_iid FILE "ModbusRTU.json")
    Q_INTERFACES(IDevicePlugin)

public:
    ModbusRTU();

    // 获取设备默认属性
    void getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 获取设备默认属性数据类型
    void getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) Q_DECL_OVERRIDE;
    // 保存属性
    void writeProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 加载属性
    void readProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 设置设备属性
    void setDeviceProperty(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;

    // 获取设备描述信息
    QString getDeviceDescInfo() Q_DECL_OVERRIDE;

private:
    QVector<QPair<QString, QString>> m_properties; // 插件私有属性
    bool m_bStartAddrBit0 = true; // 内存地址起始位为0
};

#endif // MODBUSRTU_H
