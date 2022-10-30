#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMutexLocker>
#include <QHashIterator>
#include "realtimedb.h"
#include "publicfunction.h"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <limits>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

RunTimeTag::RunTimeTag(QObject *parent) : QObject(parent), id(0)
{
    id = 0;
    blockReadID = 0;
    name = "";
    unit = "";
    addrType = "";
    addrOffset = 0;
    aAddrType2 = "";
    addrOffset2 = 0;
    dataType = TYPE_VARIANT;
    writeable = CAN_WRITE;
    remark = "";
    ownGroup = "";
    devType = "";
    bufLength = 0;
    dataFromVendor = NULL;
    dataToVendor = NULL;
    isBlockRead = false;
    lastReadTime = 0;
}

RunTimeTag::~RunTimeTag()
{
    delete[] dataFromVendor;
    dataFromVendor = NULL;
    delete[] dataToVendor;
    dataToVendor = NULL;
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
    switch (dataType) {
    case TYPE_BOOL:
        boolToBytes(dat.toUInt(), this->dataFromVendor);
        if(syncToVendor) {
            boolToBytes(dat.toUInt(), this->dataToVendor);
        }
        break;
    case TYPE_INT8:
        int8ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            int8ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_UINT8:
        uint8ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            uint8ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_INT16:
        int16ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            int16ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_UINT16:
        uint16ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            uint16ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_INT32:
        int32ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            int32ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_UINT32:
        uint32ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            uint32ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_INT64:
        int64ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            int64ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_UINT64:
        uint64ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            uint64ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_FLOAT32:
        float32ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            float32ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_FLOAT64:
        float64ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            float64ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_BCD8:
        bcd8ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            bcd8ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_BCD16:
        bcd16ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            bcd16ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_BCD32:
        bcd32ToBytes(dat.toInt(), this->dataFromVendor);
        if(syncToVendor) {
            bcd32ToBytes(dat.toInt(), this->dataToVendor);
        }
        break;
    case TYPE_ASCII2CHAR:
        updateVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        if(syncToVendor) {
            updateToVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        }
        break;
    case TYPE_STRING:
        updateVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        if(syncToVendor) {
            updateToVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        }
        break;
    case TYPE_BYTES:
        updateVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        if(syncToVendor) {
            updateToVendorData((quint8 *)dat.toLatin1().data(), dat.toLatin1().length());
        }
        break;
    default:
        break;
    };
    if(syncToVendor) {
        this->dataToVendor[this->bufLength] = 1;
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
        ret = QString((char *)dataFromVendor);
        break;
    case TYPE_STRING:
        ret = QString((char *)dataFromVendor);
        break;
    case TYPE_BYTES:
        ret = QString((char *)dataFromVendor);
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
        ret = !QString((char *)dataFromVendor).isEmpty();
        break;
    case TYPE_STRING:
        ret = !QString((char *)dataFromVendor).isEmpty();
        break;
    case TYPE_BYTES:
        ret = !QString((char *)dataFromVendor).isEmpty();
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
    releaseTags();
}

void RealTimeDB::releaseTags()
{
    QHashIterator<quint64, RunTimeTag * > iter(rtdb);
    while (iter.hasNext()) {
        iter.next();
        RunTimeTag *pObj = iter.value();
        if(pObj) {
            delete pObj;
        }
    }
    rtdb.clear();
}

void RealTimeDB::setTagData(quint64 id, quint8 *pDat, int len, bool syncToVendor)
{
    QMutexLocker locker(&m_mutex);
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            pObj->updateVendorData(pDat, len);
            if(syncToVendor) {
                pObj->updateToVendorData(pDat, len);
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

quint8* RealTimeDB::tagData(quint64 id)
{
    QMutexLocker locker(&m_mutex);
    if(rtdb.count(id) > 0) {
        RunTimeTag* pObj = rtdb.value(id);
        if(pObj) {
            return pObj->dataFromVendor;
        }
    }
    return NULL;
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
        pTagObj->fromString(QString::number(dTagVal), true);
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
        pTagObj->fromString(QString::number(dTagVal), true);
    }
}

void rtTagCopyValue(char *srcId, char *desId)
{
    quint32 srcTagID = (quint32)strtoul(srcId, NULL, 10);
    quint32 desTagID = (quint32)strtoul(desId, NULL, 10);
    RunTimeTag* pSrcTagObj = RealTimeDB::instance()->tag(srcTagID);
    RunTimeTag* pDesTagObj = RealTimeDB::instance()->tag(desTagID);
    if(pSrcTagObj && pDesTagObj) {
        pDesTagObj->fromString(pSrcTagObj->toString(), true);
    }
}

void rtTagSetValue(char *id, char *val)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        pTagObj->fromString(val, true);
    }
}

void rtTagStateChange(char *id)
{
    quint32 tagID = (quint32)strtoul(id, NULL, 10);
    RunTimeTag* pTagObj = RealTimeDB::instance()->tag(tagID);
    if(pTagObj) {
        double dTagVal = pTagObj->toString().toDouble();
        pTagObj->fromString((dTagVal > 0) ? "0" : "1", true);
    }
}

}
