#include "DatabaseSetting.h"

class DatabaseSetting;

class DatabaseSettingPrivate
{
public:
    DatabaseSettingPrivate() { }

    int m_alarmSize;
    bool m_specialDB;
    bool m_autoDelete;
    QString m_dbType;
    int m_dataKeepDays;
    QString m_ipAddress;
    bool m_useSD;
    QString m_user;
    int m_savePeriod;
    QString m_password;
    int m_sendPeriod;
    QString m_dbName;
    int m_startTime;
    int m_port;
};



DatabaseSetting::DatabaseSetting() : m_dPtr(new DatabaseSettingPrivate())
{

}

DatabaseSetting::~DatabaseSetting()
{
    if(m_dPtr != NULL) {
        delete m_dPtr;
    }
}

bool DatabaseSetting::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pDBSettingObj = pXmlObj->getCurrentChild("database_setting");
    if(pDBSettingObj == NULL) {
        return false;
    }
    m_dPtr->m_alarmSize = pDBSettingObj->getProperty("alarm").toInt();
    m_dPtr->m_specialDB = pDBSettingObj->getProperty("special") == "1";
    m_dPtr->m_autoDelete = pDBSettingObj->getProperty("aut") == "1";
    m_dPtr->m_dbType = pDBSettingObj->getProperty("dbt");
    m_dPtr->m_dataKeepDays = pDBSettingObj->getProperty("days").toInt();
    m_dPtr->m_ipAddress = pDBSettingObj->getProperty("ip");
    m_dPtr->m_useSD = pDBSettingObj->getProperty("sd") == "1";
    m_dPtr->m_user = pDBSettingObj->getProperty("user");
    m_dPtr->m_savePeriod = pDBSettingObj->getProperty("save").toInt();
    m_dPtr->m_password = pDBSettingObj->getProperty("pass");
    m_dPtr->m_sendPeriod = pDBSettingObj->getProperty("send").toInt();
    m_dPtr->m_dbName = pDBSettingObj->getProperty("db");
    m_dPtr->m_startTime = pDBSettingObj->getProperty("start").toInt();
    m_dPtr->m_port = pDBSettingObj->getProperty("port").toInt();
    return true;
}


bool DatabaseSetting::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pDBSettingObj = new XMLObject(pXmlObj);
    pDBSettingObj->setTagName("database_setting");
    pDBSettingObj->setProperty("alarm", QString::number(m_dPtr->m_alarmSize));
    pDBSettingObj->setProperty("special", m_dPtr->m_specialDB ? "1" : "0");
    pDBSettingObj->setProperty("aut", m_dPtr->m_autoDelete ? "1" : "0");
    pDBSettingObj->setProperty("dbt", m_dPtr->m_dbType);
    pDBSettingObj->setProperty("days", QString::number(m_dPtr->m_dataKeepDays));
    pDBSettingObj->setProperty("ip", m_dPtr->m_ipAddress);
    pDBSettingObj->setProperty("sd", m_dPtr->m_useSD ? "1" : "0");
    pDBSettingObj->setProperty("user", m_dPtr->m_user);
    pDBSettingObj->setProperty("save", QString::number(m_dPtr->m_savePeriod));
    pDBSettingObj->setProperty("pass", m_dPtr->m_password);
    pDBSettingObj->setProperty("send", QString::number(m_dPtr->m_sendPeriod));
    pDBSettingObj->setProperty("db", m_dPtr->m_dbName);
    pDBSettingObj->setProperty("start", QString::number(m_dPtr->m_startTime));
    pDBSettingObj->setProperty("port", QString::number(m_dPtr->m_port));
    return true;
}

int DatabaseSetting::getAlarmSize()
{
    return m_dPtr->m_alarmSize;
}

void DatabaseSetting::setAlarmSize(int size)
{
    m_dPtr->m_alarmSize = size;
}

bool DatabaseSetting::isSpecialDB()
{
    return m_dPtr->m_specialDB;
}

void DatabaseSetting::setSpecialDB(bool special)
{
    m_dPtr->m_specialDB = special;
}


bool DatabaseSetting::isAutoDelete()
{
    return m_dPtr->m_autoDelete;
}

void DatabaseSetting::setAutoDelete(bool aut)
{
    m_dPtr->m_autoDelete = aut;
}


QString DatabaseSetting::getDBType() const
{
    return m_dPtr->m_dbType;
}

void DatabaseSetting::setDBType(const QString &type)
{
    m_dPtr->m_dbType = type;
}


int DatabaseSetting::getDataKeepDays()
{
    return m_dPtr->m_dataKeepDays;
}

void DatabaseSetting::setDataKeepDays(int days)
{
    m_dPtr->m_dataKeepDays = days;
}


QString DatabaseSetting::getIPAddress() const
{
    return m_dPtr->m_ipAddress;
}

void DatabaseSetting::setIPAddress(const QString &ip)
{
    m_dPtr->m_ipAddress = ip;
}


bool DatabaseSetting::isUseSD()
{
    return m_dPtr->m_useSD;
}

void DatabaseSetting::setUseSD(bool sd)
{
    m_dPtr->m_useSD = sd;
}


QString DatabaseSetting::getUserName() const
{
    return m_dPtr->m_user;
}

void DatabaseSetting::setUserName(const QString &name)
{
    m_dPtr->m_user = name;
}


int DatabaseSetting::getSavePeriod()
{
    return m_dPtr->m_savePeriod;
}

void DatabaseSetting::setSavePeriod(int period)
{
    m_dPtr->m_savePeriod = period;
}


QString DatabaseSetting::getPassword() const
{
    return m_dPtr->m_password;
}

void DatabaseSetting::setPassword(const QString &password)
{
    m_dPtr->m_password = password;
}


int DatabaseSetting::getSendPeriod()
{
    return m_dPtr->m_sendPeriod;
}

void DatabaseSetting::setSendPeriod(int period)
{
    m_dPtr->m_sendPeriod = period;
}


QString DatabaseSetting::getDBName() const
{
    return m_dPtr->m_dbName;
}

void DatabaseSetting::setDBName(const QString &name)
{
    m_dPtr->m_dbName = name;
}


int DatabaseSetting::getStartTime()
{
    return m_dPtr->m_startTime;
}

void DatabaseSetting::setStartTime(int time)
{
    m_dPtr->m_startTime = time;
}


int DatabaseSetting::getPort()
{
    return m_dPtr->m_port;
}

void DatabaseSetting::setPort(int port)
{
    m_dPtr->m_port = port;
}



