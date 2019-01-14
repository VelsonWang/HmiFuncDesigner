
#include <QDateTime>
#include <QDate>
#include <QTime>
#include "RealTimeDB.h"
#include <QDebug>


QMap<qint32, DBTagObject* > RealTimeDB::rtdb = QMap<qint32, DBTagObject* >();
QMap<QString, qint32> RealTimeDB::varNameMapId = QMap<QString, qint32>();

RealTimeDB::RealTimeDB(QObject *parent) :
    QObject(parent)
{

}


RealTimeDB::~RealTimeDB()
{
    qDeleteAll(rtdb);
    rtdb.clear();

}


/*
* 获取DBTagObject数据
*/
QVariant RealTimeDB::GetData(qint32 id)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag != NULL)
        return pTag->GetData();
    return NULL;
}


/*
* 获取DBTagObject数据
*/
QString RealTimeDB::GetDataString(qint32 id)
{
    QString ret = "";

    DBTagObject* pTag = rtdb[id];
    if(pTag == NULL)
        return QString("");

    TTagDataType type = pTag->mType;
    QVariant val = pTag->GetData();
    switch(type)
    {
        case TYPE_VARIANT:
        {
            ret = val.toString();
        }break;
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_INT16:
        case TYPE_INT32:
        {
            ret = QString::number(val.toInt());
        }break;
        case TYPE_UINT8:
        case TYPE_UINT16:
        case TYPE_UINT32:
        {
            ret = QString::number(val.toUInt());
        }break;
        case TYPE_INT64:
        {
            ret = QString::number(val.toLongLong());
        }break;
        case TYPE_UINT64:
        {
            ret = QString::number(val.toULongLong());
        }break;
        case TYPE_FLOAT:
        {
            ret = QString::number(val.toFloat());
        }break;
        case TYPE_DOUBLE:
        {
            ret = QString::number(val.toDouble());
        }break;
        case TYPE_ASCII2CHAR:
        {
            ret = QString::number(val.toUInt());
        }break;
        case TYPE_STRING:
        {
            ret = val.toString();
        }break;
        case TYPE_BCD:
        {
            val.toString();
        }break;
    }
    return ret;
}

/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetData(qint32 id, QVariant dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == NULL)
        return;

    pTag->SetData(dat, false);

    if(pTag->pVendor != NULL)
    {
        IOTag *pIOTag = pTag->pVendor->FindIOTagByID(id);
        pTag->pVendor->AddIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 设置DBTagObject类型和数据
*/
void RealTimeDB::SetTypeAndData(qint32 id, TTagDataType type, QVariant dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag != NULL)
    {
        pTag->SetData(dat, false);
        pTag->mType = type;
        if(pTag->pVendor != NULL)
        {
            IOTag *pIOTag = pTag->pVendor->FindIOTagByID(id);
            pTag->pVendor->AddIOTagToDeviceTagWriteQueue(pIOTag);
        }
    }
}


/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetDataString(qint32 id, QString dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == NULL || dat == "")
        return;

    TTagDataType type = pTag->mType;
    QVariant val;
    switch(type)
    {
        case TYPE_VARIANT:
        {
            val = QVariant(dat);
        }break;
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_INT16:
        case TYPE_INT32:
        {
            val = QVariant(dat.toInt());
        }break;
        case TYPE_UINT8:
        case TYPE_UINT16:
        case TYPE_UINT32:
        {
            val = QVariant(dat.toUInt());
        }break;
        case TYPE_INT64:
        {
            val = QVariant(dat.toLongLong());
        }break;
        case TYPE_UINT64:
        {
            val = QVariant(dat.toULongLong());
        }break;
        case TYPE_FLOAT:
        {
            val = QVariant(dat.toFloat());
        }break;
        case TYPE_DOUBLE:
        {
            val = QVariant(dat.toDouble());
        }break;
        case TYPE_ASCII2CHAR:
        {
            val = QVariant(dat.toUInt());
        }break;
        case TYPE_STRING:
        {
            val = QVariant(dat);
        }break;
        case TYPE_BCD:
        {
            val = QVariant(dat);
        }break;
    }
    pTag->SetData(val, false);

    if(pTag->pVendor != NULL)
    {
        IOTag *pIOTag = pTag->pVendor->FindIOTagByID(id);
        pTag->pVendor->AddIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 内部设置DBTagObject数据
*/
void RealTimeDB::SetDataStringInner(qint32 id, QString dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == NULL || dat == "")
        return;

    TTagDataType type = pTag->mType;
    QVariant val;
    switch(type)
    {
        case TYPE_VARIANT:
        {
            val = QVariant(dat);
        }break;
        case TYPE_BOOL:
        case TYPE_INT8:
        case TYPE_INT16:
        case TYPE_INT32:
        {
            val = QVariant(dat.toInt());
        }break;
        case TYPE_UINT8:
        case TYPE_UINT16:
        case TYPE_UINT32:
        {
            val = QVariant(dat.toUInt());
        }break;
        case TYPE_INT64:
        {
            val = QVariant(dat.toLongLong());
        }break;
        case TYPE_UINT64:
        {
            val = QVariant(dat.toULongLong());
        }break;
        case TYPE_FLOAT:
        {
            val = QVariant(dat.toFloat());
        }break;
        case TYPE_DOUBLE:
        {
            val = QVariant(dat.toDouble());
        }break;
        case TYPE_ASCII2CHAR:
        {
            val = QVariant(dat.toUInt());
        }break;
        case TYPE_STRING:
        {
            val = QVariant(dat);
        }break;
        case TYPE_BCD:
        {
            val = QVariant(dat);
        }break;
    }
    pTag->SetData(val, false);
}

int RealTimeDB::getIdByTagName(const QString name)
{
    int ret = -1;
    ret = varNameMapId.value(name, -1);
    return ret;
}

void RealTimeDB::debug()
{
    qDebug() << "RealTimeDB: ";
    QMap<qint32, DBTagObject* >::iterator iter = rtdb.begin();
    for(iter=rtdb.begin(); iter!=rtdb.end(); ++iter)
    {
        qDebug() << iter.key() << ' ' << RealTimeDB::GetDataString(iter.key());
    }
}

void RealTimeDB::debugShowNameMapId()
{
    qDebug() << "Tag Name Map Id: ";
    QMap<QString, qint32>::iterator iter = varNameMapId.begin();
    for(iter=varNameMapId.begin(); iter!=varNameMapId.end(); ++iter)
    {
        qDebug() << iter.key() << ' ' << iter.value();
    }
}
