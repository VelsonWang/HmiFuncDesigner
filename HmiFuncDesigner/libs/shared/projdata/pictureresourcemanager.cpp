#include "pictureresourcemanager.h"
#include <QFileInfo>
#include <QFile>
#include <string>
#include <QBuffer>
#include <QPixmap>
#include <sstream>

PictureInfo::PictureInfo()
{
    m_iID = 0;
}

PictureInfo::~PictureInfo()
{

}

bool PictureInfo::openFromXml(XMLObject *pXmlObj)
{
    m_iID = pXmlObj->getProperty("id").toInt();
    m_iRefCnt = pXmlObj->getProperty("ref_cnt").toInt();
    m_szName = pXmlObj->getProperty("name");
    m_szFormat = pXmlObj->getProperty("format");
    QString szDatas = pXmlObj->getText();
    if(szDatas != "") {
        std::string szDatasTmp = szDatas.toStdString();
        QByteArray ba = QByteArray(szDatasTmp.c_str());
        m_datas = ba.fromBase64(ba);
    }
    return true;
}

bool PictureInfo::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pPicObj = new XMLObject(pXmlObj);
    pPicObj->setTagName("pic");
    pPicObj->setProperty("id", QString::number(m_iID));
    pPicObj->setProperty("ref_cnt", QString::number(m_iRefCnt));
    pPicObj->setProperty("name", m_szName);
    pPicObj->setProperty("format", m_szFormat);
    pPicObj->setText(QString(m_datas.toBase64()));
    return true;
}

///////////////////////////////////////////////////////////

int PictureResourceManager::m_iStartNewID = 0;

PictureResourceManager::PictureResourceManager()
{

}

bool PictureResourceManager::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(m_listPictures);
    m_listPictures.clear();
    XMLObject *pPicsObj = pXmlObj->getCurrentChild("pics");
    if(pPicsObj == NULL) {
        return false;
    }
    QVector<XMLObject* > listPicsObj = pPicsObj->getCurrentChildren("pic");
    foreach(XMLObject* pPicObj, listPicsObj) {
        PictureInfo *pObj = new PictureInfo();
        pObj->openFromXml(pPicObj);
        if(m_iStartNewID < pObj->m_iID) {
            m_iStartNewID = pObj->m_iID;
        }
        m_listPictures.append(pObj);
    }
    return true;
}

bool PictureResourceManager::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pPicsObj = new XMLObject(pXmlObj);
    pPicsObj->setTagName("pics");
    for(int i = 0; i < m_listPictures.count(); i++) {
        PictureInfo *pObj = m_listPictures.at(i);
        pObj->saveToXml(pPicsObj);
    }
    return true;
}

bool PictureResourceManager::add(const QString &szName)
{
    QFileInfo info(szName);
    if(!info.exists()) {
        return false;
    }

    QString szPicName = info.fileName();

    bool bFound = false;
    foreach(PictureInfo *pObj, m_listPictures) {
        if(szPicName == pObj->m_szName) {
            bFound = true;
            pObj->m_iRefCnt++;
        }
    }
    if(!bFound) {
        PictureInfo *pObj = new PictureInfo();
        pObj->m_iID = allocID();
        pObj->m_iRefCnt = 1;
        pObj->m_szName = szPicName;
        pObj->m_szFormat = info.suffix();
        QImage imageObj(szName);
        pObj->m_datas = imageToBase64(imageObj, pObj->m_szFormat);
        m_listPictures.append(pObj);
    }

    return true;
}


bool PictureResourceManager::del(const QString &szName)
{
    foreach(PictureInfo *pObj, m_listPictures) {
        if(szName == pObj->m_szName) {
            if(pObj->m_iRefCnt == 1) {
                m_listPictures.removeOne(pObj);
                return true;
            } else {
                pObj->m_iRefCnt--;
            }
        }
    }
    return true;
}


QImage PictureResourceManager::getPicture(const QString &szName)
{
    foreach(PictureInfo *pObj, m_listPictures) {
        if(szName == pObj->m_szName) {
            return base64ToImage(pObj->m_datas, pObj->m_szFormat);
        }
    }

    return QImage();
}

/**
 * @brief PictureResourceManager::allocID
 * @details 分配一个
 * @return ID
 */
int PictureResourceManager::allocID()
{
    ++m_iStartNewID;
    return m_iStartNewID;
}



QByteArray PictureResourceManager::imageToBase64(QImage &imageObj, QString szFormat)
{
    QByteArray ba;
    QBuffer buf(&ba);
    imageObj.save((QIODevice *)&buf, szFormat.toStdString().c_str());
    QByteArray baBase64 = ba.toBase64();
    buf.close();
    return baBase64;
}

QImage PictureResourceManager::base64ToImage(QByteArray baseByteArray, QString szFormat)
{
    QByteArray baImage;
    baImage = QByteArray::fromBase64(baseByteArray);
    QBuffer buffer(&baImage);
    buffer.open(QIODevice::WriteOnly);
    QImage imageObj;
    imageObj.loadFromData(baImage, szFormat.toStdString().c_str());
    return imageObj;
}


