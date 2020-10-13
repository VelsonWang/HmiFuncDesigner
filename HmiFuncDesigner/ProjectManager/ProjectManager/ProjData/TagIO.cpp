#include "TagIO.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

TagIO::TagIO()
{
    listTagIODBItem_.clear();
}

TagIO::~TagIO()
{
    qDeleteAll(listTagIODBItem_);
    listTagIODBItem_.clear();
}

/**
 * @brief TagIO::load
 * @details 读取设备标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagIO::load(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->database());
    QSqlRecord rec;

    bool ret = query.exec("select * from t_tag_io");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_io")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listTagIODBItem_);
    listTagIODBItem_.clear();

    while (query.next()) {
        rec = query.record();
        TagIODBItem *pObj = new TagIODBItem();
        pObj->m_szTagID = rec.value("tagid").toString();
        pObj->m_szGroupName = rec.value("group_name").toString();
        pObj->m_szName = rec.value("tag_name").toString();
        pObj->m_szDescription = rec.value("description").toString();
        pObj->m_szDeviceName = rec.value("dev_name").toString();
        pObj->m_szDeviceAddr = rec.value("dev_addr").toString();
        pObj->m_szRegisterArea = rec.value("reg_area").toString();
        pObj->m_szRegisterAddr = rec.value("reg_addr").toString();
        pObj->m_szAddrOffset = rec.value("addr_offset").toString();
        pObj->m_szReadWriteType = rec.value("rw_type").toString();
        pObj->m_szDataType = rec.value("data_type").toString();
        pObj->m_szInitVal = rec.value("init_val").toString();
        pObj->m_szMinVal = rec.value("min_val").toString();
        pObj->m_szMaxVal = rec.value("max_val").toString();
        pObj->m_szScale = rec.value("scale").toString();
        pObj->m_szProjectConverter = rec.value("converter").toString();
        listTagIODBItem_.append(pObj);
    }

    return ret;
}


/**
 * @brief TagIO::save
 * @details 保存设备标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagIO::save(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->database());
    bool ret = false;

    pDB->beginTransaction();
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        query.prepare("update t_tag_io set group_name = :group, tag_name = :name, "
                      "description = :desc, dev_name = :devName, dev_addr = :devAddr, "
                      "reg_area = :regArea, reg_addr = :regAddr, addr_offset = :offset, "
                      "rw_type = :rw, data_type = :dtype, init_val = :init, "
                      "min_val = :min, max_val = :max , scale = :sca , converter = :conv "
                      "where tagid = :id");

        query.bindValue(":id", pObj->m_szTagID);
        query.bindValue(":group", pObj->m_szGroupName);
        query.bindValue(":name", pObj->m_szName);
        query.bindValue(":desc", pObj->m_szDescription);
        query.bindValue(":devName", pObj->m_szDeviceName);
        query.bindValue(":devAddr", pObj->m_szDeviceAddr);
        query.bindValue(":regArea", pObj->m_szRegisterArea);
        query.bindValue(":regAddr", pObj->m_szRegisterAddr);
        query.bindValue(":offset", pObj->m_szAddrOffset);
        query.bindValue(":rw", pObj->m_szReadWriteType);
        query.bindValue(":dtype", pObj->m_szDataType);
        query.bindValue(":init", pObj->m_szInitVal);
        query.bindValue(":min", pObj->m_szMinVal);
        query.bindValue(":max", pObj->m_szMaxVal);
        query.bindValue(":sca", pObj->m_szScale);
        query.bindValue(":conv", pObj->m_szProjectConverter);

        ret = query.exec();
        if(!ret) {
            LogError(QString("update record: %1 failed! %2 ,error: %3!")
                     .arg("t_tag_io")
                     .arg(query.lastQuery())
                     .arg(query.lastError().text()));
            pDB->rollbackTransaction();
        }
    }
    pDB->commitTransaction();

    return ret;
}


bool TagIO::insert(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj)
{
    QStringList keyList, valueList;

    keyList << "tagid" <<"group_name" <<  "tag_name" << "description" << "dev_name"
            << "dev_addr" << "reg_area" << "reg_addr" << "addr_offset"
            << "rw_type" << "data_type" << "init_val" << "min_val"
            << "max_val" << "scale"<< "converter";

    valueList << pObj->m_szTagID << pObj->m_szGroupName << pObj->m_szName << pObj->m_szDescription << pObj->m_szDeviceName
              << pObj->m_szDeviceAddr << pObj->m_szRegisterArea << pObj->m_szRegisterAddr << pObj->m_szAddrOffset
              << pObj->m_szReadWriteType << pObj->m_szDataType << pObj->m_szInitVal << pObj->m_szMinVal
              << pObj->m_szMaxVal << pObj->m_szScale << pObj->m_szProjectConverter;

    return pDB->insertRecord("t_tag_io", keyList, valueList);
}


bool TagIO::insert(ProjectDataSQLiteDatabase *pDB, QList<TagIODBItem *> &pObjs)
{
    bool ret = false;

    pDB->beginTransaction();
    for(int i=0; i<pObjs.count(); i++) {
        TagIODBItem *pObj = pObjs.at(i);
        ret = insert(pDB, pObj);
        if(!ret) {
            pDB->rollbackTransaction();
        }
    }
    pDB->commitTransaction();

    return ret;
}


bool TagIO::del(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj)
{
    return pDB->deleteRecord("t_tag_io", QString("tagid='%1'").arg(pObj->m_szTagID));
}

bool TagIO::del(ProjectDataSQLiteDatabase *pDB, const QString &id)
{
    return pDB->deleteRecord("t_tag_io", QString("tagid='%1'").arg(id));
}


bool TagIO::delAll(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->deleteRecord("t_tag_io");
}


bool TagIO::update(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj)
{
    QSqlQuery query(pDB->database());
    bool ret = false;

    query.prepare("update t_tag_io set group_name = :group, tag_name = :name, "
                  "description = :desc, dev_name = :devName, dev_addr = :devAddr, "
                  "reg_area = :regArea, reg_addr = :regAddr, addr_offset = :offset, "
                  "rw_type = :rw, data_type = :dtype, init_val = :init, "
                  "min_val = :min, max_val = :max , scale = :sca , converter = :conv "
                  "where tagid = :id");

    query.bindValue(":id", pObj->m_szTagID);
    query.bindValue(":group", pObj->m_szGroupName);
    query.bindValue(":name", pObj->m_szName);
    query.bindValue(":desc", pObj->m_szDescription);
    query.bindValue(":devName", pObj->m_szDeviceName);
    query.bindValue(":devAddr", pObj->m_szDeviceAddr);
    query.bindValue(":regArea", pObj->m_szRegisterArea);
    query.bindValue(":regAddr", pObj->m_szRegisterAddr);
    query.bindValue(":offset", pObj->m_szAddrOffset);
    query.bindValue(":rw", pObj->m_szReadWriteType);
    query.bindValue(":dtype", pObj->m_szDataType);
    query.bindValue(":init", pObj->m_szInitVal);
    query.bindValue(":min", pObj->m_szMinVal);
    query.bindValue(":max", pObj->m_szMaxVal);
    query.bindValue(":sca", pObj->m_szScale);
    query.bindValue(":conv", pObj->m_szProjectConverter);

    ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_io")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));

    }

    return ret;
}

TagIODBItem *TagIO::getTagIODBItemByID(const QString &id)
{
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        if(pObj->m_szTagID == id)
            return pObj;
    }
    return Q_NULLPTR;
}

TagIODBItem *TagIO::getTagIODBItemByName(const QString &name)
{
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        if(pObj->m_szName == name)
            return pObj;
    }
    return Q_NULLPTR;
}


int TagIO::getLastInsertId(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getLastInsertId("t_tag_io");
}


bool TagIO::saveTagIODBItem(ProjectDataSQLiteDatabase *pDB, TagIODBItem *pObj)
{
    bool ret = false;
    if(pDB->getRowCount("t_tag_io", QString("tagid='%1'").arg(pObj->m_szTagID))) {
        ret = update(pDB, pObj);
    } else {
        ret = insert(pDB, pObj);
    }

    return ret;
}


//////////////////////////////////////////////////////////////////////////////////////////


TagIOGroup::TagIOGroup()
{
    listTagIOGroupDBItem_.clear();
}

TagIOGroup::~TagIOGroup()
{
    qDeleteAll(listTagIOGroupDBItem_);
    listTagIOGroupDBItem_.clear();
}

/**
 * @brief TagIOGroup::load
 * @details 读取设备标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagIOGroup::load(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->database());
    QSqlRecord rec;

    bool ret = query.exec("select * from t_io_group_info");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_io_group_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listTagIOGroupDBItem_);
    listTagIOGroupDBItem_.clear();

    while (query.next()) {
        rec = query.record();
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = rec.value("id").toInt();
        pObj->m_szGroupName = rec.value("group_name").toString();
        pObj->m_szShowName = rec.value("show_name").toString();
        listTagIOGroupDBItem_.append(pObj);
    }

    return ret;
}


/**
 * @brief TagIOGroup::save
 * @details 保存设备标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagIOGroup::save(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->database());
    bool ret = false;

    pDB->beginTransaction();
    for(int i=0; i<listTagIOGroupDBItem_.count(); i++) {
        TagIOGroupDBItem *pObj = listTagIOGroupDBItem_.at(i);
        query.prepare("update t_io_group_info set group_name = :group, show_name = :name "
                      "where id = :id");

        query.bindValue(":id", pObj->m_id);
        query.bindValue(":group", pObj->m_szGroupName);
        query.bindValue(":name", pObj->m_szShowName);
        ret = query.exec();
        if(!ret) {
            LogError(QString("update record: %1 failed! %2 ,error: %3!")
                     .arg("t_io_group_info")
                     .arg(query.lastQuery())
                     .arg(query.lastError().text()));
            pDB->rollbackTransaction();
        }
    }
    pDB->commitTransaction();

    return ret;
}


bool TagIOGroup::insert(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj)
{
    QStringList keyList, valueList;

    keyList << "group_name" <<"show_name";
    valueList << pObj->m_szGroupName << pObj->m_szShowName;

    return pDB->insertRecord("t_io_group_info", keyList, valueList);
}


bool TagIOGroup::del(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj)
{
    return pDB->deleteRecord("t_io_group_info", QString("id=%1").arg(pObj->m_id));
}


bool TagIOGroup::update(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj)
{
    QSqlQuery query(pDB->database());
    bool ret = false;

    query.prepare("update t_io_group_info set group_name = :group, show_name = :name "
                  "where id = :id");

    query.bindValue(":id", pObj->m_id);
    query.bindValue(":group", pObj->m_szGroupName);
    query.bindValue(":name", pObj->m_szShowName);
    ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_io_group_info")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
}


int TagIOGroup::getLastInsertId(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getLastInsertId("t_io_group_info");
}


bool TagIOGroup::saveTagTmpDBItem(ProjectDataSQLiteDatabase *pDB, TagIOGroupDBItem *pObj)
{
    bool ret = false;
    if(pDB->getRowCount("t_io_group_info", QString("id=%1").arg(pObj->m_id))) {
        ret = update(pDB, pObj);
    } else {
        ret = insert(pDB, pObj);
    }

    return ret;
}


QString TagIOGroup::getGroupNameByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name)
{
    bool ret = false;
    QString rzRetValue = "";
    ret = pDB->getRecord("t_io_group_info", "group_name", rzRetValue, QString("show_name='%1'").arg(name));
    if(ret) return rzRetValue;

    return QString();
}

TagIOGroupDBItem *TagIOGroup::getGroupObjByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name)
{
    bool ret = false;
    QStringList keyList, valueList;
    keyList << "id" << "group_name";
    ret = pDB->getRecord("t_io_group_info", keyList, valueList, QString("show_name='%1'").arg(name));
    if(ret) {
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = valueList.at(0).toInt();
        pObj->m_szGroupName = valueList.at(1);
        pObj->m_szShowName = name;
        return pObj;
    }

    return Q_NULLPTR;
}

int TagIOGroup::getGroupCount(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getRowCount("t_io_group_info");
}

int TagIOGroup::getGroupCountByShowName(ProjectDataSQLiteDatabase *pDB, const QString &name)
{
    return pDB->getRowCount("t_io_group_info", QString("show_name='%1'").arg(name));
}
