#include "TagTmp.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

TagTmp::TagTmp()
{
    listTagTmpDBItem_.clear();
}

TagTmp::~TagTmp()
{
    qDeleteAll(listTagTmpDBItem_);
    listTagTmpDBItem_.clear();
}

/**
 * @brief TagTmp::load
 * @details 读取中间标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagTmp::load(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_tag_tmp");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_tmp")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listTagTmpDBItem_);
    listTagTmpDBItem_.clear();

    while (query.next()) {
        rec = query.record();
        TagTmpDBItem *pObj = new TagTmpDBItem();
        pObj->m_szTagID = rec.value("tagid").toString();
        pObj->m_szDataType = rec.value("data_type").toString();
        pObj->m_szName = rec.value("name").toString();
        pObj->m_szDescription = rec.value("description").toString();
        pObj->m_szInitVal = rec.value("init_val").toString();
        pObj->m_szMinVal = rec.value("min_val").toString();
        pObj->m_szMaxVal = rec.value("max_val").toString();
        pObj->m_szProjectConverter = rec.value("converter").toString();
        listTagTmpDBItem_.append(pObj);
    }

    return ret;
}


/**
 * @brief TagTmp::save
 * @details 保存中间标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagTmp::save(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->db_);
    bool ret = false;

    pDB->beginTransaction();
    for(int i=0; i<listTagTmpDBItem_.count(); i++) {
        TagTmpDBItem *pObj = listTagTmpDBItem_.at(i);
        query.prepare("update t_tag_tmp set name = :name, data_type = :type, "
                      "description = :desc, init_val = :init, min_val = :min, "
                      "max_val = :max, converter = :conv where tagid = :id");

        query.bindValue(":id", pObj->m_szTagID);
        query.bindValue(":name", pObj->m_szName);
        query.bindValue(":type", pObj->m_szDataType);
        query.bindValue(":desc", pObj->m_szDescription);
        query.bindValue(":init", pObj->m_szInitVal);
        query.bindValue(":min", pObj->m_szMinVal);
        query.bindValue(":max", pObj->m_szMaxVal);
        query.bindValue(":conv", pObj->m_szProjectConverter);

        ret = query.exec();
        if(!ret) {
            LogError(QString("update record: %1 failed! %2 ,error: %3!")
                     .arg("t_tag_tmp")
                     .arg(query.lastQuery())
                     .arg(query.lastError().text()));
            pDB->rollbackTransaction();
        }
    }
    pDB->commitTransaction();

    return ret;
}


bool TagTmp::insert(ProjectDataSQLiteDatabase *pDB, TagTmpDBItem *pObj)
{
    QStringList keyList, valueList;

    keyList << "tagid" << "name" << "description" << "data_type"
            << "init_val" << "min_val" << "max_val" << "converter";

    valueList << pObj->m_szTagID << pObj->m_szName << pObj->m_szDescription << pObj->m_szDataType
              << pObj->m_szInitVal << pObj->m_szMinVal << pObj->m_szMaxVal << pObj->m_szProjectConverter;

    return pDB->insertRecord("t_tag_tmp", keyList, valueList);
}


bool TagTmp::insert(ProjectDataSQLiteDatabase *pDB, QList<TagTmpDBItem *> &pObjs)
{
    bool ret = false;

    pDB->beginTransaction();
    for(int i=0; i<pObjs.count(); i++) {
        TagTmpDBItem *pObj = pObjs.at(i);
        ret = insert(pDB, pObj);
        if(!ret) {
            pDB->rollbackTransaction();
        }
    }
    pDB->commitTransaction();

    return ret;
}


bool TagTmp::del(ProjectDataSQLiteDatabase *pDB, TagTmpDBItem *pObj)
{
    return pDB->deleteRecord("t_tag_tmp", QString("tagid='%1'").arg(pObj->m_szTagID));
}

bool TagTmp::delAll(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->deleteRecord("t_tag_tmp");
}


bool TagTmp::update(ProjectDataSQLiteDatabase *pDB, TagTmpDBItem *pObj)
{
    QSqlQuery query(pDB->db_);
    bool ret = false;

    query.prepare("update t_tag_tmp set name = :name, data_type = :type, "
                  "description = :desc, init_val = :init, min_val = :min, "
                  "max_val = :max, converter = :conv where tagid = :id");

    query.bindValue(":id", pObj->m_szTagID);
    query.bindValue(":name", pObj->m_szName);
    query.bindValue(":type", pObj->m_szDataType);
    query.bindValue(":desc", pObj->m_szDescription);
    query.bindValue(":init", pObj->m_szInitVal);
    query.bindValue(":min", pObj->m_szMinVal);
    query.bindValue(":max", pObj->m_szMaxVal);
    query.bindValue(":conv", pObj->m_szProjectConverter);

    ret = query.exec();
    if(!ret) {
        LogError(QString("update record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_tmp")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
    }

    return ret;
}

TagTmpDBItem *TagTmp::getTagTmpDBItemByID(ProjectDataSQLiteDatabase *pDB, const QString &id)
{
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;
    QString szSql = QString("select * from t_tag_tmp where tagid='%1'").arg(id);
    bool ret = query.exec(szSql);
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_tmp")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return Q_NULLPTR;
    }

    while (query.next()) {
        rec = query.record();
        TagTmpDBItem *pObj = new TagTmpDBItem();
        pObj->m_szTagID = rec.value("tagid").toString();
        pObj->m_szDataType = rec.value("data_type").toString();
        pObj->m_szName = rec.value("name").toString();
        pObj->m_szDescription = rec.value("description").toString();
        pObj->m_szInitVal = rec.value("init_val").toString();
        pObj->m_szMinVal = rec.value("min_val").toString();
        pObj->m_szMaxVal = rec.value("max_val").toString();
        pObj->m_szProjectConverter = rec.value("converter").toString();
        return pObj;
    }

    return Q_NULLPTR;
}


int TagTmp::getLastInsertId(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getLastInsertId("t_tag_tmp");
}


bool TagTmp::saveTagTmpDBItem(ProjectDataSQLiteDatabase *pDB, TagTmpDBItem *pObj)
{
    bool ret = false;
    if(pDB->getRowCount("t_tag_tmp", QString("tagid='%1'").arg(pObj->m_szTagID))) {
        ret = update(pDB, pObj);
    } else {
        ret = insert(pDB, pObj);
    }
    return ret;
}


int TagTmp::rowCount(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getRowCount("t_tag_tmp");
}
