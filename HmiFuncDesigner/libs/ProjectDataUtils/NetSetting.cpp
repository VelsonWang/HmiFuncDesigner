#include "NetSetting.h"

class NetSetting;

class NetSettingPrivate
{
public:
    NetSettingPrivate() { }

    bool bHotStandbyMode_;
    bool bClientMode_;
    bool bServerStation_;
    bool bClientStation_;
    QString szClientAddress_;
    QString szServerAddress_;
    int iHeartbeatTime_;
    int iDatabaseSyncTime_;
};



NetSetting::NetSetting()
    : dPtr_(new NetSettingPrivate())
{

}

NetSetting::~NetSetting()
{
    if(dPtr_ != Q_NULLPTR) {
        delete dPtr_;
    }
}


bool NetSetting::openFromXml(XMLObject *pXmlObj) {
    XMLObject *pNetSettingObj = pXmlObj->getCurrentChild("net_setting");
    dPtr_->bHotStandbyMode_ = pNetSettingObj->getProperty("hot") == "1";
    dPtr_->bClientMode_= pNetSettingObj->getProperty("clientMode") == "1";
    dPtr_->bServerStation_ = pNetSettingObj->getProperty("sStation") == "1";
    dPtr_->bClientStation_ = pNetSettingObj->getProperty("cStation") == "1";
    dPtr_->szClientAddress_ = pNetSettingObj->getProperty("cAddress");
    dPtr_->szServerAddress_ = pNetSettingObj->getProperty("sAddress");
    dPtr_->iHeartbeatTime_ = pNetSettingObj->getProperty("heartbeat").toInt();
    dPtr_->iDatabaseSyncTime_ = pNetSettingObj->getProperty("dbSync").toInt();
    return true;
}


bool NetSetting::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pNetSettingObj = new XMLObject(pXmlObj);
    pNetSettingObj->setTagName("net_setting");
    pNetSettingObj->setProperty("hot", dPtr_->bHotStandbyMode_ ? "1" : "0");
    pNetSettingObj->setProperty("clientMode", dPtr_->bClientMode_ ? "1" : "0");
    pNetSettingObj->setProperty("sStation", dPtr_->bServerStation_ ? "1" : "0");
    pNetSettingObj->setProperty("cStation", dPtr_->bClientStation_ ? "1" : "0");
    pNetSettingObj->setProperty("cAddress", dPtr_->szClientAddress_);
    pNetSettingObj->setProperty("sAddress", dPtr_->szServerAddress_);
    pNetSettingObj->setProperty("heartbeat", QString::number(dPtr_->iHeartbeatTime_));
    pNetSettingObj->setProperty("dbSync", QString::number(dPtr_->iDatabaseSyncTime_));
    return true;
}

bool NetSetting::isHotStandbyMode() {
    return dPtr_->bHotStandbyMode_;
}

void NetSetting::setHotStandbyMode(bool mode) {
    dPtr_->bHotStandbyMode_ = mode;
}

bool NetSetting::isClientMode() {
    return dPtr_->bClientMode_;
}

void NetSetting::setClientMode(bool mode) {
    dPtr_->bClientMode_ = mode;
}

bool NetSetting::isServerStation() {
    return dPtr_->bServerStation_;
}

void NetSetting::setServerStation(bool server) {
    dPtr_->bServerStation_ = server;
}

bool NetSetting::isClientStation() {
    return dPtr_->bClientStation_;
}

void NetSetting::setClientStation(bool client) {
    dPtr_->bClientStation_ = client;
}

QString NetSetting::getClientAddress() const {
    return dPtr_->szClientAddress_;
}

void NetSetting::setClientAddress(const QString &addr) {
    dPtr_->szClientAddress_ = addr;
}

QString NetSetting::getServerAddress() const {
    return dPtr_->szServerAddress_;
}

void NetSetting::setServerAddress(const QString &addr) {
    dPtr_->szServerAddress_ = addr;
}

int NetSetting::getHeartbeatTime() {
    return dPtr_->iHeartbeatTime_;
}

void NetSetting::setHeartbeatTime(int time) {
    dPtr_->iHeartbeatTime_ = time;
}

int NetSetting::getDatabaseSyncTime() {
    return dPtr_->iDatabaseSyncTime_;
}

void NetSetting::setDatabaseSyncTime(int time) {
    dPtr_->iDatabaseSyncTime_ = time;
}




