#include "DatabaseSetting.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

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



DatabaseSetting::DatabaseSetting()
    : dPtr_(new DatabaseSettingPrivate()) {

}

DatabaseSetting::~DatabaseSetting() {
    if(dPtr_ != Q_NULLPTR) {
        delete dPtr_;
    }
}

/**
 * @brief DatabaseSetting::load
 * @details 读取网络配置信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool DatabaseSetting::load(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_database_setting");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_database_setting")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        dPtr_->iAlarmSize_ = rec.value("alarm_size").toInt();
        dPtr_->bSpecialDB_ = rec.value("special_db").toInt() > 0 ? true : false;
        dPtr_->bAutoDelete_ = rec.value("auto_delete").toInt() > 0 ? true : false;
        dPtr_->szDBType_ = rec.value("db_type").toString();
        dPtr_->iDataKeepDays_ = rec.value("data_keep_days").toInt();
        dPtr_->szIPAddress_ = rec.value("ip_address").toString();
        dPtr_->bUseSD_ = rec.value("use_sd").toInt() > 0 ? true : false;
        dPtr_->szUser_ = rec.value("user").toString();
        dPtr_->iSavePeriod_ = rec.value("save_period").toInt();
        dPtr_->szPassword_ = rec.value("password").toString();
        dPtr_->iSendPeriod_ = rec.value("send_period").toInt();
        dPtr_->szDBName_ = rec.value("db_name").toString();
        dPtr_->iStartTime_ = rec.value("start_time").toInt();
        dPtr_->iPort_ = rec.value("port").toInt();
    }

    return ret;
}

/**
 * @brief DatabaseSetting::save
 * @details 保存网络配置信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool DatabaseSetting::save(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    query.prepare("update t_database_setting set "
                  "alarm_size = :alarm, special_db = :special, auto_delete = :aut, db_type = :dbt, "
                  "data_keep_days = :days, ip_address = :ip, use_sd = :sd, user = :user, "
                  "save_period = :save, password = :pass, send_period = :send, db_name = :db, "
                  "start_time = :start, port = :port where id = 1");

    query.bindValue(":alarm", dPtr_->iAlarmSize_);
    query.bindValue(":special", dPtr_->bSpecialDB_ ? 1 : 0);
    query.bindValue(":aut", dPtr_->bAutoDelete_ ? 1 : 0);
    query.bindValue(":dbt", dPtr_->szDBType_);
    query.bindValue(":days", dPtr_->iDataKeepDays_);
    query.bindValue(":ip", dPtr_->szIPAddress_);
    query.bindValue(":sd", dPtr_->bUseSD_ ? 1 : 0);
    query.bindValue(":user", dPtr_->szUser_);
    query.bindValue(":save", dPtr_->iSavePeriod_);
    query.bindValue(":pass", dPtr_->szPassword_);
    query.bindValue(":send", dPtr_->iSendPeriod_);
    query.bindValue(":db", dPtr_->szDBName_);
    query.bindValue(":start", dPtr_->iStartTime_);
    query.bindValue(":port", dPtr_->iPort_);

    bool ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_database_setting")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
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



