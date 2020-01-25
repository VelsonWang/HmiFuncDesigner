#ifndef IOTAG_H
#define IOTAG_H

#include <QObject>
#include "public.h"


class IOTag : public QObject
{
    Q_OBJECT
public:
    explicit IOTag(QObject *parent = Q_NULLPTR);
    ~IOTag();

public:
    QString GetDeviceName();
    void SetDeviceName(QString s);
    TPermissionType GetPermissionType();
    void SetPermissionType(TPermissionType t);
    qint32 GetDeviceAddress();
    void SetDeviceAddress(qint32 add);
    QString GetRegisterArea();
    void SetRegisterArea(QString s);
    qint32 GetRegisterAddress();
    void SetRegisterAddress(qint32 add);
    TTagDataType GetDataType();
    qint32 GetDataTypeLength();
    void SetDataType(TTagDataType t);
    qint32 GetOffset();
    void SetOffset(qint32 o);
    qint32 GetMaxValue();
    void SetMaxValue(qint32 v);
    qint32 GetMinValue();
    void SetMinValue(qint32 v);
    qint32 GetInitializeValue();
    void SetInitializeValue(qint32 v);
    float GetScale();
    void SetScale(float v);
    qint32 GetInFrameAddress();
    void SetInFrameAddress(qint32 i);
    void* GetDBTagObject();
    void SetDBTagObject(void * p);
    // 获取所属块读变量ID
    QString getBlockReadTagId() const;
    // 设置所属块读变量ID
    void setBlockReadTagId(const QString &szID);
    // 获取块读变量读取成功或失败标志
    bool isReadBlockReadTagSuccess();
    // 设置块读变量读取成功或失败标志
    void setReadBlockReadTagSuccess(bool bFlag);
    // 设置所属块读变量
    void setBlockReadTag(IOTag* pTag);
    // 获取所属块读变量
    IOTag* getBlockReadTag();

private:
    QString m_sDeviceName;
    TPermissionType m_PermissionType;
    qint32 m_iDeviceAddress;
    QString m_sRegisterArea;
    qint32 m_iRegisterAddress;
    TTagDataType m_sDataType;
    qint32 m_iOffset;
    qint32 m_iMaxValue;
    qint32 m_iMinValue;
    qint32 m_iInitializeValue;
    float m_fScale;
    qint32 m_iInFrameAddress;
    void *m_pDBTagObject;

    QString m_szBlockReadTagId = ""; // 所属块读变量ID,用于从块读变量内存拷贝数据
    bool m_bReadBlockReadTagSuccess = false; // 块读变量 true-变量读取成功, false-变量读取失败
    IOTag* m_pObjBlockReadTag = Q_NULLPTR; // 所属块读变量

private:
    QString mId; // 变量ID

public:
    void SetTagBufferLength(qint32 i);
    void SetTagID(const QString &id);
    QString GetTagID();

    IOTag &operator=(const IOTag &);

public:
    qint32 mLength; // 长度   
    unsigned char* pReadBuf;
    unsigned char* pWriteBuf;
};

#endif // IOTAG_H
