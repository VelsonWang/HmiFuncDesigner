#include "DatabaseSetting.h"

class DatabaseSetting;

class DatabaseSettingPrivate
{
public:
    DatabaseSettingPrivate() { }

    int iAlarmSize_;
    bool bSpecialDB_;
    bool bAutoDelete_;
    QString szDBType_;
    int iDataKeepDays_;
    QString szIPAddress_;
    bool bUseSD_;
    QString szUser_;
    int iSavePeriod_;
    QString szPassword_;
    int iSendPeriod_;
    QString szDBName_;
    int iStartTime_;
    int iPort_;
};



DatabaseSetting::DatabaseSetting() : dPtr_(new DatabaseSettingPrivate()) {

}

DatabaseSetting::~DatabaseSetting() {
    if(dPtr_ != Q_NULLPTR) {
        delete dPtr_;
    }
}

bool DatabaseSetting::openFromXml(XMLObject *pXmlObj) {
    XMLObject *pDBSettingObj = pXmlObj->getCurrentChild("database_setting");
    dPtr_->iAlarmSize_ = pDBSettingObj->getProperty("alarm").toInt();
    dPtr_->bSpecialDB_ = pDBSettingObj->getProperty("special") == "1";
    dPtr_->bAutoDelete_ = pDBSettingObj->getProperty("aut") == "1";
    dPtr_->szDBType_ = pDBSettingObj->getProperty("dbt");
    dPtr_->iDataKeepDays_ = pDBSettingObj->getProperty("days").toInt();
    dPtr_->szIPAddress_ = pDBSettingObj->getProperty("ip");
    dPtr_->bUseSD_ = pDBSettingObj->getProperty("sd") == "1";
    dPtr_->szUser_ = pDBSettingObj->getProperty("user");
    dPtr_->iSavePeriod_ = pDBSettingObj->getProperty("save").toInt();
    dPtr_->szPassword_ = pDBSettingObj->getProperty("pass");
    dPtr_->iSendPeriod_ = pDBSettingObj->getProperty("send").toInt();
    dPtr_->szDBName_ = pDBSettingObj->getProperty("db");
    dPtr_->iStartTime_ = pDBSettingObj->getProperty("start").toInt();
    dPtr_->iPort_ = pDBSettingObj->getProperty("port").toInt();
    return true;
}


bool DatabaseSetting::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pDBSettingObj = new XMLObject(pXmlObj);
    pDBSettingObj->setTagName("database_setting");
    pDBSettingObj->setProperty("alarm", QString::number(dPtr_->iAlarmSize_));
    pDBSettingObj->setProperty("special", dPtr_->bSpecialDB_ ? "1" : "0");
    pDBSettingObj->setProperty("aut", dPtr_->bAutoDelete_ ? "1" : "0");
    pDBSettingObj->setProperty("dbt", dPtr_->szDBType_);
    pDBSettingObj->setProperty("days", QString::number(dPtr_->iDataKeepDays_));
    pDBSettingObj->setProperty("ip", dPtr_->szIPAddress_);
    pDBSettingObj->setProperty("sd", dPtr_->bUseSD_ ? "1" : "0");
    pDBSettingObj->setProperty("user", dPtr_->szUser_);
    pDBSettingObj->setProperty("save", QString::number(dPtr_->iSavePeriod_));
    pDBSettingObj->setProperty("pass", dPtr_->szPassword_);
    pDBSettingObj->setProperty("send", QString::number(dPtr_->iSendPeriod_));
    pDBSettingObj->setProperty("db", dPtr_->szDBName_);
    pDBSettingObj->setProperty("start", QString::number(dPtr_->iStartTime_));
    pDBSettingObj->setProperty("port", QString::number(dPtr_->iPort_));
    return true;
}

int DatabaseSetting::getAlarmSize() {
    return dPtr_->iAlarmSize_;
}

void DatabaseSetting::setAlarmSize(int size) {
    dPtr_->iAlarmSize_ = size;
}

bool DatabaseSetting::isSpecialDB() {
    return dPtr_->bSpecialDB_;
}

void DatabaseSetting::setSpecialDB(bool special) {
    dPtr_->bSpecialDB_ = special;
}


bool DatabaseSetting::isAutoDelete() {
    return dPtr_->bAutoDelete_;
}

void DatabaseSetting::setAutoDelete(bool aut) {
    dPtr_->bAutoDelete_ = aut;
}


QString DatabaseSetting::getDBType() const {
    return dPtr_->szDBType_;
}

void DatabaseSetting::setDBType(const QString &type) {
    dPtr_->szDBType_ = type;
}


int DatabaseSetting::getDataKeepDays() {
    return dPtr_->iDataKeepDays_;
}

void DatabaseSetting::setDataKeepDays(int days) {
    dPtr_->iDataKeepDays_ = days;
}


QString DatabaseSetting::getIPAddress() const {
    return dPtr_->szIPAddress_;
}

void DatabaseSetting::setIPAddress(const QString &ip) {
    dPtr_->szIPAddress_ = ip;
}


bool DatabaseSetting::isUseSD() {
    return dPtr_->bUseSD_;
}

void DatabaseSetting::setUseSD(bool sd) {
    dPtr_->bUseSD_ = sd;
}


QString DatabaseSetting::getUserName() const {
    return dPtr_->szUser_;
}

void DatabaseSetting::setUserName(const QString &name) {
    dPtr_->szUser_ = name;
}


int DatabaseSetting::getSavePeriod() {
    return dPtr_->iSavePeriod_;
}

void DatabaseSetting::setSavePeriod(int period) {
    dPtr_->iSavePeriod_ = period;
}


QString DatabaseSetting::getPassword() const {
    return dPtr_->szPassword_;
}

void DatabaseSetting::setPassword(const QString &password) {
    dPtr_->szPassword_ = password;
}


int DatabaseSetting::getSendPeriod() {
    return dPtr_->iSendPeriod_;
}

void DatabaseSetting::setSendPeriod(int period) {
    dPtr_->iSendPeriod_ = period;
}


QString DatabaseSetting::getDBName() const {
    return dPtr_->szDBName_;
}

void DatabaseSetting::setDBName(const QString &name) {
    dPtr_->szDBName_ = name;
}


int DatabaseSetting::getStartTime() {
    return dPtr_->iStartTime_;
}

void DatabaseSetting::setStartTime(int time) {
    dPtr_->iStartTime_ = time;
}


int DatabaseSetting::getPort() {
    return dPtr_->iPort_;
}

void DatabaseSetting::setPort(int port) {
    dPtr_->iPort_ = port;
}



