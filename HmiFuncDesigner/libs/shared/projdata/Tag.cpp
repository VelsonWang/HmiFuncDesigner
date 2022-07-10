#include "tag.h"
#include "limits.h"
#include <QDebug>


Tag::Tag() : m_id(0)
{
}

Tag::~Tag()
{

}


Tag::Tag(const Tag &obj)
{
    copyObject(obj);
}


Tag &Tag::operator=(const Tag &obj)
{
    copyObject(obj);
    return *this;
}

void Tag::copyObject(const Tag &obj)
{
    m_name = obj.m_name;
    m_unit = obj.m_unit;
    m_addrType = obj.m_addrType;
    m_addrOffset = obj.m_addrOffset;
    m_addrType2 = obj.m_addrType2;
    m_addrOffset2 = obj.m_addrOffset2;
    m_dataType = obj.m_dataType;
    m_writeable = obj.m_writeable;
    m_remark = obj.m_remark;
    m_ownGroup = obj.m_ownGroup;
    m_devType = obj.m_devType;
}


void Tag::copyFromTag(Tag obj)
{
    m_name = obj.m_name;
    m_unit = obj.m_unit;
    m_addrType = obj.m_addrType;
    m_addrOffset = obj.m_addrOffset;
    m_addrType2 = obj.m_addrType2;
    m_addrOffset2 = obj.m_addrOffset2;
    m_dataType = obj.m_dataType;
    m_writeable = obj.m_writeable;
    m_remark = obj.m_remark;
    m_ownGroup = obj.m_ownGroup;
    m_devType = obj.m_devType;
}


bool Tag::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pTagObj = pXmlObj;
    if(pXmlObj == NULL) {
        return false;
    }
    m_id = pTagObj->getProperty("id").toInt();
    m_blockReadId = pTagObj->getProperty("blockReadId").toInt();
    m_name = pTagObj->getProperty("name");
    m_unit = pTagObj->getProperty("unit");
    m_addrType = pTagObj->getProperty("addr");
    m_addrOffset = pTagObj->getProperty("offset");
    m_addrType2 = pTagObj->getProperty("addr2");
    m_addrOffset2 = pTagObj->getProperty("offset2");
    m_dataType = pTagObj->getProperty("type");
    m_writeable = pTagObj->getProperty("writeable").toInt();
    m_remark = pTagObj->getProperty("remark");
    m_ownGroup = pTagObj->getProperty("group");
    m_devType = pTagObj->getProperty("dev");
    return true;
}


bool Tag::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pTagObj = new XMLObject(pXmlObj);
    return saveToXmlInner(pTagObj);
}

bool Tag::saveToXmlInner(XMLObject *pXmlObj)
{
    pXmlObj->setTagName("tag");
    pXmlObj->setProperty("id", QString::number(m_id));
    pXmlObj->setProperty("blockReadId", QString::number(m_blockReadId));
    pXmlObj->setProperty("name", m_name);
    pXmlObj->setProperty("unit", m_unit);
    pXmlObj->setProperty("addr", m_addrType);
    pXmlObj->setProperty("offset", m_addrOffset);
    pXmlObj->setProperty("addr2", m_addrType2);
    pXmlObj->setProperty("offset2", m_addrOffset2);
    pXmlObj->setProperty("type", m_dataType);
    pXmlObj->setProperty("writeable", QString::number(m_writeable));
    pXmlObj->setProperty("remark", m_remark);
    pXmlObj->setProperty("group", m_ownGroup);
    pXmlObj->setProperty("dev", m_devType);
    return true;
}

QString Tag::toXmlNodeString()
{
    XMLObject tagXml;
    saveToXmlInner(&tagXml);
    return tagXml.write();
}

bool Tag::fromXmlNodeString(const QString &szNode)
{
    XMLObject tagXml;
    if(!tagXml.load(szNode, NULL)) {
        return false;
    }
    return openFromXml(&tagXml);
}

