#include "NetSetting.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QFile>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

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
    : dPtr_(new NetSettingPrivate()) {

}

NetSetting::~NetSetting() {
    if(dPtr_ != Q_NULLPTR) {
        delete dPtr_;
    }
}

/**
 * @brief NetSetting::load
 * @details 读取网络配置信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool NetSetting::load(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_net_setting");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_net_setting")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    while (query.next()) {
        rec = query.record();
        dPtr_->bHotStandbyMode_ = rec.value("hot_standby_mode").toInt() > 0 ? true : false;
        dPtr_->bClientMode_ = rec.value("client_mode").toInt() > 0 ? true : false;
        dPtr_->bServerStation_ = rec.value("server_station").toInt() > 0 ? true : false;
        dPtr_->bClientStation_ = rec.value("client_station").toInt() > 0 ? true : false;
        dPtr_->szClientAddress_ = rec.value("client_address").toString();
        dPtr_->szServerAddress_ = rec.value("server_address").toString();
        dPtr_->iHeartbeatTime_ = rec.value("heartbeat_time").toInt();
        dPtr_->iDatabaseSyncTime_ = rec.value("database_sync_time").toInt();
    }

    return ret;
}

/**
 * @brief NetSetting::save
 * @details 保存网络配置信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool NetSetting::save(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    query.prepare("update t_net_setting set hot_standby_mode = :hot, "
                  "client_mode = :clientMode, server_station = :sStation, "
                  "client_station = :cStation, client_address = :cAddress, "
                  "server_address = :sAddress, heartbeat_time = :heartbeat, "
                  "database_sync_time = :dbSync where id = 1");

    query.bindValue(":hot", dPtr_->bHotStandbyMode_ ? 1 : 0);
    query.bindValue(":clientMode", dPtr_->bClientMode_ ? 1 : 0);
    query.bindValue(":sStation", dPtr_->bServerStation_ ? 1 : 0);
    query.bindValue(":cStation", dPtr_->bClientStation_ ? 1 : 0);
    query.bindValue(":cAddress", dPtr_->szClientAddress_);
    query.bindValue(":sAddress", dPtr_->szServerAddress_);
    query.bindValue(":heartbeat", dPtr_->iHeartbeatTime_);
    query.bindValue(":dbSync", dPtr_->iDatabaseSyncTime_);

    bool ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_net_setting")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
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




