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


bool TagIOGroup::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(listTagIOGroupDBItem_);
    listTagIOGroupDBItem_.clear();
    XMLObject *pTagIOGroupsObj = pXmlObj->getCurrentChild("tag_io_groups");
    if(pTagIOGroupsObj == Q_NULLPTR) return false;
    QList<XMLObject* > listTagIOGroupsObj = pTagIOGroupsObj->getCurrentChildren("tag_io_group");
    foreach(XMLObject* pTagIOGroupObj, listTagIOGroupsObj) {
        TagIOGroupDBItem *pObj = new TagIOGroupDBItem();
        pObj->m_id = pTagIOGroupObj->getProperty("id").toInt();
        pObj->m_szGroupName = pTagIOGroupObj->getProperty("group");
        pObj->m_szShowName = pTagIOGroupObj->getProperty("name");
        listTagIOGroupDBItem_.append(pObj);
    }
    return true;
}


bool TagIOGroup::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pTagIOGroupsObj = new XMLObject(pXmlObj);
    pTagIOGroupsObj->setTagName("tag_io_groups");
    for(int i=0; i<listTagIOGroupDBItem_.count(); i++) {
        TagIOGroupDBItem *pObj = listTagIOGroupDBItem_.at(i);
        XMLObject *pTagIOGroupObj = new XMLObject(pTagIOGroupsObj);
        pTagIOGroupObj->setTagName("tag_io_group");
        pTagIOGroupObj->setProperty("id", QString::number(pObj->m_id));
        pTagIOGroupObj->setProperty("group", pObj->m_szGroupName);
        pTagIOGroupObj->setProperty("name", pObj->m_szShowName);
    }
    return true;
}

QString TagIOGroup::getGroupNameByShowName(const QString &szShowName) {
    for(int i=0; i<listTagIOGroupDBItem_.count(); i++) {
        TagIOGroupDBItem *pObj = listTagIOGroupDBItem_.at(i);
        if(pObj->m_szShowName == szShowName) return pObj->m_szGroupName;
    }
    return QString();
}

TagIOGroupDBItem *TagIOGroup::getGroupObjByShowName(const QString &szShowName) {
    for(int i=0; i<listTagIOGroupDBItem_.count(); i++) {
        TagIOGroupDBItem *pObj = listTagIOGroupDBItem_.at(i);
        if(pObj->m_szShowName == szShowName) return pObj;
    }
    return Q_NULLPTR;
}

int TagIOGroup::getGroupCount()
{
    return this->listTagIOGroupDBItem_.count();
}

int TagIOGroup::getGroupCountByShowName(const QString &szShowName)
{
    int iCnt = 0;
    for(int i=0; i<listTagIOGroupDBItem_.count(); i++) {
        TagIOGroupDBItem *pObj = listTagIOGroupDBItem_.at(i);
        if(pObj->m_szShowName == szShowName) return ++iCnt;
    }
    return iCnt;
}