QJsonObject Tag::toJsonObject()
{
    QJsonObject jsonTagObj;
    jsonTagObj.insert("id", QJsonValue(m_id));
    jsonTagObj.insert("blockReadId", QJsonValue(m_blockReadId));
    jsonTagObj.insert("name", QJsonValue(m_name));
    jsonTagObj.insert("unit", QJsonValue(m_unit));
    jsonTagObj.insert("addr", QJsonValue(m_addrType));
    jsonTagObj.insert("offset", QJsonValue(m_addrOffset));
    jsonTagObj.insert("addr2", QJsonValue(m_addrType2));
    jsonTagObj.insert("offset2", QJsonValue(m_addrOffset2));
    jsonTagObj.insert("type", QJsonValue(m_dataType));
    jsonTagObj.insert("writeable", QJsonValue(m_writeable));
    jsonTagObj.insert("remark", QJsonValue(m_remark));
    jsonTagObj.insert("group", QJsonValue(m_ownGroup));
    jsonTagObj.insert("dev", QJsonValue(m_devType));
    return jsonTagObj;
}

void Tag::fromJsonObject(QJsonObject jsonObj)
{
    if(!jsonObj.isEmpty()) {
        m_id = jsonObj["id"].toInt();
        m_blockReadId = jsonObj["blockReadId"].toInt();
        m_name = jsonObj["name"].toString();
        m_unit = jsonObj["unit"].toString();
        m_addrType = jsonObj["addr"].toString();
        m_addrOffset = jsonObj["offset"].toString();
        m_addrType2 = jsonObj["addr2"].toString();
        m_addrOffset2 = jsonObj["offset2"].toString();
        m_dataType = jsonObj["type"].toString();
        m_writeable = jsonObj["writeable"].toInt();
        m_remark = jsonObj["remark"].toString();
        m_ownGroup = jsonObj["group"].toString();
        m_devType = jsonObj["dev"].toString();
    }
}

TagManager::TagManager()
{
    m_vecTags.clear();
}

TagManager::~TagManager()
{
    qDeleteAll(m_vecTags);
    m_vecTags.clear();
}


bool TagManager::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(m_vecTags);
    m_vecTags.clear();
    QVector<XMLObject* > listTagsObj = pXmlObj->getCurrentChildren("tag");
    foreach(XMLObject* pTagObj, listTagsObj) {
        Tag *pObj = new Tag();
        pObj->openFromXml(pTagObj);
        m_vecTags.append(pObj);
    }
    return true;
}


bool TagManager::saveToXml(XMLObject *pXmlObj)
{
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags.at(i);
        pObj->saveToXml(pXmlObj);
    }
    return true;
}

bool TagManager::saveBlockReadTagToXml(XMLObject *pXmlObj)
{
    QList<QString> devs = m_mapDevBlockReadTags.keys();
    foreach (QString dev, devs) {
        XMLObject *pDevObj = new XMLObject(pXmlObj);
        pDevObj->setTagName(dev);
        QVector<Tag *> &vecTags = m_mapDevBlockReadTags[dev];
        for(int i = 0; i < vecTags.count(); i++) {
            Tag *pObj = vecTags.at(i);
            pObj->saveToXml(pDevObj);
        }
    }
    return true;
}

/**
 * @brief TagManager::allocID
 * @details 分配一个
 * @return ID
 */
int TagManager::allocID()
{
    int notUseID = 1;
    while(notUseID < INT_MAX) {
        bool found = false;
        for(int i = 0; i < m_vecTags.count(); i++) {
            Tag *pObj = m_vecTags.at(i);
            if(pObj->m_id == notUseID) {
                found = true;
                notUseID++;
                break;
            }
        }
        if(!found) {
            return notUseID;
        }
    }
    return 0;
}

Tag *TagManager::getTag(int id)
{
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags.at(i);
        if(pObj->m_id == id) {
            return pObj;
        }
    }
    return NULL;
}

Tag *TagManager::getBlockReadTag(int id)
{
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags.at(i);
        if(pObj->m_blockReadId == id) {
            return pObj;
        }
    }
    return NULL;
}

void TagManager::getAllTagName(QStringList &szList)
{
    szList.clear();
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags[i];
        szList.append(QString("%1:%2").arg(pObj->m_id).arg(pObj->m_name));
    }
}

void TagManager::debugInfo()
{
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags[i];
        qDebug() << pObj->toXmlNodeString();
    }
}
