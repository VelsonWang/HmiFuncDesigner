#include "Modbus.h"
#include "../../Public/PublicFunction.h"

Modbus::Modbus(QObject *parent)
    : QObject(parent)
{
    iFacePort_ = nullptr;
    m_iCurReadAddress = 0;
    str2BytesOrder_ = "21";
    str3BytesOrder_ = "321";
    str4BytesOrder_ = "2143";
    strFloatBytesOrder_ = "2143";
}

Modbus::~Modbus()
{

}


void Modbus::SetPort(IPort *pPort)
{
    if(pPort != nullptr)
        iFacePort_ = pPort;
}

IPort *Modbus::GetPort()
{
    return iFacePort_;
}

void Modbus::set2BytesOrder(const QString &s)
{
    str2BytesOrder_ = s;
}

QString Modbus::get2BytesOrder()
{
    return str2BytesOrder_;
}

void Modbus::set3BytesOrder(const QString &s)
{
    str3BytesOrder_ = s;
}

QString Modbus::get3BytesOrder()
{
    return str3BytesOrder_;
}

void Modbus::set4BytesOrder(const QString &s)
{
    str4BytesOrder_ = s;
}

QString Modbus::get4BytesOrder()
{
    return str4BytesOrder_;
}

void Modbus::setFloatBytesOrder(const QString &s)
{
    strFloatBytesOrder_ = s;
}

QString Modbus::getFloatBytesOrder()
{
    return strFloatBytesOrder_;
}

void Modbus::ClearBuffer()
{
    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));
    BytesSetZero(m_pTmpBuf, sizeof(m_pTmpBuf)/sizeof(unsigned char));
}



/*
* get single coil value
*/
unsigned char Modbus::GetBoolValue()
{
    return m_pDataBuf[0]&0x01;
}


/*
* get single coil value by read address
*/
unsigned char Modbus::GetBoolValue(int add)
{
    int addoffset = add - m_iCurReadAddress;
    int byteoffset = addoffset/8;
    int bitoffset = addoffset%8;
    unsigned char value = 0;

    if(addoffset >= 0)
    {
        value = (m_pDataBuf[byteoffset]>>bitoffset)&0x01;
    }
    return value;
}


short Modbus::GetShortValue()
{
    return GetShortValue(m_iCurReadAddress);
}



short Modbus::GetShortValue(int add)
{
    int addoffset = add - m_iCurReadAddress;

    if(get2BytesOrder() == "21")
    {
        return (short)(m_pDataBuf[addoffset*2]<<8 | m_pDataBuf[addoffset*2+1]);
    }
    else if(get2BytesOrder() == "12")
    {
        return (short)(m_pDataBuf[addoffset*2+1]<<8 | m_pDataBuf[addoffset*2]);
    }
    return 0;
}



unsigned short Modbus::GetUnsignedShortValue()
{
    return GetUnsignedShortValue(m_iCurReadAddress);
}



unsigned short Modbus::GetUnsignedShortValue(int add)
{
    int addoffset = add - m_iCurReadAddress;

    if(get2BytesOrder() == "21")
    {
        return (unsigned short)(m_pDataBuf[addoffset*2]<<8 | m_pDataBuf[addoffset*2+1]);
    }
    else if(get2BytesOrder() == "12")
    {
        return (unsigned short)(m_pDataBuf[addoffset*2+1]<<8 | m_pDataBuf[addoffset*2]);
    }
    return 0;
}



int Modbus::GetIntValue()
{
    return GetIntValue(m_iCurReadAddress);
}



int Modbus::GetIntValue(int add)
{
    int o = add - m_iCurReadAddress;

    if(get4BytesOrder() == "4321")
    {
        return (int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(get4BytesOrder() == "4312")
    {
        return (int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(get4BytesOrder() == "3421")
    {
        return (int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(get4BytesOrder() == "3412")
    {
        return (int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(get4BytesOrder() == "2143")
    {
        return (int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(get4BytesOrder() == "2134")
    {
        return (int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    else if(get4BytesOrder() == "1243")
    {
        return (int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(get4BytesOrder() == "1234")
    {
        return (int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    return 0;
}



unsigned int Modbus::GetUnsignedIntValue()
{
    return GetUnsignedIntValue(m_iCurReadAddress);
}



unsigned int Modbus::GetUnsignedIntValue(int add)
{
    int o = add - m_iCurReadAddress;

    if(get4BytesOrder() == "4321")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(get4BytesOrder() == "4312")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(get4BytesOrder() == "3421")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(get4BytesOrder() == "3412")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(get4BytesOrder() == "2143")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(get4BytesOrder() == "2134")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    else if(get4BytesOrder() == "1243")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(get4BytesOrder() == "1234")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    return 0;
}



float Modbus::GetFloatValue()
{
    return GetFloatValue(m_iCurReadAddress);
}



float Modbus::GetFloatValue(int add)
{
    union unionFloat
    {
        float ufloat;
        unsigned char ubytes[4];
    };
    unionFloat uFloat;
    int o = add - m_iCurReadAddress;

    if(getFloatBytesOrder() == "4321")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+3];
        uFloat.ubytes[1] = m_pDataBuf[o*2+2];
        uFloat.ubytes[2] = m_pDataBuf[o*2+1];
        uFloat.ubytes[3] = m_pDataBuf[o*2+0];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "4312")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+3];
        uFloat.ubytes[1] = m_pDataBuf[o*2+2];
        uFloat.ubytes[2] = m_pDataBuf[o*2+0];
        uFloat.ubytes[3] = m_pDataBuf[o*2+1];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "3421")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+2];
        uFloat.ubytes[1] = m_pDataBuf[o*2+3];
        uFloat.ubytes[2] = m_pDataBuf[o*2+1];
        uFloat.ubytes[3] = m_pDataBuf[o*2+0];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "3412")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+2];
        uFloat.ubytes[1] = m_pDataBuf[o*2+3];
        uFloat.ubytes[2] = m_pDataBuf[o*2+0];
        uFloat.ubytes[3] = m_pDataBuf[o*2+1];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "2143")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+1];
        uFloat.ubytes[1] = m_pDataBuf[o*2+0];
        uFloat.ubytes[2] = m_pDataBuf[o*2+3];
        uFloat.ubytes[3] = m_pDataBuf[o*2+2];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "2134")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+1];
        uFloat.ubytes[1] = m_pDataBuf[o*2+0];
        uFloat.ubytes[2] = m_pDataBuf[o*2+2];
        uFloat.ubytes[3] = m_pDataBuf[o*2+3];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "1243")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+0];
        uFloat.ubytes[1] = m_pDataBuf[o*2+1];
        uFloat.ubytes[2] = m_pDataBuf[o*2+3];
        uFloat.ubytes[3] = m_pDataBuf[o*2+2];
        return uFloat.ufloat;
    }
    else if(getFloatBytesOrder() == "1234")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+0];
        uFloat.ubytes[1] = m_pDataBuf[o*2+1];
        uFloat.ubytes[2] = m_pDataBuf[o*2+2];
        uFloat.ubytes[3] = m_pDataBuf[o*2+3];
        return uFloat.ufloat;
    }
    return 0;
}


