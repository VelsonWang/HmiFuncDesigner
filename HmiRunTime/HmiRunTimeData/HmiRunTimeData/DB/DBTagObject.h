#ifndef DBTAGOBJECT_H
#define DBTAGOBJECT_H

#include <QObject>
#include <QVariant>
#include "public.h"
#include "Vendor.h"
#include "../Event/Event.h"
#include "../Tag/Tag.h"

class Vendor;

//------------------------------------------------------------------------------

struct DBTagObjectPrivate
{
    TTagType type;
    QObject *pTagObject;
};
typedef DBTagObjectPrivate TDBTagObjectPrivate;
typedef DBTagObjectPrivate* PDBTagObjectPrivate;

//------------------------------------------------------------------------------

typedef union any
{
#define MAX_LEN    (64)
    quint8 t_bool;
    qint8 t_int8;
    quint8 t_uint8;
    qint16 t_int16;
    quint16 t_uint16;
    qint32 t_int32;
    quint32 t_uint32;
    qint64 t_int64;
    quint64 t_uint64;
    float t_float32;
    double t_float64;
    quint16 t_ascii2char;
    char t_string[MAX_LEN]; // TODO 字符串长度待定
    quint32 t_bcd;
    quint8 t_bytes[MAX_LEN];

    bool operator==(const any &rhs) {
        for (int i=0; i<MAX_LEN; i++)
            if(t_bytes[i] != rhs.t_bytes[i]) return false;
        return true;
    }

    bool operator!=(const any &rhs) {
        for (int i=0; i<MAX_LEN; i++)
            if(t_bytes[i] != rhs.t_bytes[i]) return true;
        return false;
    }

#undef MAX_LEN
} __attribute__ ((packed)) TAny;

typedef struct tagDBTagObject
{
    char szID[32]; // 变量ID
    quint8 iType; // 变量类型 字节 字 双字等
    quint8 iPermission; // 读写权限
    qint32 mLength; // 长度
    quint8 byWriteToVendor; // 标记需要写入PLC，仪器等设备
    quint8 byUpdateFromVendor; // 标记已从PLC，仪器等设备采集更新
    TAny rData; // 变量数据(read from device)
    TAny wData; // 变量数据(write to device)
} __attribute__ ((packed)) TDBTagObject, *PDBTagObject;

//------------------------------------------------------------------------------

class DBTagObject : public EventProducer
{
public:
    explicit DBTagObject();
    DBTagObject(PDBTagObject pDBTagObj, QString id, TTagDataType t,
                TPermissionType p, qint32 len, TTagType tt, QObject *pObj);
    ~DBTagObject();

    void SetData(TAny v, bool bReadFromDevice = true);
    void SetData(unsigned char* buffer);
    TAny GetData();
    QByteArray GetDataBytes();
    TAny GetWriteData();
    QByteArray GetWriteDataBytes();

signals:

public slots:

public:
    PDBTagObject m_pDBTagObject = Q_NULLPTR;
    TDBTagObjectPrivate m_private; // 存放标签属性对象
    Vendor *m_pVendor;
};

class DBTagObject_Event_Listener : public Listener
{
public:
    void performed(RuntimeEvent e)
    {
        DBTagObject *pObj = (DBTagObject *)e.getObj();
        QObject *pTagObj = pObj->m_private.pTagObject;
        if(pTagObj != nullptr)
        {
            switch(pObj->m_private.type)
            {
                case TYPE_IO:
                {
                    IoDataTag *pIoDataTag = (IoDataTag *)pTagObj;
                    pIoDataTag->m_Function.ExecFunctions(e);
                }
                break;
                case TYPE_TMP:
                {
                    TmpDataTag *pTmpTag = (TmpDataTag *)pTagObj;
                    pTmpTag->m_Function.ExecFunctions(e);
                }
                break;
                case TYPE_SYSTEM:
                {
                    SysDataTag *pSysTag = (SysDataTag *)pTagObj;
                    pSysTag->m_Function.ExecFunctions(e);
                }
                break;
            }
        }
    }
};
#endif // DBTAGOBJECT_H
