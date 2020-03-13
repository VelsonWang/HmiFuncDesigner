#include <QDateTime>
#include <QDate>
#include <QTime>
#include "RealTimeDB.h"
#include "log4qt/logger.h"
#include "Log.h"
#include <QDebug>

#define MAX_DBTAGOBJECT    (5000) // 共享内存创建的最大个数


RealTimeDB::RealTimeDB(QObject *parent) :
    QObject(parent)
{
    // 为实时数据创建共享内存
    m_rtdbSharememory.setName("runtime.rtdb");
    if(m_rtdbSharememory.create(sizeof(TDBTagObject) * MAX_DBTAGOBJECT)) {
        LogInfo("create sharememory[runtime.rtdb] success!");
        m_pDBTagObjectBaseAddr = (PDBTagObject)m_rtdbSharememory.open();
        if(NULL == m_pDBTagObjectBaseAddr) {
            LogError("get sharememory[runtime.rtdb] pointer fail!");
            m_bMemStatus = false;
        } else {
            LogInfo("get sharememory[runtime.rtdb] pointer success!");
            for(int i=0; i<MAX_DBTAGOBJECT; i++) {
                memset(&m_pDBTagObjectBaseAddr[i], 0, sizeof(TDBTagObject));
            }
            LogInfo("clear sharememory[runtime.rtdb] success!");
        }
    } else {
        LogError("create sharememory[runtime.rtdb] fail!");
        m_bMemStatus = false;
    }
}


RealTimeDB::~RealTimeDB()
{
    // 释放共享内存
    m_rtdbSharememory.destroy();
}

///
/// \brief RealTimeDB::getEmptyDBTagObject
/// \details 从共享内存取一个未使用的DBTagObject对象
/// \return DBTagObject对象
///
PDBTagObject RealTimeDB::getEmptyDBTagObject()
{
    for(int i=0; i<MAX_DBTAGOBJECT; i++) {
        PDBTagObject pObj = (PDBTagObject)&m_pDBTagObjectBaseAddr[i];
        quint32 iSum = 0;
        quint8 *pData = (quint8 *)pObj;
        for(int j=0; j<sizeof(TDBTagObject); j++) {
            iSum += pData[j];
        }
        if(iSum == 0) {
            //LogInfo(QString::asprintf("%s base address: %p", __func__, pObj));
            return pObj;
        }
    }
    return Q_NULLPTR;
}


///
/// \brief RealTimeDB::getEmptyDBTagObject
/// \details 从共享内查找指定ID的DBTagObject对象
/// \return DBTagObject对象
///
PDBTagObject RealTimeDB::getDBTagObject(const QString &szID)
{
    m_bMemStatus = false;
    if(m_pDBTagObjectBaseAddr == Q_NULLPTR) return Q_NULLPTR;

    for(int i=0; i<MAX_DBTAGOBJECT; i++) {
        PDBTagObject pObj = (PDBTagObject)&m_pDBTagObjectBaseAddr[i];
        //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << szID << pObj->szID;
        if(strcasecmp(pObj->szID, szID.toStdString().c_str()) == 0) {
            m_bMemStatus = true;
            return pObj;
        }
    }
    return Q_NULLPTR;
}


/*
* 获取DBTagObject数据
*/
TAny RealTimeDB::GetData(const QString &id)
{
    TAny any;
    DBTagObject* pTag = rtdb[id];
    if(pTag != nullptr)
        return pTag->GetData();
    return any;
}


/*
* 获取DBTagObject数据
*/
QString RealTimeDB::GetDataString(const QString &id)
{
    QString szRet = "";

    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr)
        return QString("");

    TTagDataType type = static_cast<TTagDataType>(pTag->m_pDBTagObject->iType);
    TAny any = pTag->GetData();

    switch(type) {
    case TYPE_VARIANT: {
        szRet = QString((char *)any.t_bytes);
    }break;
    case TYPE_BOOL: {
        szRet = QString::number(any.t_bool);
    }break;
    case TYPE_INT8: {
        szRet = QString::number(any.t_int8);
    }break;
    case TYPE_INT16: {
        szRet = QString::number(any.t_int16);
    }break;
    case TYPE_INT32: {
        szRet = QString::number(any.t_int32);
    }break;
    case TYPE_UINT8:{
        szRet = QString::number(any.t_uint8);
    }break;
    case TYPE_UINT16:{
        szRet = QString::number(any.t_uint16);
    }break;
    case TYPE_UINT32: {
        szRet = QString::number(any.t_uint32);
    }break;
    case TYPE_INT64: {
        szRet = QString::number(any.t_int64);
    }break;
    case TYPE_UINT64: {
        szRet = QString::number(any.t_uint64);
    }break;
    case TYPE_FLOAT: {
        szRet = QString::number(static_cast<double>(any.t_float32));
    }break;
    case TYPE_DOUBLE: {
        szRet = QString::number(any.t_float64);
    }break;
    case TYPE_ASCII2CHAR: {
        szRet = QString::number(any.t_uint16);
    }break;
    case TYPE_STRING: {
        szRet = QString((char *)any.t_string);
    }break;
    case TYPE_BCD: {
        szRet = QString::number(any.t_uint32);
    }break;
    default: {

    }break;
    }
    return szRet;
}

/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetData(const QString &id, TAny dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr) return;

    pTag->SetData(dat, false);

    if(pTag->m_pVendor != nullptr) {
        IOTag *pIOTag = pTag->m_pVendor->findIOTagByID(id);
        pTag->m_pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 设置DBTagObject类型和数据
*/
void RealTimeDB::SetTypeAndData(const QString &id, TTagDataType type, TAny dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag != nullptr)
    {
        pTag->SetData(dat, false);
        pTag->m_pDBTagObject->iType = type;
        if(pTag->m_pVendor != nullptr) {
            IOTag *pIOTag = pTag->m_pVendor->findIOTagByID(id);
            pTag->m_pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
        }
    }
}


