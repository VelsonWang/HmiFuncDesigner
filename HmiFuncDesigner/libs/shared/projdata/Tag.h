#ifndef TAG_H
#define TAG_H

#include <QString>
#include <QVector>
#include <QMetaType>
#include <QJsonObject>
#include <QJsonValue>
#include "../xmlobject.h"
#include "../sharedlibglobal.h"

class SHAREDLIB_EXPORT Tag
{
public:
    Tag();
    Tag(const Tag &obj);
    Tag &operator=(const Tag &obj);
    void copyObject(const Tag &obj);
    void copyFromTag(Tag obj);
    ~Tag();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    QString toXmlNodeString();
    bool fromXmlNodeString(const QString &szNode);

    QJsonObject toJsonObject();
    void fromJsonObject(QJsonObject jsonObj);

public:
    ////////////////////<基本信息>//////////////////////////

    int m_id = 0; // 变量ID
    QString m_name = ""; // 变量名称
    QString m_unit = ""; // 变量单位
    QString m_addrType = ""; // 地址类型
    QString m_addrOffset = "0"; // 地址偏移
    QString m_addrType2 = ""; // 地址类型2
    QString m_addrOffset2 = "0"; // 地址偏移2
    QString m_dataType = ""; // 数据类型
    int m_writeable = 1; // 读写类型  0-只读, 1-可写
    QString m_remark = ""; // 变量描述
    QString m_ownGroup = ""; // 变量所属组
    QString m_devType = ""; // 变量设备类型-设备协议名称, 内存变量-MEMORY, 系统变量-SYSTEM

private:
    bool saveToXmlInner(XMLObject *pXmlObj);
};
Q_DECLARE_METATYPE(Tag)

class SHAREDLIB_EXPORT TagManager
{
public:
    TagManager();
    ~TagManager();

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

    //分配一个 ID
    int allocID();

    Tag *getTag(int id);

    void getAllTagName(QStringList &szList);

public:
    QVector<Tag *> m_vecTags;
    static int m_startNewID;
};
Q_DECLARE_METATYPE(TagManager)

#endif // TAG_H
