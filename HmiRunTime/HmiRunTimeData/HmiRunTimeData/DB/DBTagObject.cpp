#include "DBTagObject.h"
#include "HmiRunTime.h"
#include <QDebug>

DBTagObject::DBTagObject()
{
    m_pDBTagObject = Q_NULLPTR;
    m_private.type =TYPE_TMP;
    m_private.pTagObject = Q_NULLPTR;
    m_pVendor = Q_NULLPTR;
}

DBTagObject::DBTagObject(PDBTagObject pDBTagObj, QString id, TTagDataType t,
                         TPermissionType p, qint32 len, TTagType tt, QObject *pObj)
{
    if(pDBTagObj != Q_NULLPTR) {
        m_pDBTagObject = pDBTagObj;
        strncpy(m_pDBTagObject->szID, id.toStdString().c_str(), id.length());
        m_pDBTagObject->iType = static_cast<quint8>(t);
        m_pDBTagObject->iPermission = static_cast<quint8>(p);
        m_pDBTagObject->mLength = len;
        m_pDBTagObject->byWriteToVendor = 0;
    }

    m_private.type = tt;
    m_private.pTagObject = pObj;
    m_pVendor = nullptr;
}


DBTagObject::~DBTagObject()
{
    if(m_private.pTagObject != nullptr)
        delete m_private.pTagObject;
}

void DBTagObject::SetData(TAny v, bool bReadFromDevice)
{
    if(m_pDBTagObject->rData != v) {
        // 发送更新事件到消息服务
        QString msg = QString("%1 %2").arg("VALUE_CHANGE").arg(m_pDBTagObject->szID);
        HmiRunTime::doMessage(msg);
    }
    m_pDBTagObject->rData = v;
    if(!bReadFromDevice) {
        m_pDBTagObject->wData = v;
    } else {
        RuntimeEvent event(this, VALUE_CHANGE, Q_NULLPTR);
        fireAEvent(event);
    }
}


void DBTagObject::SetData(unsigned char* buffer)
{
    TAny oldData = m_pDBTagObject->rData;
    TTagDataType type = static_cast<TTagDataType>(m_pDBTagObject->iType);

    TAny any;
    memset(any.t_bytes, 0, sizeof(any.t_bytes)/sizeof(quint8));
    memcpy(any.t_bytes, buffer, m_pDBTagObject->mLength);
    m_pDBTagObject->rData = any;

    if(m_pDBTagObject->rData != oldData) {
        // 发送更新事件到消息服务
        QString msg = QString("%1 %2").arg("VALUE_CHANGE").arg(m_pDBTagObject->szID);
        HmiRunTime::doMessage(msg);
    }
}


TAny DBTagObject::GetData()
{
    RuntimeEvent event(this, BEFORE_READ, Q_NULLPTR);
    fireAEvent(event);
    return m_pDBTagObject->rData;
}

QByteArray DBTagObject::GetDataBytes()
{
    QByteArray dat;
    dat.resize(m_pDBTagObject->mLength);
    for(int i=0; i<m_pDBTagObject->mLength; i++)
        dat.append(static_cast<char>(m_pDBTagObject->rData.t_bytes[i]));
    return dat;
}


TAny DBTagObject::GetWriteData()
{
    return m_pDBTagObject->wData;
}


QByteArray DBTagObject::GetWriteDataBytes()
{
    QByteArray dat;
    for(int i=0; i<m_pDBTagObject->mLength; i++) {
        dat.append(static_cast<char>(m_pDBTagObject->wData.t_bytes[i]));
    }
    return dat;
}




