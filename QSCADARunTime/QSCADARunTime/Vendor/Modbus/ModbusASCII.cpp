
#include <QDebug>

#include "ModbusASCII.h"
#include "../../Public/PublicFunction.h"


ModbusASCII::ModbusASCII()
{

}


ModbusASCII::~ModbusASCII()
{

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, len, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int iWaitLen = 3 + 7 * 2;
    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num * 2;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 3 + (4 + databuflen) * 2;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

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

    if(get2BytesOrder() == "21")
    {
        m_pTmpBuf[len++] = data >> 8;
        m_pTmpBuf[len++] = data;
    }
    else if(get2BytesOrder() == "12")
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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 14, 1000);

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

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 14, 1000);

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

    if(get4BytesOrder() == "4321")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(get4BytesOrder() == "4312")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(get4BytesOrder() == "3421")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(get4BytesOrder() == "3412")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(get4BytesOrder() == "2143")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(get4BytesOrder() == "2134")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    else if(get4BytesOrder() == "1243")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(get4BytesOrder() == "1234")
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

    if(get4BytesOrder() == "4321")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(get4BytesOrder() == "4312")
    {
        pbuf[0] = uInt.ubytes[3];
        pbuf[1] = uInt.ubytes[2];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(get4BytesOrder() == "3421")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[1];
        pbuf[3] = uInt.ubytes[0];
    }
    else if(get4BytesOrder() == "3412")
    {
        pbuf[0] = uInt.ubytes[2];
        pbuf[1] = uInt.ubytes[3];
        pbuf[2] = uInt.ubytes[0];
        pbuf[3] = uInt.ubytes[1];
    }
    else if(get4BytesOrder() == "2143")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(get4BytesOrder() == "2134")
    {
        pbuf[0] = uInt.ubytes[1];
        pbuf[1] = uInt.ubytes[0];
        pbuf[2] = uInt.ubytes[2];
        pbuf[3] = uInt.ubytes[3];
    }
    else if(get4BytesOrder() == "1243")
    {
        pbuf[0] = uInt.ubytes[0];
        pbuf[1] = uInt.ubytes[1];
        pbuf[2] = uInt.ubytes[3];
        pbuf[3] = uInt.ubytes[2];
    }
    else if(get4BytesOrder() == "1234")
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

    if(get4BytesOrder() == "4321")
    {
        pbuf[0] = uFloat.ubytes[3];
        pbuf[1] = uFloat.ubytes[2];
        pbuf[2] = uFloat.ubytes[1];
        pbuf[3] = uFloat.ubytes[0];
    }
    else if(get4BytesOrder() == "4312")
    {
        pbuf[0] = uFloat.ubytes[3];
        pbuf[1] = uFloat.ubytes[2];
        pbuf[2] = uFloat.ubytes[0];
        pbuf[3] = uFloat.ubytes[1];
    }
    else if(get4BytesOrder() == "3421")
    {
        pbuf[0] = uFloat.ubytes[2];
        pbuf[1] = uFloat.ubytes[3];
        pbuf[2] = uFloat.ubytes[1];
        pbuf[3] = uFloat.ubytes[0];
    }
    else if(get4BytesOrder() == "3412")
    {
        pbuf[0] = uFloat.ubytes[2];
        pbuf[1] = uFloat.ubytes[3];
        pbuf[2] = uFloat.ubytes[0];
        pbuf[3] = uFloat.ubytes[1];
    }
    else if(get4BytesOrder() == "2143")
    {
        pbuf[0] = uFloat.ubytes[1];
        pbuf[1] = uFloat.ubytes[0];
        pbuf[2] = uFloat.ubytes[3];
        pbuf[3] = uFloat.ubytes[2];
    }
    else if(get4BytesOrder() == "2134")
    {
        pbuf[0] = uFloat.ubytes[1];
        pbuf[1] = uFloat.ubytes[0];
        pbuf[2] = uFloat.ubytes[2];
        pbuf[3] = uFloat.ubytes[3];
    }
    else if(get4BytesOrder() == "1243")
    {
        pbuf[0] = uFloat.ubytes[0];
        pbuf[1] = uFloat.ubytes[1];
        pbuf[2] = uFloat.ubytes[3];
        pbuf[3] = uFloat.ubytes[2];
    }
    else if(get4BytesOrder() == "1234")
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






