#ifndef PICTURERESOURCEMANAGER_H
#define PICTURERESOURCEMANAGER_H

#include <QString>
#include <QList>
#include <QImage>
#include <QByteArray>
#include "XMLObject.h"
#include "projdata_global.h"

class PROJDATASHARED_EXPORT PictureInfo
{
public:
    PictureInfo();
    ~PictureInfo();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    int m_iID;
    int m_iRefCnt; // 引用计数
    QString m_szName; // 图片名称
    QString m_szFormat; // 图片格式
    QByteArray m_datas; // 图片数据
};


class PROJDATASHARED_EXPORT PictureResourceManager
{
public:
    PictureResourceManager();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    bool add(const QString &szName);
    bool del(const QString &szName);

    QImage getPicture(const QString &szName);

    // 分配一个ID
    static int allocID();

    static QByteArray imageToBase64(QImage &imageObj, QString szFormat = "PNG");
    static QImage base64ToImage(QByteArray baseByteArray, QString szFormat = "PNG");

private:
    QList<PictureInfo *> m_listPictures;
    static int m_iStartNewID;
};

#endif // PICTURERESOURCEMANAGER_H
