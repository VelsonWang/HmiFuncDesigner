#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H

#include <QStringList>
#include <QPair>
#include <QVector>
#include <QMap>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>

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
    // 生成块读变量
    virtual bool buildBlockReadTags(const QString &xmlDevTags, const QString &properties, QString &xmlDevBlockReadTags, QVector<QPair<QString, QString>>& idToBlockId) = 0;

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

    int tagLength(const QString &type) {
        if(type == "bool" || type == "int8" || type == "uint8" || type == "bcd8"){
            return 1;
        } else if(type == "int16" || type == "uint16" || type == "bcd16") {
            return 2;
        } else if(type == "int32" || type == "uint32" || type == "float32" || type == "bcd32") {
            return 4;
        } else if(type == "double" || type == "int64" || type == "uint64") {
            return 8;
        }
        return 0;
    }
};


QT_BEGIN_NAMESPACE

#define DevicePluginInterface_iid "HmiFuncDesigner.Device.PluginInterface"
Q_DECLARE_INTERFACE(IDevicePlugin, DevicePluginInterface_iid)

QT_END_NAMESPACE

#endif // IDEVICEPLUGIN_H
