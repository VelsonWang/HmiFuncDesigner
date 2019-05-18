
#include "ModbusRTU.h"
#include "DataPack.h"
#include "../../Public/PublicFunction.h"

const unsigned char auchCRCHi[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
} ;

const unsigned char auchCRCLo[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5,
    0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B,
    0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE,
    0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6,
    0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8,
    0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21,
    0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A,
    0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
    0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51,
    0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D,
    0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
} ;

ModbusRTU::ModbusRTU()
{

}

ModbusRTU::~ModbusRTU()
{

}

unsigned short ModbusRTU::CRC16(unsigned char *pbuf, int len)
{
    unsigned char uchCRCHi = 0xFF ;
    unsigned char uchCRCLo = 0xFF ;
    int uIndex ;
    while (len--)
    {
        uIndex = uchCRCHi ^ *pbuf++ ;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }
    return (uchCRCHi << 8 | uchCRCLo) ;
}


bool ModbusRTU::MessageCheck(unsigned char *inBuf, short bufLen)
{
    unsigned short revCRC16,calCRC16;

    calCRC16 = CRC16(inBuf, bufLen-2);
    memcpy(&revCRC16, inBuf+bufLen-2, 2);
    RecoverSelfData((unsigned char *)&revCRC16, 2);

    if(calCRC16 == revCRC16)
    {
        return true;
    }

    return false;
}



/*
* 读线圈状态
*/
ResultType ModbusRTU::ReadCoils(int addDev, int addCol, int num,
                            unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addCol;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x01;
    m_pWriteBuf[len++] = addCol >> 8;
    m_pWriteBuf[len++] = addCol;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 5 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[3+idx];
            m_pDataBuf[idx] = m_pReadBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制单个线圈    0xFF00--on, 0x0000--Off
*/
ResultType ModbusRTU::WriteCoil(int addDev, int addCol, int value)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));

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

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

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
ResultType ModbusRTU::WriteMultipleCoils(int addDev, int addCol,
                                     int num, int cntByte, unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

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

    tmpCRC16 = CRC16(m_pWriteBuf, len);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 8, 1000);

    if(resultlen == 8 && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* read input bit status
*/
ResultType ModbusRTU::ReadDiscreteInputs(int addDev, int addinput, int num,
                              unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addinput;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x02;
    m_pWriteBuf[len++] = addinput >> 8;
    m_pWriteBuf[len++] = addinput;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num/8;
    if(num%8)databuflen++;

    int iWaitLen = 5 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[3+idx];
            m_pDataBuf[idx] = m_pReadBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读输入寄存器
*/
ResultType ModbusRTU::ReadReadInputRegister(int addDev, int addReg, int num,
                                            unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addReg;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x04;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 5 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[3+idx];
            m_pDataBuf[idx] = m_pReadBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 读保持寄存器
*/
ResultType ModbusRTU::ReadHoldingRegister(int addDev, int addReg, int num,
                                          unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

    m_iCurReadAddress = addReg;

    m_pWriteBuf[len++] = addDev;
    m_pWriteBuf[len++] = 0x03;
    m_pWriteBuf[len++] = addReg >> 8;
    m_pWriteBuf[len++] = addReg;
    m_pWriteBuf[len++] = num >> 8;
    m_pWriteBuf[len++] = num;

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int databuflen = num*2;

    int iWaitLen = 5 + databuflen;

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, iWaitLen, 1000);

    if(resultlen == iWaitLen && MessageCheck(m_pReadBuf, resultlen))
    {
        for(int idx=0; idx<databuflen; idx++)
        {
            if(pbuf != 0)
                pbuf[idx] = m_pReadBuf[3+idx];
            m_pDataBuf[idx] = m_pReadBuf[3+idx];
        }
        return SUCCESS;
    }

    return FAIL;
}


/*
* 预置单个寄存器
*/
ResultType ModbusRTU::WriteHoldingRegister(int addDev, int addReg, unsigned short data)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

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

    tmpCRC16 = CRC16(m_pWriteBuf, 6);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 8, 1000);

    if(resultlen == 8 && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


/*
* 强制多个寄存器
*/
ResultType ModbusRTU::WriteMultipleHoldingRegister(int addDev, int addReg,
                                                   int num, unsigned char *pbuf)
{
    unsigned short tmpCRC16;
    unsigned char len = 0;

    BytesSetZero(m_pWriteBuf, sizeof(m_pWriteBuf)/sizeof(unsigned char));
    BytesSetZero(m_pReadBuf, sizeof(m_pReadBuf)/sizeof(unsigned char));
    BytesSetZero(m_pDataBuf, sizeof(m_pDataBuf)/sizeof(unsigned char));

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

    tmpCRC16 = CRC16(m_pWriteBuf, len);
    RecoverSelfData((unsigned char*)&tmpCRC16, 2);

    m_pWriteBuf[len++] = tmpCRC16;
    m_pWriteBuf[len++] = tmpCRC16 >> 8;

    if(GetPort() != nullptr)
        GetPort()->write(m_pWriteBuf, len, 1000);

    int resultlen = 0;
    if(GetPort() != nullptr)
        resultlen = GetPort()->read(m_pReadBuf, 8, 1000);

    if(resultlen == 8 && MessageCheck(m_pReadBuf, resultlen))
    {
        return SUCCESS;
    }

    return FAIL;
}


ResultType ModbusRTU::WriteIntToHoldingRegister(int addDev, int addReg,
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


ResultType ModbusRTU::WriteUIntToHoldingRegister(int addDev, int addReg,
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


ResultType ModbusRTU::WriteFloatToHoldingRegister(int addDev, int addReg,
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



