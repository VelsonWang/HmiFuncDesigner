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
            if (pair.first == szVal1 && pair.second != "")
                return pair.second;
        }
        // 获取默认属性值
        QVector<QPair<QString, QString>> defaultProperties;
        this->getDefaultDeviceProperty(defaultProperties);
        for (int i = 0; i < defaultProperties.size(); ++i) {
            QPair<QString, QString> pair = defaultProperties[i];
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
