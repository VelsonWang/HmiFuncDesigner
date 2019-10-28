#include "TagSys.h"
#include "ProjectDataSQLiteDatabase.h"
#include "ulog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

TagSys::TagSys()
{
    listTagSysDBItem_.clear();
}

TagSys::~TagSys()
{
    qDeleteAll(listTagSysDBItem_);
    listTagSysDBItem_.clear();
}

/**
 * @brief TagSys::load
 * @details 读取系统标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagSys::load(ProjectDataSQLiteDatabase *pDB)
{
    QSqlQuery query(pDB->db_);
    QSqlRecord rec;

    bool ret = query.exec("select * from t_tag_sys");
    if(!ret) {
        LogError(QString("get record: %1 failed! %2 ,error: %3!")
                 .arg("t_tag_sys")
                 .arg(query.lastQuery())
                 .arg(query.lastError().text()));
        return false;
    }

    qDeleteAll(listTagSysDBItem_);
    listTagSysDBItem_.clear();

    while (query.next()) {
        rec = query.record();
        TagSysDBItem *pObj = new TagSysDBItem();
        pObj->m_szTagID = rec.value("tagid").toString();
        pObj->m_szName = rec.value("name").toString();
        pObj->m_szDescription = rec.value("description").toString();
        pObj->m_szUnit = rec.value("unit").toString();
        pObj->m_szProjectConverter = rec.value("converter").toString();
        pObj->m_szComments = rec.value("comments").toString();
        listTagSysDBItem_.append(pObj);
    }

    return ret;
}


/**
 * @brief TagSys::save
 * @details 保存系统标签变量
 * @param pDB 数据库对象
 * @return true-成功, false-失败
 */
bool TagSys::save(ProjectDataSQLiteDatabase *pDB)
{
    Q_UNUSED(pDB)
    return false;
}


bool TagSys::insert(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj)
{
    Q_UNUSED(pDB)
    Q_UNUSED(pObj)
    return false;
}

bool TagSys::del(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj)
{
    Q_UNUSED(pDB)
    Q_UNUSED(pObj)
    return false;
}

bool TagSys::delAll(ProjectDataSQLiteDatabase *pDB)
{
    Q_UNUSED(pDB)
    return false;
}


bool TagSys::update(ProjectDataSQLiteDatabase *pDB, TagSysDBItem *pObj)
{
    Q_UNUSED(pDB)
    Q_UNUSED(pObj)
    return false;
}

TagSysDBItem *TagSys::getTagSysDBItemByID(const QString &id)
{
    for(int i=0; i<listTagSysDBItem_.count(); i++) {
        TagSysDBItem *pObj = listTagSysDBItem_.at(i);
        if(pObj->m_szTagID == id)
            return pObj;
    }
    return Q_NULLPTR;
}

TagSysDBItem *TagSys::getTagSysDBItemByName(const QString &name)
{
    for(int i=0; i<listTagSysDBItem_.count(); i++) {
        TagSysDBItem *pObj = listTagSysDBItem_.at(i);
        if(pObj->m_szName == name)
            return pObj;
    }
    return Q_NULLPTR;
}


int TagSys::getLastInsertId(ProjectDataSQLiteDatabase *pDB)
{
    return pDB->getLastInsertId("t_tag_sys");
}

