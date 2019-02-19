
#include <QDebug>

#include "ModbusASCII.h"
#include "../../Public/PublicFunction.h"


ModbusASCII::ModbusASCII()
{
    iFacePort = 0;
    m_pReadBuf = new unsigned char[512];
    m_pWriteBuf = new unsigned char[512];
    m_pDataBuf = new unsigned char[512];
    m_pTmpBuf = new unsigned char[1024];
    m_iCurReadAddress = 0;
    m_str2BytesOrder = "21";
    m_str3BytesOrder = "321";
    m_str4BytesOrder = "2143";
    m_strFloatBytesOrder = "2143";
}


void ModbusASCII::SetPort(IPort *pPort)
{
    iFacePort = pPort;
}

ModbusASCII::~ModbusASCII()
{
    delete[] m_pReadBuf;
    delete[] m_pWriteBuf;
    delete[] m_pDataBuf;
    delete[] m_pTmpBuf;
}


void ModbusASCII::set2BytesOrder(QString s)
{
    m_str2BytesOrder = s;
}


void ModbusASCII::set3BytesOrder(QString s)
{
    m_str3BytesOrder = s;
}


void ModbusASCII::set4BytesOrder(QString s)
{
    m_str4BytesOrder = s;
}


void ModbusASCII::setFloatBytesOrder(QString s)
{
    m_strFloatBytesOrder = s;
}

unsigned char ModbusASCII::GetLRC(unsigned char *pdat, short len)
{
    unsigned char uchLrc = 0;

    while (len--)
        uchLrc += *pdat++ ;

    return ((unsigned char)(-((char)uchLrc)));
}


bool ModbusASCII::MessageCheck(unsigned char *inBuf, short bufLen)
{
    unsigned char cLRC = 0, sLRC = 0;
    int len = 0;

    BytesSetZero(m_pTmpBuf, sizeof(m_pTmpBuf)/sizeof(unsigned char));
    MakeAsiiToCode(inBuf+1, m_pTmpBuf, bufLen - 3);
    len = (bufLen - 3) / 2;

    sLRC = m_pTmpBuf[len - 1];
    cLRC = GetLRC(m_pTmpBuf, len - 1);

    if(cLRC == sLRC)
        return true;

    return false;
}


void ModbusASCII::ClearBuffer()
{
    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));
    BytesSetZero(m_pTmpBuf, sizeof(m_pTmpBuf)/sizeof(unsigned char));
}


