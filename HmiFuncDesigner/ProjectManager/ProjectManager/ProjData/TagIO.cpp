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


bool TagIO::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(listTagIODBItem_);
    listTagIODBItem_.clear();
    XMLObject *pTagIOsObj = pXmlObj->getCurrentChild("tag_ios");
    if(pTagIOsObj == Q_NULLPTR) return false;
    QList<XMLObject* > listTagIOGroupsObj = pTagIOsObj->getCurrentChildren("tag_io");
    foreach(XMLObject* pTagIOGroupObj, listTagIOGroupsObj) {
        TagIODBItem *pObj = new TagIODBItem();
        pObj->m_szTagID = pTagIOGroupObj->getProperty("id");
        pObj->m_szGroupName = pTagIOGroupObj->getProperty("group");
        pObj->m_szName = pTagIOGroupObj->getProperty("name");
        pObj->m_szDescription = pTagIOGroupObj->getProperty("desc");
        pObj->m_szDeviceName = pTagIOGroupObj->getProperty("dev_name");
        pObj->m_szDeviceAddr = pTagIOGroupObj->getProperty("dev_addr");
        pObj->m_szRegisterArea = pTagIOGroupObj->getProperty("reg_area");
        pObj->m_szRegisterAddr = pTagIOGroupObj->getProperty("reg_addr");
        pObj->m_szAddrOffset = pTagIOGroupObj->getProperty("offset");
        pObj->m_szReadWriteType = pTagIOGroupObj->getProperty("rw");
        pObj->m_szDataType = pTagIOGroupObj->getProperty("data_type");
        pObj->m_szInitVal = pTagIOGroupObj->getProperty("init");
        pObj->m_szMinVal = pTagIOGroupObj->getProperty("min");
        pObj->m_szMaxVal = pTagIOGroupObj->getProperty("max");
        pObj->m_szScale = pTagIOGroupObj->getProperty("scale");
        pObj->m_szProjectConverter = pTagIOGroupObj->getProperty("conv");
        listTagIODBItem_.append(pObj);
    }
    return true;
}


bool TagIO::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pTagIOsObj = new XMLObject(pXmlObj);
    pTagIOsObj->setTagName("tag_ios");
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        XMLObject *pTagIOObj = new XMLObject(pTagIOsObj);
        pTagIOObj->setTagName("tag_io");
        pTagIOObj->setProperty("id", pObj->m_szTagID);
        pTagIOObj->setProperty("group", pObj->m_szGroupName);
        pTagIOObj->setProperty("name", pObj->m_szName);
        pTagIOObj->setProperty("desc", pObj->m_szDescription);
        pTagIOObj->setProperty("dev_name", pObj->m_szDeviceName);
        pTagIOObj->setProperty("dev_addr", pObj->m_szDeviceAddr);
        pTagIOObj->setProperty("reg_area", pObj->m_szRegisterArea);
        pTagIOObj->setProperty("reg_addr", pObj->m_szRegisterAddr);
        pTagIOObj->setProperty("offset", pObj->m_szAddrOffset);
        pTagIOObj->setProperty("rw", pObj->m_szReadWriteType);
        pTagIOObj->setProperty("data_type", pObj->m_szDataType);
        pTagIOObj->setProperty("init", pObj->m_szInitVal);
        pTagIOObj->setProperty("min", pObj->m_szMinVal);
        pTagIOObj->setProperty("max", pObj->m_szMaxVal);
        pTagIOObj->setProperty("scale", pObj->m_szScale);
        pTagIOObj->setProperty("conv", pObj->m_szProjectConverter);
    }
    return true;
}


TagIODBItem *TagIO::getTagIODBItemByID(const QString &id) {
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        if(pObj->m_szTagID == id)
            return pObj;
    }
    return Q_NULLPTR;
}

TagIODBItem *TagIO::getTagIODBItemByName(const QString &name) {
    for(int i=0; i<listTagIODBItem_.count(); i++) {
        TagIODBItem *pObj = listTagIODBItem_.at(i);
        if(pObj->m_szName == name)
            return pObj;
    }
    return Q_NULLPTR;
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
