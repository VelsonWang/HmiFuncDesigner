#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QStringList>
#include <QList>
#include "../xmlobject.h"
#include "../sharedlibglobal.h"

class SHAREDLIB_EXPORT DeviceInfoObject
{
public:
    explicit DeviceInfoObject()
    {

    }

    DeviceInfoObject(const DeviceInfoObject &obj)
    {
        copyObject(obj);
    }

    ~DeviceInfoObject()
    {

    }

    DeviceInfoObject &operator=(const DeviceInfoObject &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const DeviceInfoObject &obj)
    {
        m_id = obj.m_id;
        m_deviceType = obj.m_deviceType;
        m_name = obj.m_name;
        m_deviceName = obj.m_deviceName;
        m_frameLen = obj.m_frameLen;
        m_protocol = obj.m_protocol;
        m_link = obj.m_link;
        m_stateVar = obj.m_stateVar;
        m_frameTimePeriod = obj.m_frameTimePeriod;
        m_ctrlVar = obj.m_ctrlVar;
        m_dynamicOptimization = obj.m_dynamicOptimization;
        m_remotePort = obj.m_remotePort;
        m_portParameters = obj.m_portParameters;
        m_properties = obj.m_properties;
    }

public:
    int m_id;
    QString m_deviceType; //COM, NET, BUS, OPC
    QString m_name; // 显示名称
    QString m_deviceName; // 设备插件名称
    int m_frameLen;
    QString m_protocol;
    QString m_link;
    int m_stateVar;
    int m_frameTimePeriod;
    int m_ctrlVar;
    bool m_dynamicOptimization;
    int m_remotePort;
    QString m_portParameters;
    QString m_properties; // 设备私有属性
};

class SHAREDLIB_EXPORT ComDevice : public DeviceInfoObject
{
public:
    explicit ComDevice() : DeviceInfoObject()
    {

    }

    ComDevice(const ComDevice &obj) : DeviceInfoObject(obj)
    {
        copyObject(obj);
    }

    ~ComDevice()
    {

    }

    ComDevice &operator=(const ComDevice &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const ComDevice &obj)
    {
        DeviceInfoObject::copyObject(obj);
        m_portNumber = obj.m_portNumber;
        m_baudrate = obj.m_baudrate;
        m_databit = obj.m_databit;
        m_stopbit = obj.m_stopbit;
        m_verifybit = obj.m_verifybit;
        m_timeout = obj.m_timeout;
    }

    void fromString(const QString &param)
    {
        QStringList listParam = param.split('|');
        if(listParam.count() == 6) {
            m_portNumber = listParam.at(0);
            m_baudrate = listParam.at(1).toInt();
            m_databit = listParam.at(2).toInt();
            m_stopbit = listParam.at(3).toFloat();
            m_verifybit = listParam.at(4);
            m_timeout = listParam.at(5).toInt();
        }
    }

    QString toString()
    {
        QStringList param;
        param.append(m_portNumber);
        param.append(QString::number(m_baudrate));
        param.append(QString::number(m_databit));
        param.append(QString::number(m_stopbit));
        param.append(m_verifybit);
        param.append(QString::number(m_timeout));
        return param.join("|");
    }

public:
    QString m_portNumber;
    int m_baudrate;
    int m_databit;
    float m_stopbit;
    QString m_verifybit;
    int m_timeout;
};


class SHAREDLIB_EXPORT NetDevice : public DeviceInfoObject
{
public:
    explicit NetDevice() : DeviceInfoObject()
    {

    }

    NetDevice(const NetDevice &obj) : DeviceInfoObject(obj)
    {
        copyObject(obj);
    }

    ~NetDevice()
    {

    }

    NetDevice &operator=(const NetDevice &obj)
    {
        copyObject(obj);
        return *this;
    }

    void copyObject(const NetDevice &obj)
    {
        DeviceInfoObject::copyObject(obj);
        m_ipAddress = obj.m_ipAddress;
        m_port = obj.m_port;
        m_ipAddress1 = obj.m_ipAddress1;
        m_port1 = obj.m_port1;
    }

    void fromString(const QString &param)
    {
        QStringList listParam = param.split('|');
        if(listParam.count() == 4) {
            m_ipAddress = listParam.at(0);
            m_port = listParam.at(1).toInt();
            m_ipAddress1 = listParam.at(2);
            m_port1 = listParam.at(3).toInt();
        }
    }

    QString toString()
    {
        QStringList param;
        param.append(m_ipAddress);
        param.append(QString::number(m_port));
        param.append(m_ipAddress1);
        param.append(QString::number(m_port1));
        return param.join("|");
    }

public:
    QString m_ipAddress;
    int m_port;
    QString m_ipAddress1;
    int m_port1;
};


class SHAREDLIB_EXPORT DeviceInfo
{
public:
    explicit DeviceInfo();
    ~DeviceInfo();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    DeviceInfoObject *newObject();
    DeviceInfoObject *getObjectByID(int id);
    DeviceInfoObject *getObjectByName(const QString &name);

    // 分配一个设备ID
    static int allocNewDeviceID();

public:
    QList<DeviceInfoObject *> m_deviceInfoObject;

private:
    static int m_nextDeviceID;
};

#endif // DEVICEINFO_H
