#include "DeviceInfo.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

DeviceInfo::DeviceInfo() {
    listDeviceInfoObject_.clear();
}

DeviceInfo::~DeviceInfo() {
    qDeleteAll(listDeviceInfoObject_);
    listDeviceInfoObject_.clear();
}

/**
 * @brief DeviceInfo::load
 * @details 读取设备参数信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool DeviceInfo::load(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_device_info");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_device_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listDeviceInfoObject_);
    listDeviceInfoObject_.clear();

    while (query.next()) {
        rec = query.record();
        DeviceInfoObject *pObj = new DeviceInfoObject();
        pObj->iID_ = rec.value("id").toInt();
        pObj->szDeviceType_ = rec.value("type").toString();
        pObj->szDeviceName_ = rec.value("name").toString();
        pObj->iFrameLen_ = rec.value("frame_length").toInt();
        pObj->szProtocol_ = rec.value("protocol").toString();
        pObj->szLink_ = rec.value("link").toString();
        pObj->iStateVar_ = rec.value("state_var").toInt();
        pObj->iFrameTimePeriod_ = rec.value("frame_time_period").toInt();
        pObj->iCtrlVar_ = rec.value("ctrl_var").toInt();
        pObj->bDynamicOptimization_ = rec.value("dynamic_optimization").toInt() > 0 ? true : false;
        pObj->iRemotePort_ = rec.value("remote_port").toInt();
        pObj->szPortParameters_ = rec.value("port_parameters").toString();
        listDeviceInfoObject_.append(pObj);
    }

    return ret;
}


/**
 * @brief DeviceInfo::save
 * @details 保存设备参数信息
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool DeviceInfo::save(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    pDB->db_.transaction();
    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        query.prepare("update t_device_info set type = :type, "
                      "name = :name, frame_length = :flen, protocol = :pro, link = :link, "
                      "state_var = :state, frame_time_period = :fperiod, ctrl_var = :ctrl, dynamic_optimization = :dyn, "
                      "remote_port = :remote, port_parameters = :param where id = :id");

        query.bindValue(":id", pObj->iID_);
        query.bindValue(":type", pObj->szDeviceType_);
        query.bindValue(":name", pObj->szDeviceName_);
        query.bindValue(":flen", pObj->iFrameLen_);
        query.bindValue(":pro", pObj->szProtocol_);
        query.bindValue(":link", pObj->szLink_);
        query.bindValue(":state", pObj->iStateVar_);
        query.bindValue(":fperiod", pObj->iFrameTimePeriod_);
        query.bindValue(":ctrl", pObj->iCtrlVar_);
        query.bindValue(":dyn", pObj->bDynamicOptimization_ ? 1 : 0);
        query.bindValue(":remote", pObj->iRemotePort_);
        query.bindValue(":param", pObj->szPortParameters_);

        ret = query.exec();
        if(!ret) {
            LogError(QString("update record: %1 failed! %2 ,error: %3!")
                     .arg("t_device_info")
                     .arg(query.lastQuery())
                     .arg(query.lastError().text()));
            pDB->db_.rollback();
        }
    }
    pDB->db_.commit();

    return ret;
}


bool DeviceInfo::insert(ProjectDataSQLiteDatabase *pDB,
                        DeviceInfoObject *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("insert into t_device_info (type, "
                  "name, frame_length, protocol, link, "
                  "state_var, frame_time_period, ctrl_var, dynamic_optimization, "
                  "remote_port, port_parameters) values (:type, "
                  ":name, :flen, :pro, :link, "
                  ":state, :fperiod, :ctrl, :dyn, "
                  ":remote, :param)");

    query.bindValue(":id", pObj->iID_);
    query.bindValue(":type", pObj->szDeviceType_);
    query.bindValue(":name", pObj->szDeviceName_);
    query.bindValue(":flen", pObj->iFrameLen_);
    query.bindValue(":pro", pObj->szProtocol_);
    query.bindValue(":link", pObj->szLink_);
    query.bindValue(":state", pObj->iStateVar_);
    query.bindValue(":fperiod", pObj->iFrameTimePeriod_);
    query.bindValue(":ctrl", pObj->iCtrlVar_);
    query.bindValue(":dyn", pObj->bDynamicOptimization_ ? 1 : 0);
    query.bindValue(":remote", pObj->iRemotePort_);
    query.bindValue(":param", pObj->szPortParameters_);

    ret = query.exec();
    if(!ret) {
        LogError(QString("insert record: %1 failed! %2 ,error: %3!")
                 .arg("t_device_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);
}

bool DeviceInfo::del(ProjectDataSQLiteDatabase *pDB,
                     DeviceInfoObject *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("delete from t_device_info where id = :id");
    query.bindValue(":id", pObj->iID_);

    ret = query.exec();
    if(!ret) {
        LogError(QString("delete record: %1 failed! %2 ,error: %3!")
                 .arg("t_device_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);

    return ret;
}

bool DeviceInfo::delAll(ProjectDataSQLiteDatabase *pDB) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("delete from t_device_info");
    ret = query.exec();
    if(!ret) {
        LogError(QString("delete record: %1 failed! %2 ,error: %3!")
                 .arg("t_device_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);

    return ret;
}


bool DeviceInfo::update(ProjectDataSQLiteDatabase *pDB,
                        DeviceInfoObject *pObj) {
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("update t_device_info set type = :type, "
                  "name = :name, frame_length = :flen, protocol = :pro, link = :link, "
                  "state_var = :state, frame_time_period = :fperiod, ctrl_var = :ctrl, dynamic_optimization = :dyn, "
                  "remote_port = :remote, port_parameters = :param where id = :id");

    query.bindValue(":id", pObj->iID_);
    query.bindValue(":type", pObj->szDeviceType_);
    query.bindValue(":name", pObj->szDeviceName_);
    query.bindValue(":flen", pObj->iFrameLen_);
    query.bindValue(":pro", pObj->szProtocol_);
    query.bindValue(":link", pObj->szLink_);
    query.bindValue(":state", pObj->iStateVar_);
    query.bindValue(":fperiod", pObj->iFrameTimePeriod_);
    query.bindValue(":ctrl", pObj->iCtrlVar_);
    query.bindValue(":dyn", pObj->bDynamicOptimization_ ? 1 : 0);
    query.bindValue(":remote", pObj->iRemotePort_);
    query.bindValue(":param", pObj->szPortParameters_);

    ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_device_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    this->load(pDB);

    return ret;
}

DeviceInfoObject *DeviceInfo::getDeviceInfoObjectByID(int id) {
    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        if(pObj->iID_ == id)
            return pObj;
    }
    return nullptr;
}

DeviceInfoObject *DeviceInfo::getDeviceInfoObjectByName(const QString &name) {
    for(int i=0; i<listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = listDeviceInfoObject_.at(i);
        if(pObj->szDeviceName_ == name)
            return pObj;
    }
    return nullptr;
}


int DeviceInfo::getLastInsertId(ProjectDataSQLiteDatabase *pDB) {
    return pDB->getLastInsertId("t_device_info");
}

