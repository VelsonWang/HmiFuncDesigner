#include "IOTag.h"

IOTag::IOTag(QObject *parent) :
    QObject(parent),
    m_pDBTagObject(0),
    mLength(0)
{
    pReadBuf = 0;
    pWriteBuf = 0;
}

IOTag::~IOTag()
{
    if(pReadBuf != 0)
    {
        delete[] pReadBuf;
        pReadBuf = 0;
    }
    if(pWriteBuf != 0)
    {
        delete[] pWriteBuf;
        pWriteBuf = 0;
    }
}


QString IOTag::GetDeviceName()
{
    return m_sDeviceName;
}


void IOTag::SetDeviceName(QString s)
{
    m_sDeviceName = s;
}


TPermissionType IOTag::GetPermissionType()
{
    return m_PermissionType;
}


void IOTag::SetPermissionType(TPermissionType t)
{
    m_PermissionType = t;
}


qint32 IOTag::GetDeviceAddress()
{
    return m_iDeviceAddress;
}


void IOTag::SetDeviceAddress(qint32 add)
{
    m_iDeviceAddress = add;
}


QString IOTag::GetRegisterArea()
{
    return m_sRegisterArea;
}


void IOTag::SetRegisterArea(QString s)
{
    m_sRegisterArea = s;
}


qint32 IOTag::GetRegisterAddress()
{
    return m_iRegisterAddress;
}


void IOTag::SetRegisterAddress(qint32 add)
{
    m_iRegisterAddress = add;
}


TTagDataType IOTag::GetDataType()
{
    return m_sDataType;
}

qint32 IOTag::GetDataTypeLength()
{
    int ret = 0;
    switch (m_sDataType) {
        case TYPE_VARIANT:
        {
            ret = 0;
            break;
        }
        case TYPE_BOOL:
        {
            ret = 1;
            break;
        }
        case TYPE_INT8:
        case TYPE_UINT8:
        {
            ret = 1;
            break;
        }
        case TYPE_INT16:
        case TYPE_UINT16:
        {
            ret = 2;
            break;
        }
        case TYPE_INT32:
        case TYPE_UINT32:
        case TYPE_FLOAT:
        {
            ret = 4;
            break;
        }
        case TYPE_INT64:
        case TYPE_UINT64:
        case TYPE_DOUBLE:
        {
            ret = 8;
            break;
        }
        case TYPE_ASCII2CHAR:
        {
            ret = 2;
            break;
        }
        case TYPE_STRING:
        {
            ret = mLength;
            break;
        }
        case TYPE_BCD:
        {
            ret = mLength;
            break;
        }
        default:
        {
            ret = mLength;
            break;
        }
    }
    return ret;
}

void IOTag::SetDataType(TTagDataType t)
{
    m_sDataType = t;
}


qint32 IOTag::GetOffset()
{
    return m_iOffset;
}


void IOTag::SetOffset(qint32 o)
{
    m_iOffset = o;
}


qint32 IOTag::GetMaxValue()
{
    return m_iMaxValue;
}


void IOTag::SetMaxValue(qint32 v)
{
    m_iMaxValue = v;
}


qint32 IOTag::GetMinValue()
{
    return m_iMinValue;
}


void IOTag::SetMinValue(qint32 v)
{
    m_iMinValue = v;
}


qint32 IOTag::GetInitializeValue()
{
    return m_iInitializeValue;
}


void IOTag::SetInitializeValue(qint32 v)
{
    m_iInitializeValue = v;
}


float IOTag::GetScale()
{
    return m_fScale;
}


void IOTag::SetScale(float v)
{
    m_fScale = v;
}


qint32 IOTag::GetInFrameAddress()
{
    return m_iInFrameAddress;
}


void IOTag::SetInFrameAddress(qint32 i)
{
    m_iInFrameAddress = i;
}


DBTagObject * IOTag::GetDBTagObject()
{
    return m_pDBTagObject;
}


void IOTag::SetDBTagObject(DBTagObject * p)
{
    m_pDBTagObject = p;
}


void IOTag::SetTagBufferLength(qint32 i)
{
    mLength = i;
    if(pReadBuf != 0)
        delete[] pReadBuf;
    if(pWriteBuf != 0)
        delete[] pWriteBuf;
    pReadBuf = new unsigned char[mLength];
    pWriteBuf = new unsigned char[mLength];
}

void IOTag::SetTagID(qint32 id)
{
    mId = id;
}

qint32 IOTag::GetTagID()
{
    return mId;
}

IOTag &IOTag::operator=(const IOTag &rtag)
{
    this->m_sDeviceName = rtag.m_sDeviceName;
    this->m_PermissionType = rtag.m_PermissionType;
    this->m_iDeviceAddress = rtag.m_iDeviceAddress;
    this->m_sRegisterArea = rtag.m_sRegisterArea;
    this->m_iRegisterAddress = rtag.m_iRegisterAddress;
    this->m_sDataType = rtag.m_sDataType;
    this->m_iOffset = rtag.m_iOffset;
    this->m_iMaxValue = rtag.m_iMaxValue;
    this->m_iMinValue = rtag.m_iMinValue;
    this->m_iInitializeValue = rtag.m_iInitializeValue;
    this->m_fScale = rtag.m_fScale;
    this->m_iInFrameAddress = rtag.m_iInFrameAddress;
    this->m_pDBTagObject = rtag.m_pDBTagObject;

    this->mId = rtag.mId;
    this->mLength = rtag.mLength;
    pReadBuf = new unsigned char[this->mLength];
    pWriteBuf = new unsigned char[this->mLength];

    return *this;
}
