#ifndef FX2N_H
#define FX2N_H

#include <QObject>
#include "../IDevicePlugin/IDevicePlugin.h"


class FX2N : public QObject, IDevicePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DevicePluginInterface_iid FILE "FX2N.json")
    Q_INTERFACES(IDevicePlugin)

public:
    FX2N();

    // 位组包最大寄存器个数
    int getBitMaxRegPacket() Q_DECL_OVERRIDE;
    // 字组包最大寄存器个数
    int getWordMaxRegPacket() Q_DECL_OVERRIDE;
    // 通信失败重试次数
    int getCommFailRetryTimes() Q_DECL_OVERRIDE;
    // 通信超时时间
    int getCommTimeout() Q_DECL_OVERRIDE;
    // 通信间隔时间
    int getCommIntervalTime() Q_DECL_OVERRIDE;
    // 尝试恢复通讯间隔时间
    int getCommResumeTime() Q_DECL_OVERRIDE;

    // 获取设备默认属性
    void getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 获取设备默认属性数据类型
    void getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) Q_DECL_OVERRIDE;
    // 保存属性至xml节点
    void writeAsXml(QXmlStreamWriter &xml, QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 从xml节点加载属性
    void readFromXml(QXmlStreamReader &xml, QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;
    // 设置设备属性
    void setDeviceProperty(QVector<QPair<QString, QString>>& properties) Q_DECL_OVERRIDE;

    // 获取设备支持的所有协议
    QStringList getDeviceSupportProtocol() Q_DECL_OVERRIDE;
    // 获取设备支持的所有寄存器区
    QStringList getDeviceSupportRegisterArea() Q_DECL_OVERRIDE;
    // 获取设备支持的所有数据类型
    QStringList getDeviceSupportDataType(const QString &szAreaName) Q_DECL_OVERRIDE;
    // 获取指定数据类型和读写属性设备支持的寄存器区
    QStringList getDeviceSupportRegisterArea(const QString &szDataType, bool bWriteable) Q_DECL_OVERRIDE;
    // 获取寄存器区地址的下限和上限
    void getRegisterAreaLimit(const QString &szAreaName,
                              quint32 &iLowerLimit,
                              quint32 &iUpperLimit) Q_DECL_OVERRIDE;
    // 获取地址类型别名
    QString getAddressTypeAlias(const QString &szDataType,
                                const QString &szAreaName,
                                const QString &szAddrOffset,
                                const QString &szAreaName2,
                                const QString &szAddrOffset2) Q_DECL_OVERRIDE;
    // 获取地址类型别名
    QString getAddressTypeAlias(const QString &szAreaName) Q_DECL_OVERRIDE;
    // 获取指定地址类型别名的地址类型
    QString getAddressType(const QString &szAddressTypeAlias) Q_DECL_OVERRIDE;
    // 获取寄存器地址映射列表
    void getAutoAddrMapItemList(QList<PAutoAddrMapItem> &listAutoAddrMapItem) Q_DECL_OVERRIDE;

    // 获取设备支持的地址类型所有子寄存器区
    QStringList getDeviceSupportRegisterAreaSubArea(const QString &szAreaName) Q_DECL_OVERRIDE;

private:
    QVector<QPair<QString, QString>> m_properties; // 插件私有属性

};

#endif // FX2N_H
