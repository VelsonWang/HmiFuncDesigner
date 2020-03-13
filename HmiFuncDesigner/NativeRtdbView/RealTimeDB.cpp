#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include "RealTimeDB.h"
#include <cstring>
#include <QDebug>

#define MAX_DBTAGOBJECT    (5000) // 共享内存创建的最大个数

RealTimeDB::RealTimeDB(QObject *parent) :
    QObject(parent)
{
    // 为实时数据创建共享内存
    m_rtdbSharememory.setName("runtime.rtdb");
    if(m_rtdbSharememory.create(sizeof(TDBTagObject) * MAX_DBTAGOBJECT)) {
        qDebug() << "create sharememory[runtime.rtdb] success!";
    } else {
        qDebug() << "create sharememory[runtime.rtdb] fail!";
        m_pDBTagObjectBaseAddr = (PDBTagObject)m_rtdbSharememory.open();
        if(NULL == m_pDBTagObjectBaseAddr) {
            qDebug() << "get sharememory[runtime.rtdb] pointer fail!";
        } else {
            qDebug() << "get sharememory[runtime.rtdb] pointer success!";
        }
    }
}


RealTimeDB::~RealTimeDB()
{
    // 释放共享内存
    m_rtdbSharememory.destroy();
}

///
/// \brief RealTimeDB::getEmptyDBTagObject
/// \details 从共享内查找指定ID的DBTagObject对象
/// \return DBTagObject对象
///
PDBTagObject RealTimeDB::getDBTagObject(const QString &szID)
{
    m_memStatus = false;
    if(m_pDBTagObjectBaseAddr == Q_NULLPTR) return Q_NULLPTR;

    for(int i=0; i<MAX_DBTAGOBJECT; i++) {
        PDBTagObject pObj = (PDBTagObject)&m_pDBTagObjectBaseAddr[i];
        //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << szID << pObj->szID;
        if(strcasecmp(pObj->szID, szID.toStdString().c_str()) == 0) {
            m_memStatus = true;
            return pObj;
        }
    }
    return Q_NULLPTR;
}



/*
* 获取DBTagObject数据
*/
QString RealTimeDB::GetDataString(const QString &id)
{
    QString szRet = "";

    PDBTagObject pDBTagObj = getDBTagObject(id);
    if(pDBTagObj == Q_NULLPTR) return QString("");

    TTagDataType type = static_cast<TTagDataType>(pDBTagObj->iType);
    TAny any = pDBTagObj->rData;

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
void RealTimeDB::SetDataString(const QString &id, const QString &dat)
{
    PDBTagObject pDBTagObj = getDBTagObject(id);
    if(pDBTagObj == Q_NULLPTR || dat == "") return;

    TAny any;
    memset(any.t_bytes, 0, sizeof(any.t_bytes)/sizeof(quint8));
    TTagDataType type = static_cast<TTagDataType>(pDBTagObj->iType);

    switch(type) {
    case TYPE_VARIANT: {
        memcpy(any.t_bytes, dat.toStdString().c_str(), pDBTagObj->mLength);
    }break;
    case TYPE_BOOL:
    case TYPE_INT8:
    case TYPE_INT16:
    case TYPE_INT32: {
        qint32 iVal = dat.toInt();
        memcpy(any.t_bytes, (void *)&iVal, pDBTagObj->mLength);
    }break;
    case TYPE_UINT8:
    case TYPE_UINT16:
    case TYPE_UINT32: {
        quint32 iVal = dat.toUInt();
        memcpy(any.t_bytes, (void *)&iVal, pDBTagObj->mLength);
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

    pDBTagObj->rData = any;
    pDBTagObj->wData = any;
    pDBTagObj->byWriteToVendor = 1;
}

