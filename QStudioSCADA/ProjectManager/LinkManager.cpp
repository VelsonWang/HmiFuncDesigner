#include "LinkManager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

LinkManager::LinkManager(QString ProjectPath) :
    m_ProjectPath(ProjectPath)
{

}

void LinkManager::AddDevice(DeviceBase *dev)
{
    DevList.append(dev);
}

void LinkManager::DelDevice(DeviceBase *dev)
{
    DevList.removeOne(dev);
}

void LinkManager::load(const QJsonObject &json)
{
    DevList.clear();
    QJsonArray DeviceArray = json["DeviceArray"].toArray();
    for (int i = 0; i < DeviceArray.size(); ++i) {
        DeviceBase *dev = new DeviceBase();
        QJsonObject jsonObj = DeviceArray[i].toObject();
        //jsonObj["path"] = "";
        dev->m_sProtocol = jsonObj["prog"].toString();
        dev->m_sDeviceType = jsonObj["type"].toString();
        dev->m_sDeviceName = jsonObj["name"].toString();
        //jsonObj["bstop"] = false;
        dev->m_sLink = jsonObj["rdy"].toString();
        DevList.append(dev);
    }
}

void LinkManager::save(QJsonObject &json)
{
    QJsonArray DeviceArray;
    for(int i = 0; i < DevList.count(); i++)
    {
        QJsonObject jsonObj;
        jsonObj["ramid"] = i;
        jsonObj["path"] = "";
        jsonObj["prog"] = DevList.at(i)->m_sProtocol;
        jsonObj["type"] = DevList.at(i)->m_sDeviceType; //COM,NET,BUS,OPC
        jsonObj["name"] = DevList.at(i)->m_sDeviceName;
        jsonObj["bstop"] = false;
        jsonObj["rdy"] = DevList.at(i)->m_sLink;
        DeviceArray.append(jsonObj);
    }
    json["DeviceArray"] = DeviceArray;
}

bool LinkManager::loadFromFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/LinkList.odb";
    QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << QString("Couldn't open load file: %1.").arg(file);
        return false;
    }
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(saveData) : QJsonDocument::fromBinaryData(saveData));
    load(loadDoc.object());
    loadFile.close();
    return true;
}

bool LinkManager::saveToFile(SaveFormat saveFormat)
{
    QString file = m_ProjectPath + "/LinkList.odb";
    QFile saveFile(file);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << QString("Couldn't open save file: %1.").arg(file);
        return false;
    }
    QJsonObject obj;
    save(obj);
    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
    return true;
}



















