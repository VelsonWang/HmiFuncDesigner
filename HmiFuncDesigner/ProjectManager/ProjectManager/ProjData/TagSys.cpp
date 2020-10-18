#include "TagSys.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TagSys::TagSys()
{
    listTagSysDBItem_.clear();
}

TagSys::~TagSys()
{
    qDeleteAll(listTagSysDBItem_);
    listTagSysDBItem_.clear();
}


bool TagSys::openFromXml(XMLObject *pXmlObj) {
    qDeleteAll(listTagSysDBItem_);
    listTagSysDBItem_.clear();
    XMLObject *pTagSyssObj = pXmlObj->getCurrentChild("tag_syss");
    if(pTagSyssObj == Q_NULLPTR) {
        QString szTagSysConfig = QCoreApplication::applicationDirPath() + "/SysVarList.odb";
        QFile jasonFile(szTagSysConfig);
        if (!jasonFile.open(QIODevice::ReadOnly)) return false;
        QByteArray saveData = jasonFile.readAll();
        jasonFile.close();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        const QJsonObject json = loadDoc.object();
        QJsonArray tagSysArray = json["SysVarArray"].toArray();
        for (int i = 0; i < tagSysArray.size(); ++i) {
            QJsonObject jsonObj = tagSysArray[i].toObject();
            TagSysDBItem *pObj = new TagSysDBItem();
            pObj->m_szTagID = jsonObj["sID"].toString();
            pObj->m_szName = jsonObj["sName"].toString();
            pObj->m_szDescription = jsonObj["sDescription"].toString();
            pObj->m_szUnit = jsonObj["sUnit"].toString();
            pObj->m_szProjectConverter = jsonObj["sProjectConverter"].toString();
            pObj->m_szComments = jsonObj["sComments"].toString();
            listTagSysDBItem_.append(pObj);
        }
        return true;
    }

    QList<XMLObject* > listTagSyssObj = pTagSyssObj->getCurrentChildren("tag_sys");
    foreach(XMLObject* pTagSysObj, listTagSyssObj) {
        TagSysDBItem *pObj = new TagSysDBItem();
        pObj->m_szTagID = pTagSysObj->getProperty("id");
        pObj->m_szName = pTagSysObj->getProperty("name");
        pObj->m_szDescription = pTagSysObj->getProperty("desc");
        pObj->m_szUnit = pTagSysObj->getProperty("unit");
        pObj->m_szProjectConverter = pTagSysObj->getProperty("conv");
        pObj->m_szComments = pTagSysObj->getProperty("comments");
        listTagSysDBItem_.append(pObj);
    }
    return true;
}


bool TagSys::saveToXml(XMLObject *pXmlObj) {
    XMLObject *pTagSyssObj = new XMLObject(pXmlObj);
    pTagSyssObj->setTagName("tag_syss");
    for(int i=0; i<listTagSysDBItem_.count(); i++) {
        TagSysDBItem *pObj = listTagSysDBItem_.at(i);
        XMLObject *pTagSysObj = new XMLObject(pTagSyssObj);
        pTagSysObj->setTagName("tag_sys");
        pTagSysObj->setProperty("id", pObj->m_szTagID);
        pTagSysObj->setProperty("name", pObj->m_szName);
        pTagSysObj->setProperty("desc", pObj->m_szDescription);
        pTagSysObj->setProperty("unit", pObj->m_szUnit);
        pTagSysObj->setProperty("conv", pObj->m_szProjectConverter);
        pTagSysObj->setProperty("comments", pObj->m_szComments);
    }
    return true;
}