/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetDataString(const QString &id, const QString &dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr || dat == "") return;

    TAny any;
    memset(any.t_bytes, 0, sizeof(any.t_bytes)/sizeof(quint8));
    TTagDataType type = static_cast<TTagDataType>(pTag->m_pDBTagObject->iType);
    switch(type) {
    case TYPE_VARIANT: {
        memcpy(any.t_bytes, dat.toStdString().c_str(), pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_BOOL:
    case TYPE_INT8:
    case TYPE_INT16:
    case TYPE_INT32: {
        qint32 iVal = dat.toInt();
        memcpy(any.t_bytes, (void *)&iVal, pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_UINT8:
    case TYPE_UINT16:
    case TYPE_UINT32: {
        quint32 iVal = dat.toUInt();
        memcpy(any.t_bytes, (void *)&iVal, pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_INT64: {
        any.t_int64 = dat.toLongLong();
    }break;
    case TYPE_UINT64: {
        any.t_uint64 = dat.toULongLong();
    }break;
    case TYPE_FLOAT: {
        any.t_float32 = dat.toFloat();
    }break;
    case TYPE_DOUBLE: {
        any.t_float64 = dat.toDouble();
    }break;
    case TYPE_ASCII2CHAR: {
        any.t_ascii2char = static_cast<quint16>(dat.toUInt());
    }break;
    case TYPE_STRING: {
        strncpy(any.t_string, dat.toStdString().c_str(), dat.length());
    }break;
    case TYPE_BCD: {
        any.t_bcd = static_cast<quint32>(dat.toUInt());
    }break;
    default: {

    }break;
    }
    pTag->SetData(any, false);

    if(pTag->m_pVendor != Q_NULLPTR) {
        IOTag *pIOTag = pTag->m_pVendor->findIOTagByID(id);
        pTag->m_pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 内部设置DBTagObject数据
*/
void RealTimeDB::SetDataStringInner(const QString &id, const QString &dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr || dat == "") return;

    TAny any;
    memset(any.t_bytes, 0, sizeof(any.t_bytes)/sizeof(quint8));
    TTagDataType type = static_cast<TTagDataType>(pTag->m_pDBTagObject->iType);
    switch(type) {
    case TYPE_VARIANT: {
        memcpy(any.t_bytes, dat.toStdString().c_str(), pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_BOOL:
    case TYPE_INT8:
    case TYPE_INT16:
    case TYPE_INT32: {
        qint32 iVal = dat.toInt();
        memcpy(any.t_bytes, (void *)&iVal, pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_UINT8:
    case TYPE_UINT16:
    case TYPE_UINT32: {
        quint32 iVal = dat.toUInt();
        memcpy(any.t_bytes, (void *)&iVal, pTag->m_pDBTagObject->mLength);
    }break;
    case TYPE_INT64: {
        any.t_int64 = dat.toLongLong();
    }break;
    case TYPE_UINT64: {
        any.t_uint64 = dat.toULongLong();
    }break;
    case TYPE_FLOAT: {
        any.t_float32 = dat.toFloat();
    }break;
    case TYPE_DOUBLE: {
        any.t_float64 = dat.toDouble();
    }break;
    case TYPE_ASCII2CHAR: {
        any.t_ascii2char = static_cast<quint16>(dat.toUInt());
    }break;
    case TYPE_STRING: {
        strncpy(any.t_string, dat.toStdString().c_str(), dat.length());
    }break;
    case TYPE_BCD: {
        any.t_bcd = static_cast<quint32>(dat.toUInt());
    }break;
    default: {

    }break;
    }
    pTag->SetData(any, false);
}

QString RealTimeDB::getIdByTagName(const QString &name)
{
    QString ret = "";
    ret = varNameMapId.value(name, "");
    return ret;
}

void RealTimeDB::debug()
{
    qDebug() << "RealTimeDB: ";
    QMap<QString, DBTagObject* >::iterator iter = rtdb.begin();
    for(iter=rtdb.begin(); iter!=rtdb.end(); ++iter) {
        qDebug() << iter.key() << ' ' << RealTimeDB::GetDataString(iter.key());
    }
}

void RealTimeDB::debugShowNameMapId()
{
    qDebug() << "Tag Name Map Id: ";
    QMap<QString, QString>::iterator iter = varNameMapId.begin();
    for(iter=varNameMapId.begin(); iter!=varNameMapId.end(); ++iter) {
        qDebug() << iter.key() << ' ' << iter.value();
    }
}

///
/// \brief RealTimeDB::addNeedWriteTagToDeviceWriteQueue
/// \details 添加需要写入PLC或仪器的变量至相应设备节点写队列
///
void RealTimeDB::addNeedWriteTagToDeviceWriteQueue()
{
    QMap<QString, DBTagObject* >::iterator iter = rtdb.begin();
    for(iter=rtdb.begin(); iter!=rtdb.end(); ++iter) {
        QString szID = iter.key();
        DBTagObject* pTagObj = iter.value();
        if(pTagObj != Q_NULLPTR) {
            PDBTagObject pDBTagObject = pTagObj->m_pDBTagObject;
            if(pDBTagObject != Q_NULLPTR) {
                if(pDBTagObject->byWriteToVendor) {
                    if(pTagObj->m_pVendor != nullptr) {
                        IOTag *pIOTag = pTagObj->m_pVendor->findIOTagByID(szID);
                        pTagObj->m_pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
                    }
                    pDBTagObject->byWriteToVendor = false;
                }
            }
        }
    }
}
