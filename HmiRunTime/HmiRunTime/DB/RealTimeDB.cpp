#include <QDateTime>
#include <QDate>
#include <QTime>
#include "RealTimeDB.h"
#include <QDebug>


QMap<QString, DBTagObject* > RealTimeDB::rtdb = QMap<QString, DBTagObject* >();
QMap<QString, QString> RealTimeDB::varNameMapId = QMap<QString, QString>();

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
QVariant RealTimeDB::GetData(const QString &id)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag != nullptr)
        return pTag->GetData();
    return QVariant();
}


/*
* 获取DBTagObject数据
*/
QString RealTimeDB::GetDataString(const QString &id)
{
    QString ret = "";

    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr)
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
        default:
        {

        }break;
    }
    return ret;
}

/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetData(const QString &id, QVariant dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr)
        return;

    pTag->SetData(dat, false);

    if(pTag->pVendor != nullptr)
    {
        IOTag *pIOTag = pTag->pVendor->findIOTagByID(id);
        pTag->pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 设置DBTagObject类型和数据
*/
void RealTimeDB::SetTypeAndData(const QString &id, TTagDataType type, QVariant dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag != nullptr)
    {
        pTag->SetData(dat, false);
        pTag->mType = type;
        if(pTag->pVendor != nullptr)
        {
            IOTag *pIOTag = pTag->pVendor->findIOTagByID(id);
            pTag->pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
        }
    }
}


/*
* 设置DBTagObject数据
*/
void RealTimeDB::SetDataString(const QString &id, const QString &dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr || dat == "")
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
        default:
        {

        }break;
    }
    pTag->SetData(val, false);

    if(pTag->pVendor != Q_NULLPTR)
    {
        IOTag *pIOTag = pTag->pVendor->findIOTagByID(id);      
        pTag->pVendor->addIOTagToDeviceTagWriteQueue(pIOTag);
    }
}


/*
* 内部设置DBTagObject数据
*/
void RealTimeDB::SetDataStringInner(const QString &id, const QString &dat)
{
    DBTagObject* pTag = rtdb[id];
    if(pTag == nullptr || dat == "")
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
        default:
        {

        }break;
    }
    pTag->SetData(val, false);
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
    for(iter=rtdb.begin(); iter!=rtdb.end(); ++iter)
    {
        qDebug() << iter.key() << ' ' << RealTimeDB::GetDataString(iter.key());
    }
}

void RealTimeDB::debugShowNameMapId()
{
    qDebug() << "Tag Name Map Id: ";
    QMap<QString, QString>::iterator iter = varNameMapId.begin();
    for(iter=varNameMapId.begin(); iter!=varNameMapId.end(); ++iter)
    {
        qDebug() << iter.key() << ' ' << iter.value();
    }
}
