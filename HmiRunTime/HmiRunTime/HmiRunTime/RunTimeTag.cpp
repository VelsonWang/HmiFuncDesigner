#include "RunTimeTag.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>
#include "Public/PublicFunction.h"

RunTimeTag::RunTimeTag(QObject *parent) : QObject(parent), id(0)
{
}

RunTimeTag::~RunTimeTag()
{
}


RunTimeTag::RunTimeTag(const RunTimeTag &obj)
{
    copyObject(obj);
}


RunTimeTag &RunTimeTag::operator=(const RunTimeTag &obj)
{
    copyObject(obj);
    return *this;
}

void RunTimeTag::copyObject(const RunTimeTag &obj)
{
    name = obj.name;
    unit = obj.unit;
    addrType = obj.addrType;
    addrOffset = obj.addrOffset;
    aAddrType2 = obj.aAddrType2;
    addrOffset2 = obj.addrOffset2;
    dataType = obj.dataType;
    writeable = obj.writeable;
    remark = obj.remark;
    ownGroup = obj.ownGroup;
    devType = obj.devType;
    bufLength = obj.bufLength;
    dataFromVendor = obj.dataFromVendor;
    dataToVendor = obj.dataToVendor;
}


void RunTimeTag::copyFromTag(RunTimeTag obj)
{
    name = obj.name;
    unit = obj.unit;
    addrType = obj.addrType;
    addrOffset = obj.addrOffset;
    aAddrType2 = obj.aAddrType2;
    addrOffset2 = obj.addrOffset2;
    dataType = obj.dataType;
    writeable = obj.writeable;
    remark = obj.remark;
    ownGroup = obj.ownGroup;
    devType = obj.devType;
    bufLength = obj.bufLength;
    dataFromVendor = obj.dataFromVendor;
    dataToVendor = obj.dataToVendor;
}

QString RunTimeTag::toString()
{
    QString ret = "";
    switch (dataType) {
        case TYPE_BOOL:
            bytesToBool(dataFromVendor) ? ret = "true" : "false";
            break;
        case TYPE_INT8:
            ret = QString::number(bytesToInt8(dataFromVendor));
            break;
        case TYPE_UINT8:
            ret = QString::number(bytesToUint8(dataFromVendor));
            break;
        case TYPE_INT16:
            ret = QString::number(bytesToInt16(dataFromVendor));
            break;
        case TYPE_UINT16:
            ret = QString::number(bytesToUint16(dataFromVendor));
            break;
        case TYPE_INT32:
            ret = QString::number(bytesToInt32(dataFromVendor));
            break;
        case TYPE_UINT32:
            ret = QString::number(bytesToUint32(dataFromVendor));
            break;
        case TYPE_INT64:
            ret = QString::number(bytesToInt64(dataFromVendor));
            break;
        case TYPE_UINT64:
            ret = QString::number(bytesToUint64(dataFromVendor));
            break;
        case TYPE_FLOAT32:
            ret = QString::number(bytesToFloat32(dataFromVendor));
            break;
        case TYPE_FLOAT64:
            ret = QString::number(bytesToFloat64(dataFromVendor));
            break;

        case TYPE_BCD8:
            ret = QString::number(bytesToBcd8(dataFromVendor));
            break;
        case TYPE_BCD16:
            ret = QString::number(bytesToBcd16(dataFromVendor));
            break;
        case TYPE_BCD32:
            ret = QString::number(bytesToBcd32(dataFromVendor));
            break;
        case TYPE_ASCII2CHAR:
            ret = QString(dataFromVendor);
            break;
        case TYPE_STRING:
            ret = QString(dataFromVendor);
            break;
        case TYPE_BYTES:
            ret = QString(dataFromVendor);
            break;
        default:
            break;
    };
    return ret;
}
