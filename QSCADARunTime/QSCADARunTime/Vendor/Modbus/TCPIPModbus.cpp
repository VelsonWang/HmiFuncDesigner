
#include <QDebug>

#include "TCPIPModbus.h"
#include "../../Public/PublicFunction.h"


TCPIPModbus::TCPIPModbus()
{

}

TCPIPModbus::~TCPIPModbus()
{

}

/*
* 读线圈状态
*/
ResultType TCPIPModbus::ReadCoils(int addDev, int addCol, int num,
                            unsigned char *pbuf)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addCol;


    m_pWriteBuf[len++] = 0x00; //transaction identifier
    m_pWriteBuf[len++] = 0x00; //transaction identifier

    m_pWriteBuf[len++] = 0x00; //protocol identifier
    m_pWriteBuf[len++] = 0x00; //protocol identifier

    m_pWriteBuf[len++] = 0x00; //length field (upper byte) = 0 (since all messages are smaller than 256)
    m_pWriteBuf[len++] = 0x00; //length field (lower byte) = number of bytes following

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x01;
    m_pWriteBuf[len++] = addCol >> 8;
    m_pWriteBuf[len++] = addCol;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen)
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[9 + idx];
            m_pDataBuf[idx] = m_pReadBuf[9 + idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制单个线圈    0xFF00--on, 0x0000--Off
*/
ResultType TCPIPModbus::WriteCoil(int addDev, int addCol, int value)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x05;
    m_pWriteBuf[len++] = addCol >> 8;
    m_pWriteBuf[len++] = addCol;

    if(value)
    {
        m_pWriteBuf[len++] = 0xFF;
        m_pWriteBuf[len++] = 0x00;
    }
    else
    {
        m_pWriteBuf[len++] = 0x00;
        m_pWriteBuf[len++] = 0x00;
    }

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, len, 1000);

    if(resultlen == len)
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制多个线圈
*/
ResultType TCPIPModbus::WriteMultipleCoils(int addDev, int addCol,
                                     int num, int cntByte, unsigned char *pbuf)
{
    unsigned char len = 0;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x0F;
    m_pWriteBuf[len++] = addCol >> 8;
    m_pWriteBuf[len++] = addCol;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;
    m_pWriteBuf[len++] = cntByte;

    for(int i = 0; i < cntByte; i++)
    {
        m_pWriteBuf[len++] = pbuf[i];  // array[0] coil0--7, array[1]--coil8-15
    }

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 12, 1000);

    if(resultlen == 12)
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* read input bit status
*/
ResultType TCPIPModbus::ReadDiscreteInputs(int addDev, int addinput, int num,
                              unsigned char *pbuf)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addinput;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x02;
    m_pWriteBuf[len++] = addinput >> 8;
    m_pWriteBuf[len++] = addinput;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen)
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[9+idx];
            m_pDataBuf[idx] = m_pReadBuf[9+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读输入寄存器
*/
ResultType TCPIPModbus::ReadReadInputRegister(int addDev, int addReg, int num,
                                            unsigned char *pbuf)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addReg;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x04;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen)
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[9+idx];
            m_pDataBuf[idx] = m_pReadBuf[9+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读保持寄存器
*/
ResultType TCPIPModbus::ReadHoldingRegister(int addDev, int addReg, int num,
                                          unsigned char *pbuf)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addReg;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x03;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen)
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[9+idx];
            m_pDataBuf[idx] = m_pReadBuf[9+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 预置单个寄存器
*/
ResultType TCPIPModbus::WriteHoldingRegister(int addDev, int addReg, unsigned short data)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x06;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;

    if(get2BytesOrder() == "21")
    {
        m_pWriteBuf[len++] = data >> 8;
        m_pWriteBuf[len++] = data;
    }
    else if(get2BytesOrder() == "12")
    {
        m_pWriteBuf[len++] = data;
        m_pWriteBuf[len++] = data >> 8;
    }

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 12, 1000);

    if(resultlen == 12)
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制多个寄存器
*/
ResultType TCPIPModbus::WriteMultipleHoldingRegister(int addDev, int addReg,
                                                   int num, unsigned char *pbuf)
{
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = 0x00;
    m_pWriteBuf[len++] = 0x00;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x10;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;
    m_pWriteBuf[len++] = num*2;

    for(int i = 0; i < num*2; i++)
    {
        m_pWriteBuf[len++] = pbuf[i];
    }

    m_pWriteBuf[5] = len - 6;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 12, 1000);

    if(resultlen == 12)
    {
        return SUCCESS;
    }

    return FAIL;
}


ResultType TCPIPModbus::WriteIntToHoldingRegister(int addDev, int addReg,
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


ResultType TCPIPModbus::WriteUIntToHoldingRegister(int addDev, int addReg,
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


ResultType TCPIPModbus::WriteFloatToHoldingRegister(int addDev, int addReg,
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

