#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <QTextCodec>
#include <QString>
#include <QJsonObject>
#include <QMap>
#include "DBVarGroup.h"

enum TAG_TYPE{
    TAG_SYS,
    TAG_IO,
    TAG_TMP
};
typedef TAG_TYPE TTag;


//////////////////////////////////////////////////////////


struct TagSysItem
{
    explicit TagSysItem(const QString &sVarid=QString(),
            TTag tagtype=TAG_SYS,
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_sTagID(sVarid), m_iTagType(tagtype), m_sName(sName), m_sDescription(sDescription),
          m_sUnit(sUnit), m_sProjectConverter(sProjectConverter), m_sComments(sComments)
    {

    }

    TagSysItem(const TagSysItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
    }

    TagSysItem &operator=(const TagSysItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
        return *this;
    }

    void load(const QJsonObject &json) {
            m_sTagID = json["sID"].toString();
            m_sName = json["sName"].toString();
            m_sDescription = json["sDescription"].toString();
            m_sUnit = json["sUnit"].toString();
            m_sProjectConverter = json["sProjectConverter"].toString();
            m_sComments = json["sComments"].toString();
    }

    void save(QJsonObject &json) {
        json["sID"] = m_sTagID;
        json["sName"] = m_sName;
        json["sDescription"] = m_sDescription;
        json["sUnit"] = m_sUnit;
        json["sProjectConverter"] = m_sProjectConverter;
        json["sComments"]= m_sComments;
    }

    QString m_sTagID; // 变量ID
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
};


//////////////////////////////////////////////////////////

struct TagTmpItem
{
    explicit TagTmpItem(const QString &sVarid=QString(),
            TTag tagtype=TAG_TMP,
            const QString &sDataType=QString(),
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sActionScope=QString(),
            const QString &sDataAttribute=QString(),
            const QString &sAlarm=QString(),
            const QString &sArchiveFile=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_sTagID(sVarid),m_iTagType(tagtype), m_sDataType(sDataType),
          m_sName(sName), m_sDescription(sDescription),m_sUnit(sUnit),
          m_sActionScope(sActionScope), m_sDataAttribute(sDataAttribute),m_sAlarm(sAlarm),
          m_sArchiveFile(sArchiveFile),m_sProjectConverter(sProjectConverter),m_sComments(sComments)
    {
        m_iTagType = TAG_TMP;
    }

    TagTmpItem(const TagTmpItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_sDataType = rh.m_sDataType;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sActionScope = rh.m_sActionScope;
        m_sDataAttribute = rh.m_sDataAttribute;
        m_sAlarm = rh.m_sAlarm;
        m_sArchiveFile = rh.m_sArchiveFile;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
    }

    TagTmpItem &operator=(const TagTmpItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_sDataType = rh.m_sDataType;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sActionScope = rh.m_sActionScope;
        m_sDataAttribute = rh.m_sDataAttribute;
        m_sAlarm = rh.m_sAlarm;
        m_sArchiveFile = rh.m_sArchiveFile;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
        return *this;
    }

    void load(const QJsonObject &json) {
        m_sTagID = json["sID"].toString();
        m_sName = json["sName"].toString();
        m_sDescription = json["sDescription"].toString();
        m_sUnit = json["sUnit"].toString();
        m_sProjectConverter = json["sProjectConverter"].toString();
        m_sComments = json["sComments"].toString();
        m_sDataType = json["sDataType"].toString();
        m_sActionScope = json["sActionScope"].toString();
        m_sDataAttribute = json["sDataAttribute"].toString();
        m_sAlarm = json["sAlarm"].toString();
        m_sArchiveFile = json["sArchiveFile"].toString();
    }

    void save(QJsonObject &json) {
        json["sID"] = m_sTagID;
        json["sDataType"] = m_sDataType;
        json["sName"] = m_sName;
        json["sDescription"] = m_sDescription;
        json["sUnit"] = m_sUnit;
        json["sActionScope"] = m_sActionScope;
        json["sDataAttribute"] = m_sDataAttribute;
        json["sAlarm"] = m_sAlarm;
        json["sArchiveFile"] = m_sArchiveFile;
        json["sProjectConverter"] = m_sProjectConverter;
        json["sComments"] = m_sComments;
    }

    QString m_sTagID; // 变量ID
    QString m_sDataType;  //数据类型
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sActionScope;  //作用域
    QString m_sDataAttribute;  //数据属性
    QString m_sAlarm;  //报警
    QString m_sArchiveFile;  //存盘
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
};

//////////////////////////////////////////////////////////

struct TagIOItem
{
    explicit TagIOItem(const QString &sVarid=QString(),
            TTag tagtype=TAG_IO,
            const QString &sDataType=QString(),
            const QString &sName=QString(),
            const QString &sDescription=QString(),
            const QString &sUnit=QString(),
            const QString &sIOConnect=QString(),
            const QString &sAlarm=QString(),
            const QString &sArchiveFile=QString(),
            const QString &sProjectConverter=QString(),
            const QString &sComments=QString())
        : m_sTagID(sVarid), m_iTagType(tagtype), m_sDataType(sDataType),
          m_sName(sName), m_sDescription(sDescription), m_sUnit(sUnit),
          m_sIOConnect(sIOConnect), m_sAlarm(sAlarm),
          m_sArchiveFile(sArchiveFile), m_sProjectConverter(sProjectConverter), m_sComments(sComments)
    {
        m_iTagType = TAG_IO;
    }

