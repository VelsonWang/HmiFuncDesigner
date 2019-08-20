#ifndef DBTAGOBJECT_H
#define DBTAGOBJECT_H

#include <QObject>
#include <QVariant>
#include "public.h"
#include "IVendor.h"
#include "../Event/Event.h"
#include "../Tag/Tag.h"

class IVendor;

struct DBTagObjectPrivate
{
    TTagType type;
    QObject *pTagObject;
};
typedef DBTagObjectPrivate TDBTagObjectPrivate;

////////////////////////////////////////////
/// \brief The DBTagObject class
///
class DBTagObject : public EventProducer
{
public:
    explicit DBTagObject();
    DBTagObject(QString id, TTagDataType t, TPermissionType p, qint32 len,
                QVariant dat, TTagType tt, QObject *pObj);
    ~DBTagObject();

    void SetData(QVariant v, bool bReadFromDevice = true);
    void SetData(unsigned char* buffer);
    QVariant GetData();
    QByteArray GetDataBytes();
    QVariant GetWriteData();
    QByteArray GetWriteDataBytes();

signals:

public slots:

public:
    QString mId; // 变量ID
    TTagDataType mType; // 变量类型 字节 字 双字等
    TPermissionType mPermission; // 读写权限
    qint32 mLength; // 长度
    TDBTagObjectPrivate mPrivate; // 存放标签属性对象
    IVendor *pVendor;

private:
    QVariant mData; // 变量数据(read from device)
    QVariant mDataWrite; // 变量数据(write to device)
};

class DBTagObject_Event_Listener : public Listener
{
public:
    void performed(RuntimeEvent e)
    {
        DBTagObject *pObj = (DBTagObject *)e.getObj();
        QObject *pTagObj = pObj->mPrivate.pTagObject;
        if(pTagObj != nullptr)
        {
            switch(pObj->mPrivate.type)
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
