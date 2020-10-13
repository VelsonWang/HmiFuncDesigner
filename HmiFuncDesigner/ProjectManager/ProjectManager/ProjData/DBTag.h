#ifndef DB_TAG_H
#define DB_TAG_H

#include <QString>


struct TagSysDBItem
{
    explicit TagSysDBItem(const QString &sVarid=QString(),
                          const QString &sName=QString(),
                          const QString &sDescription=QString(),
                          const QString &sUnit=QString(),
                          const QString &sProjectConverter=QString(),
                          const QString &sComments=QString())
        : m_szTagID(sVarid), m_szName(sName), m_szDescription(sDescription),
          m_szUnit(sUnit), m_szProjectConverter(sProjectConverter), m_szComments(sComments)
    {

    }

    TagSysDBItem(const TagSysDBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szUnit = rh.m_szUnit;
        m_szProjectConverter = rh.m_szProjectConverter;
        m_szComments = rh.m_szComments;
    }

    TagSysDBItem &operator=(const TagSysDBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szUnit = rh.m_szUnit;
        m_szProjectConverter = rh.m_szProjectConverter;
        m_szComments = rh.m_szComments;
        return *this;
    }

    QString m_szTagID; // 变量ID
    QString m_szName;  //名称
    QString m_szDescription;  //描述
    QString m_szUnit;  //单位
    QString m_szProjectConverter;  //工程转换
    QString m_szComments;  //备注
};


//////////////////////////////////////////////////////////

struct TagTmpDBItem
{
    explicit TagTmpDBItem(const QString &sVarid=QString(),
                          const QString &sDataType=QString(),
                          const QString &sName=QString(),
                          const QString &sDescription=QString(),
                          const QString &sInitVal=QString(),
                          const QString &sMinVal=QString(),
                          const QString &sMaxVal=QString(),
                          const QString &sProjectConverter=QString())
        : m_szTagID(sVarid), m_szDataType(sDataType),
          m_szName(sName), m_szDescription(sDescription),m_szInitVal(sInitVal),
          m_szMinVal(sMinVal), m_szMaxVal(sMaxVal),m_szProjectConverter(sProjectConverter)
    {
    }

    TagTmpDBItem(const TagTmpDBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szDataType = rh.m_szDataType;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szInitVal = rh.m_szInitVal;
        m_szMinVal = rh.m_szMinVal;
        m_szMaxVal = rh.m_szMaxVal;
        m_szProjectConverter = rh.m_szProjectConverter;
    }

    TagTmpDBItem &operator=(const TagTmpDBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szDataType = rh.m_szDataType;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szInitVal = rh.m_szInitVal;
        m_szMinVal = rh.m_szMinVal;
        m_szMaxVal = rh.m_szMaxVal;
        m_szProjectConverter = rh.m_szProjectConverter;
        return *this;
    }

    QString m_szTagID; // 变量ID
    QString m_szDataType;  //数据类型
    QString m_szName;  //名称
    QString m_szDescription;  //描述
    QString m_szInitVal;  //初始值
    QString m_szMinVal;  //最小值
    QString m_szMaxVal;  //最大值
    QString m_szProjectConverter;  //工程转换
};

//////////////////////////////////////////////////////////

struct TagIODBItem
{
    explicit TagIODBItem(const QString &sVarid=QString(),
                         const QString &sGroupName=QString("group_1"),
                         const QString &sName=QString(),
                         const QString &sDesc=QString(),
                         const QString &sDevName=QString(),
                         const QString &sDevAddr=QString(),
                         const QString &sRegArea=QString(),
                         const QString &sRegAddr=QString(),
                         const QString &sAddrOff=QString(),
                         const QString &sType=QString(),
                         const QString &sDataType=QString(),
                         const QString &sInitVal=QString(),
                         const QString &sMinVal=QString(),
                         const QString &sMaxVal=QString(),
                         const QString &sScale=QString(),
                         const QString &sConverter=QString()
            )
        : m_szTagID(sVarid), m_szGroupName(sGroupName), m_szName(sName), m_szDescription(sDesc), m_szDeviceName(sDevName),
          m_szDeviceAddr(sDevAddr), m_szRegisterArea(sRegArea), m_szRegisterAddr(sRegAddr),
          m_szAddrOffset(sAddrOff), m_szReadWriteType(sType), m_szDataType(sDataType),
          m_szInitVal(sInitVal), m_szMinVal(sMinVal), m_szMaxVal(sMaxVal), m_szScale(sScale),
          m_szProjectConverter(sConverter)
    {
    }

