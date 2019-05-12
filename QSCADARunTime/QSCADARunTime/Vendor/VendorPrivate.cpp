#include "VendorPrivate.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


VendorPrivate::VendorPrivate()
{

}


/*
* 从文件读取配置数据
*/
bool ComDevicePrivate::LoadData(SaveFormat saveFormat, QString fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    const QJsonObject &json = loadDoc.object();

    m_sDeviceName = json["sDeviceName"].toString();
    m_iFrameLen = json["iFrameLen"].toInt();
    m_sProtocol = json["sProtocol"].toString();
    m_sLink = json["sLink"].toString();
    m_iStateVar = json["iStateVar"].toString().toInt();
    m_iFrameTimePeriod = json["iFrameTimePeriod"].toString().toInt();
    m_iCtrlVar = json["iCtrlVar"].toString().toInt();
    m_bDynamicOptimization = json["bDynamicOptimization"].toBool();
    m_iRemotePort = json["iRemotePort"].toString().toInt();
    m_sDeviceType = "COM";
    m_sPortNumber = json["sPortNumber"].toString();
    m_iBaudrate = json["iBaudrate"].toString().toInt();
    m_iDatabit = json["iDatabit"].toString().toInt();
    m_fStopbit = json["fStopbit"].toString().toFloat();
    m_sVerifybit = json["sVerifybit"].toString();
    m_iTimeout = json["iTimeout"].toString().toInt();

    loadFile.close();
    return true;
}



/*
* 从文件读取配置数据
*/
bool NetDevicePrivate::LoadData(SaveFormat saveFormat, QString fileName)
{
    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open load file.");
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    const QJsonObject &json = loadDoc.object();

    m_sDeviceName = json["sDeviceName"].toString();
    m_iFrameLen = json["iFrameLen"].toInt();
    m_sProtocol = json["sProtocol"].toString();
    m_sLink = json["sLink"].toString();
    m_iStateVar = json["iStateVar"].toString().toInt();
    m_iFrameTimePeriod = json["iFrameTimePeriod"].toString().toInt();
    m_iCtrlVar = json["iCtrlVar"].toString().toInt();
    m_bDynamicOptimization = json["bDynamicOptimization"].toBool();
    m_iRemotePort = json["iRemotePort"].toString().toInt();
    m_sDeviceType = "NET";

    m_sIpAddress = json["ipAddress"].toString();
    m_iPort = json["iPort"].toString().toInt();
    m_sIpAddress1 = json["ipAddress1"].toString();
    m_iPort1 = json["iPort1"].toString().toInt();

    loadFile.close();
    return true;
}
