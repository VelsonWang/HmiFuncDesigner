#include "DBTagObject.h"
#include "SCADARunTime.h"


DBTagObject::DBTagObject()
{
    mId = 0;
    mType = TYPE_BOOL;
    mPermission = READ_WRIE;
    mLength = 0;
    mData = 0;
    mPrivate.type =TYPE_TMP;
    mPrivate.pTagObject = NULL;
    pVendor = NULL;
}

DBTagObject::DBTagObject(qint32 id, TTagDataType t, TPermissionType p, qint32 len,
                         QVariant dat, TTagType tt, QObject *pObj)
{
    mId = id;
    mType = t;
    mPermission = p;
    mLength = len;
    mData = dat;
    mPrivate.type = tt;
    mPrivate.pTagObject = pObj;
    pVendor = NULL;
}


DBTagObject::~DBTagObject()
{
    if(mPrivate.pTagObject != NULL)
        delete mPrivate.pTagObject;
}

void DBTagObject::SetData(QVariant v, bool bReadFromDevice)
{
    if(mData != v)
    {
        // 发送更新事件到消息服务
        QString msg = QString("%1 %2").arg("VALUE_CHANGE").arg(this->mId);
        SCADARunTime::doMessage(msg);
    }
    mData = v;
    if(!bReadFromDevice)
    {       
        mDataWrite = v;
    }
    else
    {
        RuntimeEvent event(this, VALUE_CHANGE, mData);
        fireAEvent(event);
    }    
}

QVariant DBTagObject::GetData()
{
    RuntimeEvent event(this, BEFORE_READ, mData);
    fireAEvent(event);
    return mData;
}

QVariant DBTagObject::GetWriteData()
{
    return mDataWrite;
}