    TagIODBItem(const TagIODBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szGroupName = rh.m_szGroupName;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szDeviceName = rh.m_szDeviceName;
        m_szDeviceAddr = rh.m_szDeviceAddr;
        m_szRegisterArea = rh.m_szRegisterArea;
        m_szRegisterAddr = rh.m_szRegisterAddr;
        m_szAddrOffset = rh.m_szAddrOffset;
        m_szReadWriteType = rh.m_szReadWriteType;
        m_szDataType = rh.m_szDataType;
        m_szInitVal = rh.m_szInitVal;
        m_szMinVal = rh.m_szMinVal;
        m_szMaxVal = rh.m_szMaxVal;
        m_szScale = rh.m_szScale;
        m_szProjectConverter = rh.m_szProjectConverter;
    }

    TagIODBItem &operator=(const TagIODBItem &rh) {
        m_szTagID = rh.m_szTagID;
        m_szGroupName = rh.m_szGroupName;
        m_szName = rh.m_szName;
        m_szDescription = rh.m_szDescription;
        m_szDeviceName = rh.m_szDeviceName;
        m_szDeviceAddr = rh.m_szDeviceAddr;
        m_szRegisterArea = rh.m_szRegisterArea;
        m_szRegisterAddr = rh.m_szRegisterAddr;
        m_szAddrOffset = rh.m_szAddrOffset;
        m_szReadWriteType = rh.m_szReadWriteType;
        m_szDataType = rh.m_szDataType;
        m_szInitVal = rh.m_szInitVal;
        m_szMinVal = rh.m_szMinVal;
        m_szMaxVal = rh.m_szMaxVal;
        m_szScale = rh.m_szScale;
        m_szProjectConverter = rh.m_szProjectConverter;
        return *this;
    }

    QString m_szTagID; // 变量ID
    QString m_szGroupName; // 变量所属组名
    QString m_szName;  //名称
    QString m_szDescription;  //变量描述
    QString m_szDeviceName;  //设备名
    QString m_szDeviceAddr;  //设备地址
    QString m_szRegisterArea;  //寄存器区
    QString m_szRegisterAddr;  //寄存器地址
    QString m_szAddrOffset;  //偏移地址
    QString m_szReadWriteType;  //读写类型
    QString m_szDataType;  //数据类型
    QString m_szInitVal;  //初始值
    QString m_szMinVal;  //最小值
    QString m_szMaxVal;  //最大值
    QString m_szScale;  //变比
    QString m_szProjectConverter;  //工程转换
};

//////////////////////////////////////////////////////////

struct TagIOGroupDBItem
{
    explicit TagIOGroupDBItem(int id=-1,
                         const QString &groupName=QString("group1"),
                         const QString &showName=QString()
            )
        : m_id(id),
          m_szGroupName(groupName),
          m_szShowName(showName)
    {
    }

    TagIOGroupDBItem(const TagIOGroupDBItem &rh) {
        m_id = rh.m_id;
        m_szGroupName = rh.m_szGroupName;
        m_szShowName = rh.m_szShowName;
    }

    TagIOGroupDBItem &operator=(const TagIOGroupDBItem &rh) {
        m_id = rh.m_id;
        m_szGroupName = rh.m_szGroupName;
        m_szShowName = rh.m_szShowName;
        return *this;
    }

    int m_id = -1;
    QString m_szGroupName = "";
    QString m_szShowName = "";
};

#endif // DB_TAG_H
