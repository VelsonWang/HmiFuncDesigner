
#include <QDebug>

#include "TCPIPModbus.h"
#include "../../Public/PublicFunction.h"


TCPIPModbus::TCPIPModbus()
{
    iFacePort = nullptr;
    m_pReadBuf = new unsigned char[512];
    m_pWriteBuf = new unsigned char[512];
    m_pDataBuf = new unsigned char[512];
    m_iCurReadAddress = 0;
    m_str2BytesOrder = "21";
    m_str3BytesOrder = "321";
    m_str4BytesOrder = "2143";
    m_strFloatBytesOrder = "2143";
}


void TCPIPModbus::SetPort(IPort *pPort)
{
    iFacePort = pPort;
}

TCPIPModbus::~TCPIPModbus()
{
    delete[] m_pReadBuf;
    delete[] m_pWriteBuf;
    delete[] m_pDataBuf;
}


void TCPIPModbus::set2BytesOrder(QString s)
{
    m_str2BytesOrder = s;
}


void TCPIPModbus::set3BytesOrder(QString s)
{
    m_str3BytesOrder = s;
}


void TCPIPModbus::set4BytesOrder(QString s)
{
    m_str4BytesOrder = s;
}


void TCPIPModbus::setFloatBytesOrder(QString s)
{
    m_strFloatBytesOrder = s;
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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, len, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

    if(value) m_pReadBuf[200] = 9;
    else m_pReadBuf[200] = 6;
    WriteMultipleCoils(addDev, addCol, 4, 1, &m_pReadBuf[200]);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, 12, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int databuflen = num*2;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int databuflen = num*2;

    int iWaitLen = 9 + databuflen;

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, iWaitLen, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(m_str2BytesOrder == "21")
    {
        m_pWriteBuf[len++] = data >> 8;
        m_pWriteBuf[len++] = data;
    }
    else if(m_str2BytesOrder == "12")
    {
        m_pWriteBuf[len++] = data;
        m_pWriteBuf[len++] = data >> 8;
    }

    m_pWriteBuf[5] = len - 6;

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, 12, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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

    if(iFacePort != 0)
        iFacePort->write(m_pWriteBuf, len, 1000);
    //ShowBufferHex("tx: ", m_pWriteBuf, len);

    int resultlen = 0;
    if(iFacePort != 0)
        resultlen = iFacePort->read(m_pReadBuf, 12, 1000);
    //ShowBufferHex("rx: ", m_pReadBuf, len);

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
unsigned char TCPIPModbus::GetBoolValue()
{
    return m_pDataBuf[0]&0x01;
}


/*
* get single coil value by read address
*/
unsigned char TCPIPModbus::GetBoolValue(int add)
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


short TCPIPModbus::GetShortValue()
{
    return GetShortValue(m_iCurReadAddress);
}



short TCPIPModbus::GetShortValue(int add)
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



unsigned short TCPIPModbus::GetUnsignedShortValue()
{
    return GetUnsignedShortValue(m_iCurReadAddress);
}



unsigned short TCPIPModbus::GetUnsignedShortValue(int add)
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



int TCPIPModbus::GetIntValue()
{
    return GetIntValue(m_iCurReadAddress);
}



int TCPIPModbus::GetIntValue(int add)
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



unsigned int TCPIPModbus::GetUnsignedIntValue()
{
    return GetUnsignedIntValue(m_iCurReadAddress);
}



unsigned int TCPIPModbus::GetUnsignedIntValue(int add)
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



float TCPIPModbus::GetFloatValue()
{
    return GetFloatValue(m_iCurReadAddress);
}



float TCPIPModbus::GetFloatValue(int add)
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







