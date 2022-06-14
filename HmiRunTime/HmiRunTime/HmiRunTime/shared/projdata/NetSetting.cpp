#include "NetSetting.h"

class NetSetting;

class NetSettingPrivate
{
public:
    NetSettingPrivate() { }

    bool m_hotStandbyMode;
    bool m_clientMode;
    bool m_serverStation;
    bool m_clientStation;
    QString m_clientAddress;
    QString m_serverAddress;
    int m_heartbeatTime;
    int m_databaseSyncTime;
};



NetSetting::NetSetting()
    : m_dPtr(new NetSettingPrivate())
{

}

NetSetting::~NetSetting()
{
    if(m_dPtr != NULL) {
        delete m_dPtr;
    }
}


bool NetSetting::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pNetSettingObj = pXmlObj->getCurrentChild("net_setting");
    if(pNetSettingObj == NULL) {
        return false;
    }
    m_dPtr->m_hotStandbyMode = pNetSettingObj->getProperty("hot") == "1";
    m_dPtr->m_clientMode = pNetSettingObj->getProperty("clientMode") == "1";
    m_dPtr->m_serverStation = pNetSettingObj->getProperty("sStation") == "1";
    m_dPtr->m_clientStation = pNetSettingObj->getProperty("cStation") == "1";
    m_dPtr->m_clientAddress = pNetSettingObj->getProperty("cAddress");
    m_dPtr->m_serverAddress = pNetSettingObj->getProperty("sAddress");
    m_dPtr->m_heartbeatTime = pNetSettingObj->getProperty("heartbeat").toInt();
    m_dPtr->m_databaseSyncTime = pNetSettingObj->getProperty("dbSync").toInt();
    return true;
}


bool NetSetting::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pNetSettingObj = new XMLObject(pXmlObj);
    pNetSettingObj->setTagName("net_setting");
    pNetSettingObj->setProperty("hot", m_dPtr->m_hotStandbyMode ? "1" : "0");
    pNetSettingObj->setProperty("clientMode", m_dPtr->m_clientMode ? "1" : "0");
    pNetSettingObj->setProperty("sStation", m_dPtr->m_serverStation ? "1" : "0");
    pNetSettingObj->setProperty("cStation", m_dPtr->m_clientStation ? "1" : "0");
    pNetSettingObj->setProperty("cAddress", m_dPtr->m_clientAddress);
    pNetSettingObj->setProperty("sAddress", m_dPtr->m_serverAddress);
    pNetSettingObj->setProperty("heartbeat", QString::number(m_dPtr->m_heartbeatTime));
    pNetSettingObj->setProperty("dbSync", QString::number(m_dPtr->m_databaseSyncTime));
    return true;
}

bool NetSetting::isHotStandbyMode()
{
    return m_dPtr->m_hotStandbyMode;
}

void NetSetting::setHotStandbyMode(bool mode)
{
    m_dPtr->m_hotStandbyMode = mode;
}

bool NetSetting::isClientMode()
{
    return m_dPtr->m_clientMode;
}

void NetSetting::setClientMode(bool mode)
{
    m_dPtr->m_clientMode = mode;
}

bool NetSetting::isServerStation()
{
    return m_dPtr->m_serverStation;
}

void NetSetting::setServerStation(bool server)
{
    m_dPtr->m_serverStation = server;
}

bool NetSetting::isClientStation()
{
    return m_dPtr->m_clientStation;
}

void NetSetting::setClientStation(bool client)
{
    m_dPtr->m_clientStation = client;
}

QString NetSetting::getClientAddress() const
{
    return m_dPtr->m_clientAddress;
}

void NetSetting::setClientAddress(const QString &addr)
{
    m_dPtr->m_clientAddress = addr;
}

QString NetSetting::getServerAddress() const
{
    return m_dPtr->m_serverAddress;
}

void NetSetting::setServerAddress(const QString &addr)
{
    m_dPtr->m_serverAddress = addr;
}

int NetSetting::getHeartbeatTime()
{
    return m_dPtr->m_heartbeatTime;
}

void NetSetting::setHeartbeatTime(int time)
{
    m_dPtr->m_heartbeatTime = time;
}

int NetSetting::getDatabaseSyncTime()
{
    return m_dPtr->m_databaseSyncTime;
}

void NetSetting::setDatabaseSyncTime(int time)
{
    m_dPtr->m_databaseSyncTime = time;
}




