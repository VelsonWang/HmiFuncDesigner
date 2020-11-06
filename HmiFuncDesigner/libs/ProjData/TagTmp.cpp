#include "TagTmp.h"

TagTmp::TagTmp()
{
    listTagTmpDBItem_.clear();
}

TagTmp::~TagTmp()
{
    qDeleteAll(listTagTmpDBItem_);
    listTagTmpDBItem_.clear();
}


bool TagTmp::openFromXml(XMLObject *pXmlObj) {
    qDeleteAll(listTagTmpDBItem_);
    listTagTmpDBItem_.clear();
    XMLObject *pTagTmpsObj = pXmlObj->getCurrentChild("tag_tmps");
    if(pTagTmpsObj == Q_NULLPTR) return false;
    QVector<XMLObject* > listTagTempsObj = pTagTmpsObj->getCurrentChildren("tag_tmp");
    foreach(XMLObject* pTagTempObj, listTagTempsObj) {
        TagTmpDBItem *pObj = new TagTmpDBItem();
        pObj->m_szTagID = pTagTempObj->getProperty("id");
        pObj->m_szName = pTagTempObj->getProperty("name");
        pObj->m_szDataType = pTagTempObj->getProperty("type");
        pObj->m_szDescription = pTagTempObj->getProperty("desc");
        pObj->m_szInitVal = pTagTempObj->getProperty("init");
        pObj->m_szMinVal = pTagTempObj->getProperty("min");
        pObj->m_szMaxVal = pTagTempObj->getProperty("max");
        pObj->m_szProjectConverter = pTagTempObj->getProperty("conv");
        listTagTmpDBItem_.append(pObj);
    }
    return true;
}


bool TagTmp::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pTagTmpsObj = new XMLObject(pXmlObj);
    pTagTmpsObj->setTagName("tag_tmps");
    for(int i=0; i<listTagTmpDBItem_.count(); i++) {
        TagTmpDBItem *pObj = listTagTmpDBItem_.at(i);
        XMLObject *pTagTmpObj = new XMLObject(pTagTmpsObj);
        pTagTmpObj->setTagName("tag_tmp");
        pTagTmpObj->setProperty("id", pObj->m_szTagID);
        pTagTmpObj->setProperty("name", pObj->m_szName);
        pTagTmpObj->setProperty("type", pObj->m_szDataType);
        pTagTmpObj->setProperty("desc", pObj->m_szDescription);
        pTagTmpObj->setProperty("init", pObj->m_szInitVal);
        pTagTmpObj->setProperty("min", pObj->m_szMinVal);
        pTagTmpObj->setProperty("max", pObj->m_szMaxVal);
        pTagTmpObj->setProperty("conv", pObj->m_szProjectConverter);
    }
    return true;
}


TagTmpDBItem *TagTmp::getTagTmpDBItemByID(const QString &id)
{
    for(int i=0; i<listTagTmpDBItem_.count(); i++) {
        TagTmpDBItem *pObj = listTagTmpDBItem_.at(i);
        if(pObj->m_szTagID == id) return pObj;
    }
    return Q_NULLPTR;
}



