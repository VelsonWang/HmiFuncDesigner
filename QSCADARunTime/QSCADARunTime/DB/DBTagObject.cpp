#include "DBTagObject.h"
#include "SCADARunTime.h"


DBTagObject::DBTagObject()
{
    mId = "";
    mType = TYPE_BOOL;
    mPermission = READ_WRIE;
    mLength = 0;
    mData = 0;
    mPrivate.type =TYPE_TMP;
    mPrivate.pTagObject = nullptr;
    pVendor = nullptr;
}

DBTagObject::DBTagObject(QString id, TTagDataType t, TPermissionType p, qint32 len,
                         QVariant dat, TTagType tt, QObject *pObj)
{
    mId = id;
    mType = t;
    mPermission = p;
    mLength = len;
    mData = dat;
    mPrivate.type = tt;
    mPrivate.pTagObject = pObj;
    pVendor = nullptr;
}


DBTagObject::~DBTagObject()
{
    if(mPrivate.pTagObject != nullptr)
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


void DBTagObject::SetData(unsigned char* buffer)
{
    TTagDataType type = mType;
    switch(type)
    {
        case TYPE_VARIANT:
        {
            QByteArray dat((const char *)buffer, mLength);
            QVariant value(dat);
            mData = value;
        }break;
        case TYPE_BOOL:
        case TYPE_UINT8:
        {
            QVariant value((quint32)buffer[0]);
            mData = value;

        }break;
        case TYPE_INT8:
        {
            QVariant value((qint32)buffer[0]);
            mData = value;

        }break;
        case TYPE_INT16:
        {
            QVariant value((qint32)(buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_UINT16:
        {
            QVariant value((quint32)(buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_INT32:
        {
            QVariant value((qint32)(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_UINT32:
        {
            QVariant value((quint32)(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_INT64:
        {
            QVariant value((qint64)(buffer[7] << 56 | buffer[6] << 48 | buffer[5] << 40 | buffer[4] << 32 | buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_UINT64:
        {
            QVariant value((quint64)(buffer[7] << 56 | buffer[6] << 48 | buffer[5] << 40 | buffer[4] << 32 | buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_FLOAT:
        {
            union unionFloat
            {
                float ufloat;
                unsigned char ubytes[4];
            };
            unionFloat uFloat;
            for(int i=0; i<4; i++)
                uFloat.ubytes[i] = buffer[i];
            QVariant value(uFloat.ufloat);
            mData = value;
        }break;
        case TYPE_DOUBLE:
        {
            union unionDouble
            {
                double udouble;
                unsigned char ubytes[8];
            };
            unionDouble uDouble;
            for(int i=0; i<8; i++)
                uDouble.ubytes[i] = buffer[i];
            QVariant value(uDouble.udouble);
            mData = value;
        }break;
        case TYPE_ASCII2CHAR:
        {
            QVariant value((quint32)(buffer[1] << 8 | buffer[0]));
            mData = value;
        }break;
        case TYPE_STRING:
        {
            QByteArray dat((const char *)buffer, mLength);
            QVariant value(dat);
            mData = value;
        }break;
        case TYPE_BCD:
        {
            QByteArray dat((const char *)buffer, mLength);
            QVariant value(dat);
            mData = value;
        }break;
    }
}


QVariant DBTagObject::GetData()
{
    RuntimeEvent event(this, BEFORE_READ, mData);
    fireAEvent(event);
    return mData;
}

QByteArray DBTagObject::GetDataBytes()
{
    QByteArray dat;
    TTagDataType type = mType;
    switch(type)
    {
        case TYPE_VARIANT:
        {
            dat = mData.toByteArray();
        }break;
        case TYPE_BOOL:
        {
            dat.resize(1);
            dat[0] = (mData.toUInt() > 0) ? 1 : 0;
        }break;
        case TYPE_INT8:
        {
            dat.resize(1);
            dat[0] = static_cast<char>(mData.toInt());
        }break;
        case TYPE_INT16:
        {
            dat.resize(2);
            int val = mData.toInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_INT32:
        {
            dat.resize(4);
            int val = mData.toInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
        }break;
        case TYPE_UINT8:
        {
            dat.resize(1);
            dat[0] = static_cast<char>(mData.toUInt());
        }break;
        case TYPE_UINT16:
        {
            dat.resize(2);
            quint32 val = mData.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_UINT32:
        {
            dat.resize(4);
            quint32 val = mData.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
        }break;
        case TYPE_INT64:
        {
            dat.resize(8);
            qint64 val = mData.toLongLong();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
            dat[4] = static_cast<char>(val >> 32);
            dat[5] = static_cast<char>(val >> 40);
            dat[6] = static_cast<char>(val >> 48);
            dat[7] = static_cast<char>(val >> 56);
        }break;
        case TYPE_UINT64:
        {
            dat.resize(8);
            quint64 val = mData.toULongLong();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
            dat[4] = static_cast<char>(val >> 32);
            dat[5] = static_cast<char>(val >> 40);
            dat[6] = static_cast<char>(val >> 48);
            dat[7] = static_cast<char>(val >> 56);
        }break;
        case TYPE_FLOAT:
        {
            union unionFloat
            {
                float ufloat;
                unsigned char ubytes[4];
            };
            unionFloat uFloat;
            dat.resize(4);
            uFloat.ufloat = mData.toFloat();
            dat[0] = static_cast<char>(uFloat.ubytes[0]);
            dat[1] = static_cast<char>(uFloat.ubytes[1]);
            dat[2] = static_cast<char>(uFloat.ubytes[2]);
            dat[3] = static_cast<char>(uFloat.ubytes[3]);
        }break;
        case TYPE_DOUBLE:
        {
            union unionDouble
            {
                double udouble;
                unsigned char ubytes[8];
            };
            unionDouble uDouble;
            dat.resize(8);
            uDouble.udouble = mData.toDouble();
            dat[0] = static_cast<char>(uDouble.ubytes[0]);
            dat[1] = static_cast<char>(uDouble.ubytes[1]);
            dat[2] = static_cast<char>(uDouble.ubytes[2]);
            dat[3] = static_cast<char>(uDouble.ubytes[3]);
            dat[4] = static_cast<char>(uDouble.ubytes[4]);
            dat[5] = static_cast<char>(uDouble.ubytes[5]);
            dat[6] = static_cast<char>(uDouble.ubytes[6]);
            dat[7] = static_cast<char>(uDouble.ubytes[7]);
        }break;
        case TYPE_ASCII2CHAR:
        {
            dat.resize(2);
            quint32 val = mData.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_STRING:
        {
            dat = mData.toByteArray();
        }break;
        case TYPE_BCD:
        {
            dat = mData.toByteArray();
        }break;
    }
    return dat;
}


QVariant DBTagObject::GetWriteData()
{
    return mDataWrite;
}


QByteArray DBTagObject::GetWriteDataBytes()
{
    QByteArray dat;
    TTagDataType type = mType;
    switch(type)
    {
        case TYPE_VARIANT:
        {
            dat = mDataWrite.toByteArray();
        }break;
        case TYPE_BOOL:
        {
            dat.resize(1);
            dat[0] = (mDataWrite.toUInt() > 0) ? 1 : 0;
        }break;
        case TYPE_INT8:
        {
            dat.resize(1);
            dat[0] = static_cast<char>(mDataWrite.toInt());
        }break;
        case TYPE_INT16:
        {
            dat.resize(2);
            int val = mDataWrite.toInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_INT32:
        {
            dat.resize(4);
            int val = mDataWrite.toInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
        }break;
        case TYPE_UINT8:
        {
            dat.resize(1);
            dat[0] = static_cast<char>(mDataWrite.toUInt());
        }break;
        case TYPE_UINT16:
        {
            dat.resize(2);
            quint32 val = mDataWrite.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_UINT32:
        {
            dat.resize(4);
            quint32 val = mDataWrite.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
        }break;
        case TYPE_INT64:
        {
            dat.resize(8);
            qint64 val = mDataWrite.toLongLong();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
            dat[4] = static_cast<char>(val >> 32);
            dat[5] = static_cast<char>(val >> 40);
            dat[6] = static_cast<char>(val >> 48);
            dat[7] = static_cast<char>(val >> 56);
        }break;
        case TYPE_UINT64:
        {
            dat.resize(8);
            quint64 val = mDataWrite.toULongLong();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
            dat[2] = static_cast<char>(val >> 16);
            dat[3] = static_cast<char>(val >> 24);
            dat[4] = static_cast<char>(val >> 32);
            dat[5] = static_cast<char>(val >> 40);
            dat[6] = static_cast<char>(val >> 48);
            dat[7] = static_cast<char>(val >> 56);
        }break;
        case TYPE_FLOAT:
        {
            union unionFloat
            {
                float ufloat;
                unsigned char ubytes[4];
            };
            unionFloat uFloat;
            dat.resize(4);
            uFloat.ufloat = mDataWrite.toFloat();
            dat[0] = static_cast<char>(uFloat.ubytes[0]);
            dat[1] = static_cast<char>(uFloat.ubytes[1]);
            dat[2] = static_cast<char>(uFloat.ubytes[2]);
            dat[3] = static_cast<char>(uFloat.ubytes[3]);
        }break;
        case TYPE_DOUBLE:
        {
            union unionDouble
            {
                double udouble;
                unsigned char ubytes[8];
            };
            unionDouble uDouble;
            dat.resize(8);
            uDouble.udouble = mDataWrite.toDouble();
            dat[0] = static_cast<char>(uDouble.ubytes[0]);
            dat[1] = static_cast<char>(uDouble.ubytes[1]);
            dat[2] = static_cast<char>(uDouble.ubytes[2]);
            dat[3] = static_cast<char>(uDouble.ubytes[3]);
            dat[4] = static_cast<char>(uDouble.ubytes[4]);
            dat[5] = static_cast<char>(uDouble.ubytes[5]);
            dat[6] = static_cast<char>(uDouble.ubytes[6]);
            dat[7] = static_cast<char>(uDouble.ubytes[7]);
        }break;
        case TYPE_ASCII2CHAR:
        {
            dat.resize(2);
            quint32 val = mDataWrite.toUInt();
            dat[0] = static_cast<char>(val);
            dat[1] = static_cast<char>(val >> 8);
        }break;
        case TYPE_STRING:
        {
            dat = mDataWrite.toByteArray();
        }break;
        case TYPE_BCD:
        {
            dat = mDataWrite.toByteArray();
        }break;
    }
    return dat;
}