    TagIOItem(const TagIOItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_sDataType = rh.m_sDataType;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sIOConnect = rh.m_sIOConnect;
        m_sAlarm = rh.m_sAlarm;
        m_sArchiveFile = rh.m_sArchiveFile;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
    }

    TagIOItem &operator=(const TagIOItem &rh) {
        m_sTagID = rh.m_sTagID;
        m_sDataType = rh.m_sDataType;
        m_iTagType = rh.m_iTagType;
        m_sName = rh.m_sName;
        m_sDescription = rh.m_sDescription;
        m_sUnit = rh.m_sUnit;
        m_sIOConnect = rh.m_sIOConnect;
        m_sAlarm = rh.m_sAlarm;
        m_sArchiveFile = rh.m_sArchiveFile;
        m_sProjectConverter = rh.m_sProjectConverter;
        m_sComments = rh.m_sComments;
        return *this;
    }

    void load(const QJsonObject &json) {
        m_sTagID = json["sID"].toString();
        m_sName = json["sName"].toString();
        m_sDescription = json["sDescription"].toString();
        m_sUnit = json["sUnit"].toString();
        m_sProjectConverter = json["sProjectConverter"].toString();
        m_sComments = json["sComments"].toString();
        m_sDataType = json["sDataType"].toString();
        m_sIOConnect = json["sIOConnect"].toString();
        m_sAlarm = json["sAlarm"].toString();
        m_sArchiveFile = json["sArchiveFile"].toString();
    }

    void save(QJsonObject &json) {
        json["sID"] = m_sTagID;
        json["sDataType"] = m_sDataType;
        json["sName"] = m_sName;
        json["sDescription"] = m_sDescription;
        json["sUnit"] = m_sUnit;
        json["sIOConnect"] = m_sIOConnect;
        json["sAlarm"] = m_sAlarm;
        json["sArchiveFile"] = m_sArchiveFile;
        json["sProjectConverter"] = m_sProjectConverter;
        json["sComments"] = m_sComments;
    }

    QString m_sTagID; // 变量ID
    QString m_sDataType;  //数据类型
    TTag m_iTagType;  // 变量类型
    QString m_sName;  //名称
    QString m_sDescription;  //描述
    QString m_sUnit;  //单位
    QString m_sIOConnect;  //IO连接
    QString m_sAlarm;  //报警
    QString m_sArchiveFile;  //存盘
    QString m_sProjectConverter;  //工程转换
    QString m_sComments;  //备注
};


//////////////////////////////////////////////////////////


class TagManager
{

private:
    TagManager();
    ~TagManager();

public:
    static void clearData();

    // 设置工程路径
    static void setProjectPath(const QString &path);
    // 取得工程路径
    static QString getProjectPath();
    // 读取工程所有的变量
    static void loadProjectTags(const QString &path);
    // 保存工程所有的变量
    static void saveProjectTags(const QString &path);
    //获取工程所有变量的名称
    static void getAllTagName(const QString &proj_path,
                              QStringList &varList,
                              const QString &type = "ALL");
public:
    static QList<TagSysItem> tagSysItems_;
    static QList<TagTmpItem> tagTmpItems_;
    static QMap<QString, QList<TagIOItem>> listTagIOItems_;
    static DBVarGroups ioDBVarGroups_;

private:
    static QString szProjectPath_;
};


#endif // TAGMANAGER_H
