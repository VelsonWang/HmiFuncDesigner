#include "Tag.h"


Tag::Tag() : m_iID(0)
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
    m_szName = obj.m_szName;
    m_szUnit = obj.m_szUnit;
    m_szAddrType = obj.m_szAddrType;
    m_szAddrOffset = obj.m_szAddrOffset;
    m_szAddrType2 = obj.m_szAddrType2;
    m_szAddrOffset2 = obj.m_szAddrOffset2;
    m_szDataType = obj.m_szDataType;
    m_iWriteable = obj.m_iWriteable;
    m_szRemark = obj.m_szRemark;
    m_szOwnGroup = obj.m_szOwnGroup;
    m_szDevType = obj.m_szDevType;
}


void Tag::copyFromTag(Tag obj)
{
    m_szName = obj.m_szName;
    m_szUnit = obj.m_szUnit;
    m_szAddrType = obj.m_szAddrType;
    m_szAddrOffset = obj.m_szAddrOffset;
    m_szAddrType2 = obj.m_szAddrType2;
    m_szAddrOffset2 = obj.m_szAddrOffset2;
    m_szDataType = obj.m_szDataType;
    m_iWriteable = obj.m_iWriteable;
    m_szRemark = obj.m_szRemark;
    m_szOwnGroup = obj.m_szOwnGroup;
    m_szDevType = obj.m_szDevType;
}


bool Tag::openFromXml(XMLObject *pXmlObj)
{
    XMLObject *pTagObj = pXmlObj;
    if(pXmlObj == Q_NULLPTR) {
        return false;
    }
    m_iID = pTagObj->getProperty("id").toInt();
    m_szName = pTagObj->getProperty("name");
    m_szUnit = pTagObj->getProperty("unit");
    m_szAddrType = pTagObj->getProperty("addr");
    m_szAddrOffset = pTagObj->getProperty("offset");
    m_szAddrType2 = pTagObj->getProperty("addr2");
    m_szAddrOffset2 = pTagObj->getProperty("offset2");
    m_szDataType = pTagObj->getProperty("type");
    m_iWriteable = pTagObj->getProperty("writeable").toInt();
    m_szRemark = pTagObj->getProperty("remark");
    m_szOwnGroup = pTagObj->getProperty("group");
    m_szDevType = pTagObj->getProperty("dev");
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
    pXmlObj->setProperty("id", QString::number(m_iID));
    pXmlObj->setProperty("name", m_szName);
    pXmlObj->setProperty("unit", m_szUnit);
    pXmlObj->setProperty("addr", m_szAddrType);
    pXmlObj->setProperty("offset", m_szAddrOffset);
    pXmlObj->setProperty("addr2", m_szAddrType2);
    pXmlObj->setProperty("offset2", m_szAddrOffset2);
    pXmlObj->setProperty("type", m_szDataType);
    pXmlObj->setProperty("writeable", QString::number(m_iWriteable));
    pXmlObj->setProperty("remark", m_szRemark);
    pXmlObj->setProperty("group", m_szOwnGroup);
    pXmlObj->setProperty("dev", m_szDevType);
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
    if(!tagXml.load(szNode, Q_NULLPTR)) {
        return false;
    }
    return openFromXml(&tagXml);
}

QJsonObject Tag::toJsonObject()
{
    QJsonObject jsonTagObj;
    jsonTagObj.insert("id", QJsonValue(m_iID));
    jsonTagObj.insert("name", QJsonValue(m_szName));
    jsonTagObj.insert("unit", QJsonValue(m_szUnit));
    jsonTagObj.insert("addr", QJsonValue(m_szAddrType));
    jsonTagObj.insert("offset", QJsonValue(m_szAddrOffset));
    jsonTagObj.insert("addr2", QJsonValue(m_szAddrType2));
    jsonTagObj.insert("offset2", QJsonValue(m_szAddrOffset2));
    jsonTagObj.insert("type", QJsonValue(m_szDataType));
    jsonTagObj.insert("writeable", QJsonValue(m_iWriteable));
    jsonTagObj.insert("remark", QJsonValue(m_szRemark));
    jsonTagObj.insert("group", QJsonValue(m_szOwnGroup));
    jsonTagObj.insert("dev", QJsonValue(m_szDevType));
    return jsonTagObj;
}

void Tag::fromJsonObject(QJsonObject jsonObj)
{
    if(!jsonObj.isEmpty()) {
        m_iID = jsonObj["id"].toInt();
        m_szName = jsonObj["name"].toString();
        m_szUnit = jsonObj["unit"].toString();
        m_szAddrType = jsonObj["addr"].toString();
        m_szAddrOffset = jsonObj["offset"].toString();
        m_szAddrType2 = jsonObj["addr2"].toString();
        m_szAddrOffset2 = jsonObj["offset2"].toString();
        m_szDataType = jsonObj["type"].toString();
        m_iWriteable = jsonObj["writeable"].toInt();
        m_szRemark = jsonObj["remark"].toString();
        m_szOwnGroup = jsonObj["group"].toString();
        m_szDevType = jsonObj["dev"].toString();
    }
}


int TagManager::m_iStartNewID = 0;

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
        if(m_iStartNewID < pObj->m_iID) {
            m_iStartNewID = pObj->m_iID;
        }
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


/**
 * @brief TagManager::allocID
 * @details 分配一个
 * @return ID
 */
int TagManager::allocID()
{
    ++m_iStartNewID;
    return m_iStartNewID;
}

Tag *TagManager::getTag(int id)
{
    for(int i = 0; i < m_vecTags.count(); i++) {
        Tag *pObj = m_vecTags.at(i);
        if(pObj->m_iID == id) {
            return pObj;
        }
    }
    return Q_NULLPTR;
}


