#include "RunTimeTag.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>


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

