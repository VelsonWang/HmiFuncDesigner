#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H

#include <QStringList>
#include <QPair>
#include <QVector>
#include <QXmlStreamWriter>

/*
* 注意本类不要派生自QObject
*/
class IDevicePlugin
{
public:
    virtual ~IDevicePlugin() {}

    // 位组包最大寄存器个数
    virtual int getBitMaxRegPacket() = 0;
    // 字组包最大寄存器个数
    virtual int getWordMaxRegPacket() = 0;
    // 通信失败重试次数
    virtual int getCommFailRetryTimes() = 0;
    // 通信超时时间
    virtual int getCommTimeout() = 0;
    // 通信间隔时间
    virtual int getCommIntervalTime() = 0;
    // 尝试恢复通讯间隔时间
    virtual int getCommResumeTime() = 0;


    // 获取设备默认属性
    virtual void getDefaultDeviceProperty(QVector<QPair<QString, QString>>& properties) = 0;
    // 获取设备默认属性数据类型
    virtual void getDefaultDevicePropertyDataType(QVector<QPair<QString, QString>>& properties_type) = 0;
    // 保存属性
    virtual void writeProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) = 0;
    // 加载属性
    virtual void readProperties(QString &szProperties, QVector<QPair<QString, QString>>& properties) = 0;
    // 设置设备属性
    virtual void setDeviceProperty(QVector<QPair<QString, QString>>& properties) = 0;

    // 获取设备描述信息
    virtual QString getDeviceDescInfo() = 0;

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

#define DevicePluginInterface_iid "HmiFuncDesigner.Device.PluginInterface"
Q_DECLARE_INTERFACE(IDevicePlugin, DevicePluginInterface_iid)

QT_END_NAMESPACE

#endif // IDEVICEPLUGIN_H