/*
* 读线圈状态
*/
ResultType ModbusASCII::ReadCoils(int addDev, int addCol, int num,
                            unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_iCurReadAddress = addCol;

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x01;
    m_pTmpBuf[len++] = addCol >> 8;
    m_pTmpBuf[len++] = addCol;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pTmpBuf[3+idx];
            m_pDataBuf[idx] = m_pTmpBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制单个线圈    0xFF00--on, 0x0000--Off
*/
ResultType ModbusASCII::WriteCoil(int addDev, int addCol, int value)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x05;
    m_pTmpBuf[len++] = addCol >> 8;
    m_pTmpBuf[len++] = addCol;

    if(value)
    {
        m_pTmpBuf[len++] = 0xFF;
        m_pTmpBuf[len++] = 0x00;
    }
    else
    {
        m_pTmpBuf[len++] = 0x00;
        m_pTmpBuf[len++] = 0x00;
    }

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, len, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == len && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制多个线圈
*/
ResultType ModbusASCII::WriteMultipleCoils(int addDev, int addCol,
                                     int num, int cntByte, unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x0F;
    m_pTmpBuf[len++] = addCol >> 8;
    m_pTmpBuf[len++] = addCol;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;
    m_pTmpBuf[len++] = cntByte;

    for(int i = 0; i < cntByte; i++)
    {
        m_pTmpBuf[len++] = pbuf[i];  // array[0] coil0--7, array[1]--coil8-15
    }

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int iWaitLen = 3 + 7 * 2;
    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* read input bit status
*/
ResultType ModbusASCII::ReadDiscreteInputs(int addDev, int addinput, int num,
                              unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_iCurReadAddress = addinput;

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x02;
    m_pTmpBuf[len++] = addinput >> 8;
    m_pTmpBuf[len++] = addinput;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pTmpBuf[3+idx];
            m_pDataBuf[idx] = m_pTmpBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读输入寄存器
*/
ResultType ModbusASCII::ReadReadInputRegister(int addDev, int addReg, int num,
                                            unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_iCurReadAddress = addReg;

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x04;
    m_pTmpBuf[len++] = addReg >> 8;
    m_pTmpBuf[len++] = addReg;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int databuflen = num * 2;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pTmpBuf[3+idx];
            m_pDataBuf[idx] = m_pTmpBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读保持寄存器
*/
ResultType ModbusASCII::ReadHoldingRegister(int addDev, int addReg, int num,
                                          unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_iCurReadAddress = addReg;

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x03;
    m_pTmpBuf[len++] = addReg >> 8;
    m_pTmpBuf[len++] = addReg;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pTmpBuf[3+idx];
            m_pDataBuf[idx] = m_pTmpBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 预置单个寄存器
*/
ResultType ModbusASCII::WriteHoldingRegister(int addDev, int addReg, unsigned short data)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x06;
    m_pTmpBuf[len++] = addReg >> 8;
    m_pTmpBuf[len++] = addReg;

    if(m_str2BytesOrder == "21")
    {
        m_pTmpBuf[len++] = data >> 8;
        m_pTmpBuf[len++] = data;
    }
    else if(m_str2BytesOrder == "12")
    {
        m_pTmpBuf[len++] = data;
        m_pTmpBuf[len++] = data >> 8;
    }

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, 14, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == 14 && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制多个寄存器
*/
ResultType ModbusASCII::WriteMultipleHoldingRegister(int addDev, int addReg,
                                                   int num, unsigned char *pbuf)
{
    unsigned char len = 0, pos = 0;

    ClearBuffer();

    m_pTmpBuf[len++] = addDev;
    m_pTmpBuf[len++] = 0x10;
    m_pTmpBuf[len++] = addReg >> 8;
    m_pTmpBuf[len++] = addReg;
    m_pTmpBuf[len++] = num >> 8;
    m_pTmpBuf[len++] = num;
    m_pTmpBuf[len++] = num*2;

    for(int i = 0; i < num*2; i++)
    {
        m_pTmpBuf[len++] = pbuf[i];
    }

    m_pTmpBuf[len] = GetLRC(m_pTmpBuf, len);
    len++;

    pos = 0;
    m_pWriteBuf[pos++] = 0x3A;
    MakeCodeToAsii(m_pTmpBuf, m_pWriteBuf + pos, len);
    pos = pos + 2 * len;

    m_pWriteBuf[pos++] = 0x0D;
    m_pWriteBuf[pos++] = 0x0A;
    len = pos;

    //ShowBufferHex("tx: ", m_pWriteBuf, len);
    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, 14, 1000);

    //ShowBufferHex("rx: ", m_pReadBuf, len);
    if(resultlen == 14 && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


ResultType ModbusASCII::WriteIntToHoldingRegister(int addDev, int addReg,
                                                int data)
{
    unsigned char *pbuf = new unsigned char[4];
    union unionInt
    {
        int uInt;
        unsigned char ubytes[4];
    };
    unionInt uInt;

    uInt.uInt = data;

    if(m_str4BytesOrder == "4321")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(m_str4BytesOrder == "4312")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(m_str4BytesOrder == "3421")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(m_str4BytesOrder == "3412")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(m_str4BytesOrder == "2143")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(m_str4BytesOrder == "2134")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    else if(m_str4BytesOrder == "1243")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(m_str4BytesOrder == "1234")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    ResultType result = WriteMultipleHoldingRegister(addDev, addReg, 2, pbuf);
    delete[] pbuf;
    return result;
}


ResultType ModbusASCII::WriteUIntToHoldingRegister(int addDev, int addReg,
                                                 unsigned int data)
{
    unsigned char *pbuf = new unsigned char[4];
    union unionInt
    {
        unsigned int uUInt;
        unsigned char ubytes[4];
    };
    unionInt uInt;

    uInt.uUInt = data;

    if(m_str4BytesOrder == "4321")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(m_str4BytesOrder == "4312")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(m_str4BytesOrder == "3421")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(m_str4BytesOrder == "3412")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(m_str4BytesOrder == "2143")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(m_str4BytesOrder == "2134")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    else if(m_str4BytesOrder == "1243")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(m_str4BytesOrder == "1234")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    ResultType result = WriteMultipleHoldingRegister(addDev, addReg, 2, pbuf);
    delete[] pbuf;
    return result;
}


ResultType ModbusASCII::WriteFloatToHoldingRegister(int addDev, int addReg,
                                                  float data)
{
    unsigned char *pbuf = new unsigned char[4];
    union unionFloat
    {
        float ufloat;
        unsigned char ubytes[4];
    };
    unionFloat uFloat;

    uFloat.ufloat = data;

    if(m_str4BytesOrder == "4321")
    {
        pbuf[0] = uFloat.ubytes[3];
        pbuf[1] = uFloat.ubytes[2];
        pbuf[2] = uFloat.ubytes[1];
        pbuf[3] = uFloat.ubytes[0];
    }
    else if(m_str4BytesOrder == "4312")
    {
        pbuf[0] = uFloat.ubytes[3];
        pbuf[1] = uFloat.ubytes[2];
        pbuf[2] = uFloat.ubytes[0];
        pbuf[3] = uFloat.ubytes[1];
    }
    else if(m_str4BytesOrder == "3421")
    {
        pbuf[0] = uFloat.ubytes[2];
        pbuf[1] = uFloat.ubytes[3];
        pbuf[2] = uFloat.ubytes[1];
        pbuf[3] = uFloat.ubytes[0];
    }
    else if(m_str4BytesOrder == "3412")
    {
        pbuf[0] = uFloat.ubytes[2];
        pbuf[1] = uFloat.ubytes[3];
        pbuf[2] = uFloat.ubytes[0];
        pbuf[3] = uFloat.ubytes[1];
    }
    else if(m_str4BytesOrder == "2143")
    {
        pbuf[0] = uFloat.ubytes[1];
        pbuf[1] = uFloat.ubytes[0];
        pbuf[2] = uFloat.ubytes[3];
        pbuf[3] = uFloat.ubytes[2];
    }
    else if(m_str4BytesOrder == "2134")
    {
        pbuf[0] = uFloat.ubytes[1];
        pbuf[1] = uFloat.ubytes[0];
        pbuf[2] = uFloat.ubytes[2];
        pbuf[3] = uFloat.ubytes[3];
    }
    else if(m_str4BytesOrder == "1243")
    {
        pbuf[0] = uFloat.ubytes[0];
        pbuf[1] = uFloat.ubytes[1];
        pbuf[2] = uFloat.ubytes[3];
        pbuf[3] = uFloat.ubytes[2];
    }
    else if(m_str4BytesOrder == "1234")
    {
        pbuf[0] = uFloat.ubytes[0];
        pbuf[1] = uFloat.ubytes[1];
        pbuf[2] = uFloat.ubytes[2];
        pbuf[3] = uFloat.ubytes[3];
    }
    ResultType result = WriteMultipleHoldingRegister(addDev, addReg, 2, pbuf);
    delete[] pbuf;
    return result;
}



/*
* get single coil value
*/
unsigned char ModbusASCII::GetBoolValue()
{
    return m_pDataBuf[0]&0x01;
}


/*
* get single coil value by read address
*/
unsigned char ModbusASCII::GetBoolValue(int add)
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


short ModbusASCII::GetShortValue()
{
    return GetShortValue(m_iCurReadAddress);
}



short ModbusASCII::GetShortValue(int add)
{
    int addoffset = add - m_iCurReadAddress;

    if(m_str2BytesOrder == "21")
    {
        return (short)(m_pDataBuf[addoffset*2]<<8 | m_pDataBuf[addoffset*2+1]);
    }
    else if(m_str2BytesOrder == "12")
    {
        return (short)(m_pDataBuf[addoffset*2+1]<<8 | m_pDataBuf[addoffset*2]);
    }
    return 0;
}



unsigned short ModbusASCII::GetUnsignedShortValue()
{
    return GetUnsignedShortValue(m_iCurReadAddress);
}



unsigned short ModbusASCII::GetUnsignedShortValue(int add)
{
    int addoffset = add - m_iCurReadAddress;

    if(m_str2BytesOrder == "21")
    {
        return (unsigned short)(m_pDataBuf[addoffset*2]<<8 | m_pDataBuf[addoffset*2+1]);
    }
    else if(m_str2BytesOrder == "12")
    {
        return (unsigned short)(m_pDataBuf[addoffset*2+1]<<8 | m_pDataBuf[addoffset*2]);
    }
    return 0;
}



int ModbusASCII::GetIntValue()
{
    return GetIntValue(m_iCurReadAddress);
}



int ModbusASCII::GetIntValue(int add)
{
    int o = add - m_iCurReadAddress;

    if(m_str4BytesOrder == "4321")
    {
        return (int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(m_str4BytesOrder == "4312")
    {
        return (int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(m_str4BytesOrder == "3421")
    {
        return (int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(m_str4BytesOrder == "3412")
    {
        return (int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(m_str4BytesOrder == "2143")
    {
        return (int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(m_str4BytesOrder == "2134")
    {
        return (int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    else if(m_str4BytesOrder == "1243")
    {
        return (int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(m_str4BytesOrder == "1234")
    {
        return (int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    return 0;
}



unsigned int ModbusASCII::GetUnsignedIntValue()
{
    return GetUnsignedIntValue(m_iCurReadAddress);
}



unsigned int ModbusASCII::GetUnsignedIntValue(int add)
{
    int o = add - m_iCurReadAddress;

    if(m_str4BytesOrder == "4321")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(m_str4BytesOrder == "4312")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<24 | m_pDataBuf[o*2+1]<<16 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(m_str4BytesOrder == "3421")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<8 | m_pDataBuf[o*2+3]<<0);
    }
    else if(m_str4BytesOrder == "3412")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<16 | m_pDataBuf[o*2+1]<<24 |
                m_pDataBuf[o*2+2]<<0 | m_pDataBuf[o*2+3]<<8);
    }
    else if(m_str4BytesOrder == "2143")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(m_str4BytesOrder == "2134")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<8 | m_pDataBuf[o*2+1]<<0 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    else if(m_str4BytesOrder == "1243")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<24 | m_pDataBuf[o*2+3]<<16);
    }
    else if(m_str4BytesOrder == "1234")
    {
        return (unsigned int)(m_pDataBuf[o*2]<<0 | m_pDataBuf[o*2+1]<<8 |
                m_pDataBuf[o*2+2]<<16 | m_pDataBuf[o*2+3]<<24);
    }
    return 0;
}



float ModbusASCII::GetFloatValue()
{
    return GetFloatValue(m_iCurReadAddress);
}



float ModbusASCII::GetFloatValue(int add)
{
    union unionFloat
    {
        float ufloat;
        unsigned char ubytes[4];
    };
    unionFloat uFloat;
    int o = add - m_iCurReadAddress;

    if(m_strFloatBytesOrder == "4321")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+3];
        uFloat.ubytes[1] = m_pDataBuf[o*2+2];
        uFloat.ubytes[2] = m_pDataBuf[o*2+1];
        uFloat.ubytes[3] = m_pDataBuf[o*2+0];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "4312")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+3];
        uFloat.ubytes[1] = m_pDataBuf[o*2+2];
        uFloat.ubytes[2] = m_pDataBuf[o*2+0];
        uFloat.ubytes[3] = m_pDataBuf[o*2+1];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "3421")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+2];
        uFloat.ubytes[1] = m_pDataBuf[o*2+3];
        uFloat.ubytes[2] = m_pDataBuf[o*2+1];
        uFloat.ubytes[3] = m_pDataBuf[o*2+0];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "3412")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+2];
        uFloat.ubytes[1] = m_pDataBuf[o*2+3];
        uFloat.ubytes[2] = m_pDataBuf[o*2+0];
        uFloat.ubytes[3] = m_pDataBuf[o*2+1];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "2143")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+1];
        uFloat.ubytes[1] = m_pDataBuf[o*2+0];
        uFloat.ubytes[2] = m_pDataBuf[o*2+3];
        uFloat.ubytes[3] = m_pDataBuf[o*2+2];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "2134")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+1];
        uFloat.ubytes[1] = m_pDataBuf[o*2+0];
        uFloat.ubytes[2] = m_pDataBuf[o*2+2];
        uFloat.ubytes[3] = m_pDataBuf[o*2+3];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "1243")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+0];
        uFloat.ubytes[1] = m_pDataBuf[o*2+1];
        uFloat.ubytes[2] = m_pDataBuf[o*2+3];
        uFloat.ubytes[3] = m_pDataBuf[o*2+2];
        return uFloat.ufloat;
    }
    else if(m_strFloatBytesOrder == "1234")
    {
        uFloat.ubytes[0] = m_pDataBuf[o*2+0];
        uFloat.ubytes[1] = m_pDataBuf[o*2+1];
        uFloat.ubytes[2] = m_pDataBuf[o*2+2];
        uFloat.ubytes[3] = m_pDataBuf[o*2+3];
        return uFloat.ufloat;
    }
    return 0;
}







