#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMutexLocker>
#include <QHashIterator>
#include "realtimedb.h"
#include "publicfunction.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <limits>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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


bool RunTimeTag::fromString(const QString &dat, bool syncToVendor)
{
    bool ret = true;
    QByteArray val;
    switch (dataType) {
        case TYPE_BOOL:
            val = boolToBytes(dat.toUInt());
            break;
        case TYPE_INT8:
            val = int8ToBytes(dat.toInt());
            break;
        case TYPE_UINT8:
            val = uint8ToBytes(dat.toUInt());
            break;
        case TYPE_INT16:
            val = int16ToBytes(dat.toInt());
            break;
        case TYPE_UINT16:
            val = uint16ToBytes(dat.toUInt());
            break;
        case TYPE_INT32:
            val = int32ToBytes(dat.toLong());
            break;
        case TYPE_UINT32:
            val = uint32ToBytes(dat.toULong());
            break;
        case TYPE_INT64:
            val = int64ToBytes(dat.toLongLong());
            break;
        case TYPE_UINT64:
            val = uint64ToBytes(dat.toULongLong());
            break;
        case TYPE_FLOAT32:
            val = float32ToBytes(dat.toFloat());
            break;
        case TYPE_FLOAT64:
            val = float64ToBytes(dat.toDouble());
            break;
        case TYPE_BCD8:
            val = bcd8ToBytes(dat.toUInt());
            break;
        case TYPE_BCD16:
            val = bcd16ToBytes(dat.toUInt());
            break;
        case TYPE_BCD32:
            val = bcd32ToBytes(dat.toULong());
            break;
        case TYPE_ASCII2CHAR:
            val = dat.toLatin1();
            break;
        case TYPE_STRING:
            val = dat.toLatin1();
            break;
        case TYPE_BYTES:
            val = dat.toLatin1();
            break;
        default:
            break;
    };
    dataFromVendor.replace(0, val.length(), val);
    if(syncToVendor) {
        dataToVendor.replace(0, val.length(), val);
    }
    return ret;
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

bool RunTimeTag::toBool()
{
    bool ret = false;
    switch (dataType) {
        case TYPE_BOOL:
            ret = bytesToBool(dataFromVendor);
            break;
        case TYPE_INT8:
            ret = (bytesToInt8(dataFromVendor) != 0);
            break;
        case TYPE_UINT8:
            ret = (bytesToUint8(dataFromVendor) != 0);
            break;
        case TYPE_INT16:
            ret = (bytesToInt16(dataFromVendor) != 0);
            break;
        case TYPE_UINT16:
            ret = (bytesToUint16(dataFromVendor) != 0);
            break;
        case TYPE_INT32:
            ret = (bytesToInt32(dataFromVendor) != 0);
            break;
        case TYPE_UINT32:
            ret = (bytesToUint32(dataFromVendor) != 0);
            break;
        case TYPE_INT64:
            ret = (bytesToInt64(dataFromVendor) != 0);
            break;
        case TYPE_UINT64:
            ret = (bytesToUint64(dataFromVendor) != 0);
            break;
        case TYPE_FLOAT32:
            ret = (bytesToFloat32(dataFromVendor) != 0);
            break;
        case TYPE_FLOAT64:
            ret = (bytesToFloat64(dataFromVendor) != 0);
            break;
        case TYPE_BCD8:
            ret = (bytesToBcd8(dataFromVendor) != 0);
            break;
        case TYPE_BCD16:
            ret = (bytesToBcd16(dataFromVendor) != 0);
            break;
        case TYPE_BCD32:
            ret = (bytesToBcd32(dataFromVendor) != 0);
            break;
        case TYPE_ASCII2CHAR:
            ret = !QString(dataFromVendor).isEmpty();
            break;
        case TYPE_STRING:
            ret = !QString(dataFromVendor).isEmpty();
            break;
        case TYPE_BYTES:
            ret = !QString(dataFromVendor).isEmpty();
            break;
        default:
            break;
    };
    return ret;
}

// 字符串转指定类型值
void rtTagStringToValue(TTagDataType iDataType, quint8* pBuf, char* pData)
{
    if(iDataType == TYPE_BOOL) {
        quint8 iValue = atoi(pData);
        memcpy(pBuf, (void *)&iValue, 1);
    }
    else if(iDataType == TYPE_INT8) {
        qint8 iValue = atoi(pData);
        memcpy(pBuf, (void *)&iValue, 1);
    }
    else if(iDataType == TYPE_UINT8) {
        quint8 iValue = atoi(pData);
        memcpy(pBuf, (void *)&iValue, 1);
    }
    else if(iDataType == TYPE_BCD8) {
        quint8 iValue = (quint8)strtoul(pData, NULL, 16);
        memcpy(pBuf, (void *)&iValue, 1);
    }
    else if(iDataType == TYPE_INT16) {
        qint16 iValue = atoi(pData);
        memcpy(pBuf, (void *)&iValue, 2);
    }
    else if(iDataType == TYPE_UINT16) {
        quint16 iValue = atoi(pData);
        memcpy(pBuf, (void *)&iValue, 2);
    }
    else if(iDataType == TYPE_BCD16) {
        quint16 iValue = (quint16)strtoul(pData, NULL, 16);
        memcpy(pBuf, (void *)&iValue, 2);
    }
    else if(iDataType == TYPE_INT32) {
        qint32 iValue = (qint32)strtol(pData, NULL, 10);
        memcpy(pBuf, (void *)&iValue, 4);
    }
    else if(iDataType == TYPE_UINT32) {
        quint32 iValue = (quint32)strtoul(pData, NULL, 10);
        memcpy(pBuf, (void *)&iValue, 4);
    }
    else if(iDataType == TYPE_BCD32) {
        quint32 iValue = (quint32)strtoul(pData, NULL, 16);
        memcpy(pBuf, (void *)&iValue, 4);
    }
    else if(iDataType == TYPE_FLOAT32) {
        float fValue = atof(pData);
        memcpy(pBuf, (void *)&fValue, 4);
    }
    else if(iDataType == TYPE_FLOAT64) {
        double dValue = strtod(pData, NULL);
        memcpy(pBuf, (void *)&dValue, 8);
    }
    else {
    }
}


// 指定类型值转字符串
char* rtTagValueToString(TTagDataType iDataType, quint8* pBuf)
{
    char buf[256] = {0};

    if(iDataType == TYPE_BOOL) {
        quint8 iValue = 0;
        memcpy((void *)&iValue, pBuf, 1);
        sprintf((char *)buf, "%u", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_INT8) {
        qint8 iValue = 0;
        memcpy((void *)&iValue, pBuf, 1);
        sprintf((char *)buf, "%d", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_UINT8) {
        quint8 iValue = 0;
        memcpy((void *)&iValue, pBuf, 1);
        sprintf((char *)buf, "%u", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_BCD8) {
        quint8 iValue = 0;
        memcpy((void *)&iValue, pBuf, 1);
        sprintf((char *)buf, "%-2X", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_INT16) {
        qint16 iValue = 0;
        memcpy((void *)&iValue, pBuf, 2);
        sprintf((char *)buf, "%d", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_UINT16) {
        quint16 iValue = 0;
        memcpy((void *)&iValue, pBuf, 2);
        sprintf((char *)buf, "%u", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_BCD16) {
        quint16 iValue = 0;
        memcpy((void *)&iValue, pBuf, 2);
        sprintf((char *)buf, "%-4X", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_INT32) {
        qint32 iValue = 0;
        memcpy((void *)&iValue, pBuf, 4);
        sprintf((char *)buf, "%d", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_UINT32) {
        quint32 iValue = 0;
        memcpy((void *)&iValue, pBuf, 4);
        sprintf((char *)buf, "%u", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_BCD32) {
        quint32 iValue = 0;
        memcpy((void *)&iValue, pBuf, 4);
        sprintf((char *)buf, "%-8X", iValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_FLOAT32) {
        float fValue = 0;
        memcpy((void *)&fValue, pBuf, 4);
        sprintf((char *)buf, "%f", fValue);
        return strdup((const char *)buf);
    }
    else if(iDataType == TYPE_FLOAT64) {
        double dValue = 0;
        memcpy((void *)&dValue, pBuf, 8);
        gcvt(dValue, 32, buf);
        return strdup((const char *)buf);
    }
    else {
    }

    return NULL;
}

QString RunTimeTag::minString()
{
    QString ret = "";
    switch (dataType) {
        case TYPE_BOOL:
            ret = "0";
            break;
        case TYPE_INT8:
            ret = QString::number(std::numeric_limits<int8_t>::min());
            break;
        case TYPE_UINT8:
            ret = QString::number(std::numeric_limits<uint8_t>::min());
            break;
        case TYPE_INT16:
            ret = QString::number(std::numeric_limits<int16_t>::min());
            break;
        case TYPE_UINT16:
            ret = QString::number(std::numeric_limits<uint16_t>::min());
            break;
        case TYPE_INT32:
            ret = QString::number(std::numeric_limits<int32_t>::min());
            break;
        case TYPE_UINT32:
            ret = QString::number(std::numeric_limits<uint32_t>::min());
            break;
        case TYPE_INT64:
            ret = QString::number(std::numeric_limits<int64_t>::min());
            break;
        case TYPE_UINT64:
            ret = QString::number(std::numeric_limits<uint64_t>::min());
            break;
        case TYPE_FLOAT32:
            ret = QString::number(std::numeric_limits<float>::min());
            break;
        case TYPE_FLOAT64:
            ret = QString::number(std::numeric_limits<double>::min());
            break;
        case TYPE_BCD8:
            ret = QString("0");
            break;
        case TYPE_BCD16:
            ret = QString("0");
            break;
        case TYPE_BCD32:
            ret = QString("0");
            break;
        case TYPE_ASCII2CHAR:
            ret = QString("0");
            break;
        case TYPE_STRING:
            ret = QString("0");
            break;
        case TYPE_BYTES:
            ret = QString("0");
            break;
        default:
            break;
    };
    return ret;
}

QString RunTimeTag::maxString()
{
    QString ret = "";
    switch (dataType) {
        case TYPE_BOOL:
            ret = "1";
            break;
        case TYPE_INT8:
            ret = QString::number(std::numeric_limits<int8_t>::max());
            break;
        case TYPE_UINT8:
            ret = QString::number(std::numeric_limits<uint8_t>::max());
            break;
        case TYPE_INT16:
            ret = QString::number(std::numeric_limits<int16_t>::max());
            break;
        case TYPE_UINT16:
            ret = QString::number(std::numeric_limits<uint16_t>::max());
            break;
        case TYPE_INT32:
            ret = QString::number(std::numeric_limits<int32_t>::max());
            break;
        case TYPE_UINT32:
            ret = QString::number(std::numeric_limits<uint32_t>::max());
            break;
        case TYPE_INT64:
            ret = QString::number(std::numeric_limits<int64_t>::max());
            break;
        case TYPE_UINT64:
            ret = QString::number(std::numeric_limits<uint64_t>::max());
            break;
        case TYPE_FLOAT32:
            ret = QString::number(std::numeric_limits<float>::max());
            break;
        case TYPE_FLOAT64:
            ret = QString::number(std::numeric_limits<double>::max());
            break;
        case TYPE_BCD8:
            ret = QString(std::numeric_limits<uint8_t>::max());
            break;
        case TYPE_BCD16:
            ret = QString(std::numeric_limits<uint16_t>::max());
            break;
        case TYPE_BCD32:
            ret = QString(std::numeric_limits<uint32_t>::max());
            break;
        case TYPE_ASCII2CHAR:
            ret = QString(std::numeric_limits<uint16_t>::max());
            break;
        case TYPE_STRING:
            ret = QString("0");
            break;
        case TYPE_BYTES:
            ret = QString("0");
            break;
        default:
            break;
    };
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// VIP
/// 声明共享数据段, 解决不同动态链接库以及动态链接库和程序间指针不一致问题
///
#ifdef _MSC_VER  // vc
    #pragma data_seg("MyShared")
    void *g_pRtdbObj = NULL;
    #pragma data_seg()
    #pragma comment(linker, "/SECTION:MyShared,RWS")
#else // gcc
    void *g_pRtdbObj __attribute__ ((section("MyShared"))) = NULL;
#endif

QMutex RealTimeDB::m_mutex;

RealTimeDB *RealTimeDB::instance()
{
    QMutexLocker locker(&m_mutex);
    if(g_pRtdbObj == NULL) {
        g_pRtdbObj = new RealTimeDB();
    }
    return (RealTimeDB *)g_pRtdbObj;
}

RealTimeDB::RealTimeDB(QObject *parent) : QObject(parent)
{

}


RealTimeDB::~RealTimeDB()
{
    qDeleteAll(rtdb);
    rtdb.clear();
}

void RealTimeDB::setTagData(quint64 id, QByteArray val, bool syncToVendor)
{
    QMutexLocker locker(&m_mutex);
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            pObj->dataFromVendor.replace(0, val.length(), val);
            if(syncToVendor) {
                pObj->dataToVendor.replace(0, val.length(), val);
            }
        }
    }
}

void RealTimeDB::setTagData(quint64 id, const QString &dat, bool syncToVendor)
{
    QMutexLocker locker(&m_mutex);
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            pObj->fromString(dat, syncToVendor);
        }
    }
}

QByteArray RealTimeDB::tagData(quint64 id)
{
    QMutexLocker locker(&m_mutex);
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            return pObj->dataFromVendor;
        }
    }
    return QByteArray();
}

RunTimeTag* RealTimeDB::tag(quint64 id)
{
    if(rtdb.count(id) > 0) {
        return rtdb.value(id);
    }
    return NULL;
}

quint64 RealTimeDB::tagId(const QString &name)
{
    QHashIterator<quint64, RunTimeTag * > iter(rtdb);
    while (iter.hasNext()) {
        iter.next();
        RunTimeTag *pObj = iter.value();
        if(pObj) {
            if(pObj->name == name) {
                return iter.key();
            }
        }
    }
    return 0;
}

void RealTimeDB::debug()
{
    qDebug() << "RealTimeDB:----------------------------------------";
    QHashIterator<quint64, RunTimeTag * > iter(rtdb);
    while (iter.hasNext()) {
        iter.next();
        RunTimeTag *pObj = iter.value();
        if(pObj) {
            qDebug() << pObj->id << "=" << pObj->toString();
        }
    }
    qDebug() << "---------------------------------------------------";
}

extern "C" {

void rtTagAddValue(char *id, char *val)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    double dValue = strtod(val, NULL);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        double dTagVal = pTagObj->toString().toDouble();
        dTagVal += dValue;
        pTagObj->fromString(QString::number(dTagVal));
    }
}

void rtTagSubValue(char *id, char *val)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    double dValue = strtod(val, NULL);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        double dTagVal = pTagObj->toString().toDouble();
        dTagVal -= dValue;
        pTagObj->fromString(QString::number(dTagVal));
    }
}

void rtTagCopyValue(char *srcId, char *desId)
{
    quint32 srcTagID = (quint32)strtoul(srcId, NULL, 10);
    quint32 desTagID = (quint32)strtoul(desId, NULL, 10);
    RunTimeTag* pSrcTagObj = RealTimeDB::instance()->tag(srcTagID);
    RunTimeTag* pDesTagObj = RealTimeDB::instance()->tag(desTagID);
    if(pSrcTagObj && pDesTagObj) {
        pDesTagObj->fromString(pSrcTagObj->toString());
    }
}

void rtTagSetValue(char *id, char *val)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        pTagObj->fromString(val);
    }
}

void rtTagStateChange(char *id)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        double dTagVal = pTagObj->toString().toDouble();
        pTagObj->fromString((dTagVal > 0) ? "0" : "1");
    }
}

}
