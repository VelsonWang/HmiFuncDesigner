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

    memset(readDataBuffer_, 0, sizeof(readDataBuffer_)/sizeof(quint8));
    memset(writeDataBuffer_, 0, sizeof(writeDataBuffer_)/sizeof(quint8));
}

Modbus::~Modbus()
{

}


void Modbus::setPort(IPort *pPort) {
    if(pPort != nullptr)
        iFacePort_ = pPort;
}


IPort *Modbus::getPort() {
    return iFacePort_;
}


TModbus_CPUMEM Modbus::getCpuMem(const QString &szRegisterArea) {
    if(szRegisterArea == QObject::tr("DO线圈"))
        return CM_0x;
    else if(szRegisterArea == QObject::tr("DI离散输入寄存器"))
        return CM_1x;
    else if(szRegisterArea == QObject::tr("AI输入寄存器"))
        return CM_3x;
    else if(szRegisterArea == QObject::tr("AO保持寄存器"))
        return CM_4x;
    else
        return CM_NON;
}

/**
 * @brief getFuncode
 * @details 获取功能码
 * @param pTag 变量对象
 * @param rw_flag 1-读, 2-写
 * @return 功能码
 */
quint8 Modbus::getFuncode(IOTag* pTag, TModbus_ReadWrite rw_flag)
{
    quint8 byRet = 0;
    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

    if(rw_flag == FLAG_READ)
    {
        switch(cm)
        {
        case CM_0x:
            byRet = 0x01;
            break;
        case CM_1x:
            byRet = 0x02;
            break;
        case CM_3x:
            byRet = 0x04;
            break;
        case CM_4x:
            byRet = 0x03;
            break;
        }
    }
    else if(rw_flag == FLAG_WRITE)
    {
        switch(cm)
        {
        case CM_0x:
            if(pTag->GetDataTypeLength() == 1)
            {
                byRet = 0x05;
            }
            else
            {
                byRet = 0x0F;
            }
            break;
        case CM_1x:
            //不能写
            break;
        case CM_3x:
            //不能写
            break;
        case CM_4x:
            if(pTag->GetDataTypeLength() == 2)
            {
                byRet = 0x06;
            }
            else
            {
                byRet = 0x10;
            }
            break;
        }
    }
    return byRet;
}


/**
 * @brief Modbus::getRegNum
 * @details 获得寄存器个数
 * @param pTag pTag 变量对象
 * @return 寄存器个数
 */
quint16 Modbus::getRegNum(IOTag* pTag)
{
    quint16 byRet = 0;
    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());
    switch(cm)
    {
    case CM_0x:
    case CM_1x:
        if(pTag->GetDataType() == TYPE_BOOL)
            byRet = pTag->GetDataTypeLength();
        else
            byRet = pTag->GetDataTypeLength() * 8;
        break;
    case CM_3x:
    case CM_4x:
        byRet = pTag->GetDataTypeLength() / 2;
        break;
    }

    return byRet;
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

#if 0

#include "MODICON_QUANTUM.h"
#include "PlcPortTrans.h"
#include "DataPack.h"

#include "../../IOSLayer/OSFunc.h"

#include <string.h>

BOOL ABB_Parameter144 = FALSE;
BOOL ABB_Parameter403 = FALSE;
/************************************
TPubVarPlcCommArg字段特殊定义说明:
*************************************
dwDataPos:
    针对指针域或字符串: 必须是1 + 16 * n, n = 0, 1, 2......
RepeatLen
    针对指针域或字符串: [0x, 1x, 3x, 4x]内存区RepeatLen必须为4的倍数
                        [7x, 8x, 9x]内存区RepeatLen必须为2的倍数
************************************/

typedef enum _TModicon_Quantum_CPUMEM
{
    CM_NON  =   0,
    CM_0x 	=   1,
    CM_1x	=   2,
    CM_3x 	=   3,
    CM_4x   =   4,
    CM_7x   =   5,
    CM_8x   =   6,
    CM_9x   =   7
}TModicon_Quantum_CPUMEM;


//Modicon_Quantum型号协议参数数据结构定义
typedef enum _TModicon_Quantum_SLC_500_CPUTYPE
{
    SLC_CPU = 0
}TModicon_Quantum_SLC_500_CPUTYPE;

//等待PLC应答最大时间
#define WAIT_REPLY_MAX_TIME			(500 * PF_CLOCK_TICK_ADJ)

#define MAX_WRITE_DATA_LEN	255	//指针域写类型变量最大数据长度(字节)
#define MAX_READ_DATA_LEN	255	//指针域读类型变量最大数据长度(字节)
#define R_flag  1
#define W_flag  2
#define RTU     0
#define ASC     1

/* CRC 高位字节值表 */

static _BYTE auchCRCHi[] = {
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
    0x00, 0xC1, 0x81, 0x40} ;

/* CRC低位字节值表*/

static _BYTE auchCRCLo[] = {
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
    0x41, 0x81, 0x80, 0x40} ;
//------------------------------------------------------------------------
// 函数名:_BYTE CRC16(puchMsg, usDataLen)
// unsigned char *puchMsg ; /* 要进行CRC校验的消息 */
//
// unsigned short usDataLen ; /* 消息中字节数 */
// 功能描述: 计算LRC校验码
// Write by Rockey 2006.3.2
//------------------------------------------------------------------------

unsigned short CRC16(_BYTE *puchMsg, short usDataLen)
{

    _BYTE uchCRCHi = 0xFF ; /* 高CRC字节初始化 */

    _BYTE uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */

    short uIndex ; /* CRC循环中的索引 */

    while (usDataLen--) /* 传输消息缓冲区 */

    {

        uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */

        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;

        uchCRCLo = auchCRCLo[uIndex] ;

    }

    return (uchCRCHi << 8 | uchCRCLo) ;

}

//------------------------------------------------------------------------
// 函数名:_BYTE LRC(_BYTE *inBuf,short bufLen)
// unsigned char *auchMsg ; /* 要进行计算的消息 */
// unsigned short usDataLen ; /* LRC 要处理的字节的数量*/RC
// 功能描述: 计算LRC校验码
// Write by Rockey 2006.3.2
//------------------------------------------------------------------------

static _BYTE LRC(_BYTE *auchMsg,short usDataLen)
{
    _BYTE uchLRC = 0 ;                           /* LRC 字节初始化 */

    while (usDataLen--)                          /* 完成整个报文缓冲区 */
        uchLRC += *auchMsg++ ;                   /* 缓冲区字节相加，无进位 */
    return ((unsigned char)(-((char)uchLRC))) ;  /* 返回二进制补码 */

}

//判断该变量定义属性是否可以写
static BOOL Modicon_Quantum_IsCanWrite(PPubVarPlcCommArg pVarPlcArg)
{
    size_t iDataLen = 0;

    iDataLen = GetHmiDataFmtLen(pVarPlcArg);

    if((iDataLen > MAX_WRITE_DATA_LEN) || (iDataLen == 0))
        return FALSE;

    return TRUE;
}

//功能代码选择函数
static _BYTE getFuncode(PPubVarPlcCommArg pFunPlcArg,unsigned short RW_flag)
{
    /*0x,1x 有BOOL 类型*/

    _BYTE byRet = 0;

    if(RW_flag == R_flag)
    {
        switch(pFunPlcArg->cm)
        {

        case CM_0x:
            byRet = 0x01;
            break;
        case CM_1x:
            byRet = 0x02;
            break;
        case CM_3x:
            byRet = 0x04;
            break;
        case CM_4x:
            byRet = 0x03;
            break;
        case CM_7x:
            byRet = 0x03;
            break;
        case CM_8x:
            byRet = 0x03;
            break;
        case CM_9x:
            byRet = 0x03;
            break;

        }
    }
    else if(RW_flag == W_flag)
    {
        switch(pFunPlcArg->cm)
        {

        case CM_0x:
            if( GetHmiDataFmtLen(pFunPlcArg) == 1 )
            {
                byRet = 0x05;
            }
            else
            {
                byRet = 0x0F;
            }
            break;
        case CM_1x:
            //不能写
            break;
        case CM_3x:
            //不能写
            break;
        case CM_4x:
            if( GetHmiDataFmtLen(pFunPlcArg) == 2 )
            {
                byRet = 0x06;
            }
            else
            {
                byRet = 0x10;
            }
            break;
        case CM_7x:
            byRet = 0x03;
            break;
        case CM_8x:
            byRet = 0x03;
            break;
        case CM_9x:
            byRet = 0x03;
            break;

        }
    }
    return byRet;
}
static UINT getRegNum(PPubVarPlcCommArg pgetRebNumPlcArg)
{
    /*0x,1x 有BOOL 类型*/
    UINT byRet = 0;
    switch(pgetRebNumPlcArg->cm)
    {
    case CM_0x:
    case CM_1x:
        if(pgetRebNumPlcArg->PlcDataType == PDP_BIT)
            byRet = pgetRebNumPlcArg->RepeatLen;
        else
            byRet = GetHmiDataFmtLen(pgetRebNumPlcArg)*8;
        break;
    case CM_3x:
    case CM_4x:
        if(pgetRebNumPlcArg->PlcDataType == PDP_BIT)
            byRet = 1; //功能码为3、4 时对位时特殊处理 报文以字的接收 zuyuan add 20071010
        else
            byRet = GetHmiDataFmtLen(pgetRebNumPlcArg)/2;

        break;
    }

    return byRet;

}


/**********************************************************
* modbus 报文构造函数
*
* unsigned short RW_flag;Read /Write Flag, read:1;write:2;
* return value: message package length .
* Write by Rockey 060823
***********************************************************/
static short Modbus_MessageStructure(PBYTE pSendData,PPubVarPlcCommArg pPlcArg, PModicon_Quantum_ExtArg pExtArg,unsigned short RW_flag,unsigned short *retVarLen)
{

    unsigned short mesPi;
    unsigned short tmpCRC16;
    unsigned long tmpDataPos;
    unsigned int tmpUnit;
    unsigned short spi;
    _BYTE tmpLRC,byteCount,tmpLen;
    short i,j;
    memset(_TempBuffer, 0, COMM_TEMP_BUF_SIZE);

    mesPi=0;
    //设备地址
    _TempBuffer[mesPi++] = pPlcArg->wAddress;
    //功能代码
    _TempBuffer[mesPi++] = getFuncode(pPlcArg,RW_flag);

    tmpDataPos = pPlcArg->dwDataPos - 1;
    RecoverSelfData(&tmpDataPos,2);
    //开始地址
    memcpy(_TempBuffer+mesPi, &tmpDataPos,2);
    mesPi+=2;

    //读取个数
    tmpUnit = getRegNum(pPlcArg);

    //根据读/写方式构造报文
    if(RW_flag == R_flag)
    {
        //计算返回报文长度
        tmpLen = GetHmiDataFmtLen(pPlcArg) + 3;// 3 = 一个设备地址+一个功能码 + 一个计数

        //功能码为3、4 时对位时特殊处理 报文以字的接收 zuyuan add 20071010
        if( pPlcArg->PlcDataType == PDP_BIT && (pPlcArg->cm == CM_3x || pPlcArg->cm == CM_4x) )
        {
            tmpLen += 1;
        }

        RecoverSelfData(&tmpUnit,2);

        memcpy(_TempBuffer+mesPi, &tmpUnit, 2);
        mesPi+=2;
    }
    else if(RW_flag == W_flag)
    {
        //_CommTempBuffer[mesPi] = pPlcArg->pData;

        tmpLen = 6;// 6 = 返回从机地址1，功能代码1，起始地址2以及强制线圈数2
        byteCount = GetHmiDataFmtLen(pPlcArg);

        if(_TempBuffer[1] != 0x06 && _TempBuffer[1] != 0x05) //功能码为10
        {
            RecoverSelfData(&tmpUnit,2);
            memcpy(_TempBuffer+mesPi, &tmpUnit, 2);
            mesPi+=2;
            _TempBuffer[mesPi++] = byteCount;
        }

        if(pPlcArg->cm == CM_3x || pPlcArg->cm == CM_4x)
        {
            if((pPlcArg->PlcDataType == PDP_DWORD || pPlcArg->PlcDataType == PDP_FLOAT))
            {
                for(i=0;i<byteCount;i++)
                {
                    _TempBuffer[mesPi++]=pPlcArg->pData[i+1];
                    _TempBuffer[mesPi++]=pPlcArg->pData[i];
                    i++;
                }
            }
            else if(pPlcArg->PlcDataType == PDP_BYTE)
            {

                j = byteCount/2;
                if( pPlcArg->byDataFormat == DP_POINT_AREA)
                {
                    for(i=byteCount-1;i>=0;i--)
                    {
                        _TempBuffer[mesPi++]=pPlcArg->pData[i-1];
                        _TempBuffer[mesPi++]=pPlcArg->pData[i];
                        i--;
                    }
                }
                else
                {
                    for(i=0;i<j;i++)
                    {
                        _TempBuffer[mesPi++] = pPlcArg->pData[2*i+1];
                        _TempBuffer[mesPi++] = pPlcArg->pData[2*i];

                    }
                }
            }
            else
            {
                for(i=byteCount-1;i>=0;i--)
                    _TempBuffer[mesPi++]=pPlcArg->pData[i];
            }
        }
        else
        {
            if(pPlcArg->PlcDataType == PDP_WORD || pPlcArg->PlcDataType == PDP_DWORD ||  pPlcArg->PlcDataType == PDP_FLOAT)
            {
                for(i=0;i<byteCount;i++)
                    _TempBuffer[mesPi++]=pPlcArg->pData[i];
            }
            else
            {
                for(i=byteCount-1;i>=0;i--)
                    _TempBuffer[mesPi++]=pPlcArg->pData[i];

                // 增加功能码为05写BOOL的操作
                if(_TempBuffer[1] == 0x05)
                {
                    if(_TempBuffer[4] == 0x01)
                    {
                        _TempBuffer[4] = 0xFF;
                    }
                    else
                    {
                        _TempBuffer[4] = 0x00;
                    }
                    _TempBuffer[5] = 0x00;
                    mesPi++;
                }
            }
        }
    }

    //根据串行传输模式进行相应转换
    if(pExtArg->VerifyMode==ASC)
    {
        tmpLRC = LRC(_TempBuffer,mesPi);
        RecoverSelfData(&tmpLRC,1);
        memcpy(_TempBuffer+mesPi, &tmpLRC, 1);
        mesPi++;

        spi = 0;

        pSendData[spi++] = 0x3A;

        MakeCodeToAsii(_TempBuffer,pSendData+spi,mesPi);
        spi = spi+2*mesPi;

        pSendData[spi++] = 0x0D;
        pSendData[spi++] = 0x0A;
        mesPi = spi;

        *retVarLen = tmpLen*2 + 1 +2 +2;//一个头'0x3a'，两个校验，两个结束'0x0d 0x0a'
    }
    else if(pExtArg->VerifyMode==RTU)
    {
        memcpy(pSendData,_TempBuffer,mesPi);
        tmpCRC16 = CRC16(pSendData,mesPi);

        RecoverSelfData(&tmpCRC16,2);
        memcpy(pSendData+mesPi,&tmpCRC16,2);
        mesPi+=2;

        *retVarLen = tmpLen +2;
    }

    return mesPi;
}

static BOOL Modbus_MessageCheck(_BYTE *inBuf,short bufLen,PModicon_Quantum_ExtArg pExtArg)
{
    unsigned short revCRC16,calCRC16;
    short len;

    _BYTE cLRC,sLRC;

    if(pExtArg->VerifyMode==ASC)
    {
        len=bufLen-1;

        cLRC=0;
        cLRC=LRC(inBuf,len);

        memcpy(&sLRC,&inBuf[len],1);
        if(cLRC==sLRC)
            return TRUE;
        else
            return FALSE;
    }
    else if(pExtArg->VerifyMode==RTU)
    {
        calCRC16 = CRC16(inBuf,bufLen-2);
        memcpy(&revCRC16,inBuf+bufLen-2,2);
        RecoverSelfData(&revCRC16,2);

        if(calCRC16 == revCRC16)
            return TRUE;
        else
            return FALSE;

    }

    return FALSE;
}



//写一个变量值到plc设备
//返回值遵守PlcCommInterface.h文件中CMD_PVCP_WRITEQUE_EXECUTE命令码定义
static int Modicon_Quantum_WriteData(PPubVarPlcCommArg pVarPlcArg, PModicon_Quantum_ExtArg pExtArg)
{
    PBYTE pVailableData = _TempBuffer;
    unsigned short msgLen,revLen;

    pVarPlcArg->bRet = FALSE;

    memset(_VailableSendData, 0, MAX_READ_DATA_LEN);
    memset(_wRDataBuf, 0, DEVCOMM_REC_BUFFER);

    msgLen = Modbus_MessageStructure(_VailableSendData,pVarPlcArg,pExtArg,W_flag,&revLen);

    //清空接收缓冲区
    ClearReadBuf();
    //发送读命令帧
    if(!PlcSendData(_VailableSendData, msgLen, TRUE))
        return -2;

    if(pExtArg->VerifyMode==ASC)
    {
        /*if(!PlcReadData(_wRDataBuf, 11, WAIT_REPLY_MAX_TIME))
            return -2;

        if(_wRDataBuf[3]>='8')
            return -2;

        if(revLen-11 > 0)
        {
            if(!PlcReadData(_wRDataBuf+11, revLen-11, WAIT_REPLY_MAX_TIME))
                return -2;
        }*/

        if(!PlcReadData(_wRDataBuf, revLen, WAIT_REPLY_MAX_TIME))
            return -2;

        memcpy(pVailableData,_wRDataBuf,revLen);
        MakeAsiiToCode(pVailableData+1, _wRDataBuf, revLen);
        revLen = (revLen-3)/2;
    }
    else if(pExtArg->VerifyMode==RTU)
    {
        /*if(!PlcReadData(_wRDataBuf, 5, WAIT_REPLY_MAX_TIME))
            return -2;

        if(_wRDataBuf[2]>= 0x80)
            return -2;

        if(revLen-11 > 0)
        {
            if(!PlcReadData(_wRDataBuf+5, revLen-5, WAIT_REPLY_MAX_TIME))
                return -2;
        }*/

        if(!PlcReadData(_wRDataBuf, revLen, WAIT_REPLY_MAX_TIME))
            return -2;
    }


    //校验返回信息
    if(!Modbus_MessageCheck(_wRDataBuf, revLen,pExtArg))
        return 0;

    pVarPlcArg->bRet = TRUE;
    return 0;

}

//判断该变量定义属性是否可以读
static BOOL Modicon_Quantum_IsCanRead(PPubVarPlcCommArg pVarPlcArg,PModicon_Quantum_ExtArg pExtArg)
{
    size_t iDataLen = 0;

    iDataLen = GetHmiDataFmtLen(pVarPlcArg);

    if((iDataLen > MAX_READ_DATA_LEN) || (iDataLen == 0))
        return FALSE;

    return TRUE;
}

//判断是否已经块读溢出
//返回值遵守PlcCommInterface.h文件中CMD_PVCP_ISREADQUE_OVERFLOW命令码定义
static int Modicon_Quantum_IsReadQueOverFlow(PPubVarPlcCommArg *ppVarPlcArg, int iCount)
{
    return 0;
}

//写一个变量值到plc设备
//返回值遵守PlcCommInterface.h文件中CMD_PVCP_READQUE_EXECUTE命令码定义
static int Modicon_Quantum_ReadQueExecute(PPubVarPlcCommArg *ppVarPlcArg, PModicon_Quantum_ExtArg *pExtArg, int iCount)
{

    //可用全局缓冲区
    ///////////////////////////////////////
    //接transducer收读应答数据
    //_BYTE _rRDataBuf[DEVCOMM_REC_BUFFER];
    //发送读命令帧数据打包
    //_BYTE _VailableRecData[VAILABLE_VAR_DATA];
    //临时用
    //_BYTE _CommTempBuffer[COMM_TEMP_BUF_SIZE];
    ///////////////////////////////////////
    //pExtArg[0]->VerifyMode为数据校验方式
    int i;


    for(i = 0; i < iCount; i ++)
    {
        ppVarPlcArg[i]->bRet = FALSE;
    }

    //.....

    return 0;
}


//------------------------------------------------------------------------
// 函数名:static int Modicon_Quantum_ReadData(pVarPlcArg, _pExtArg)
// 输入:  PPubVarPlcCommArg：协议处理模块的变量数据属性结构
// 输出:
// 功能描述:******从plc设备读一个变量值******
// 全局变量:Null
// Write by Rockey 2006.08.21
//------------------------------------------------------------------------
static int Modicon_Quantum_ReadData(PPubVarPlcCommArg pVarPlcArg, PModicon_Quantum_ExtArg pExtArg)
{
    PBYTE pVailableData = _TempBuffer;
    short retSize,msgLen,revLen,i,j;
    size_t wDataLen;

    OSTimeDlyHMSM(0, 0, 0, 30); //兼容某些仪表通讯慢或通讯中断

    pVarPlcArg->bRet = FALSE;

    memset(_VailableRecData, 0, MAX_READ_DATA_LEN);
    memset(_rRDataBuf, 0, DEVCOMM_REC_BUFFER);

    //构造报文
    msgLen = Modbus_MessageStructure(_VailableRecData,pVarPlcArg,pExtArg,R_flag,&revLen);

    //清空接收缓冲区
    ClearReadBuf();
    //发送读命令帧
    if(!PlcSendData(_VailableRecData, msgLen, TRUE))
        return -2;

    if(pVarPlcArg->cm == CM_0x || pVarPlcArg->cm == CM_1x)
    {
        if(!PlcReadData(_rRDataBuf, 3, WAIT_REPLY_MAX_TIME))
            return -2;
        if(!PlcReadData(_rRDataBuf+3, _rRDataBuf[2]+2, WAIT_REPLY_MAX_TIME))
            return -2;
        if(pVarPlcArg->PlcDataType == PDP_BIT)
        {
            if(_rRDataBuf[2] > 1)
            {
                revLen = revLen + _rRDataBuf[2] - 1;
            }
        }
    }
    else
    {
        if(!PlcReadData(_rRDataBuf, revLen, WAIT_REPLY_MAX_TIME))
            return -2;
    }

    //如果是ASCII模式，先将其转换成对应的RTU
    if(pExtArg->VerifyMode==ASC)
    {
        memcpy(pVailableData,_rRDataBuf,revLen);
        MakeAsiiToCode(pVailableData+1, _rRDataBuf, revLen);
        revLen = (revLen-3)/2;
    }

    //校验返回信息
    if(!Modbus_MessageCheck(_rRDataBuf, revLen,pExtArg))
        return 0;

    //返回数据的处理

    if(pVarPlcArg->PlcDataType == PDP_BIT)
    {
        //功能码为3、4 时对位时特殊处理 报文以字的接收 zuyuan add 20071010
        if(getFuncode(pVarPlcArg,R_flag) == 0x03 || getFuncode(pVarPlcArg,R_flag) == 0x04)
        {
            unsigned short iValue = 0;

            retSize = _rRDataBuf[2];
            *pVailableData=_rRDataBuf[4];
            *(pVailableData+1)=_rRDataBuf[3];

            memcpy(&iValue,pVailableData,2);
            iValue = (iValue >> pVarPlcArg->byBitPos) & 0x01 ;
            memcpy(pVailableData,(unsigned char *)&iValue,2);

            wDataLen=2;
        }
        else
        {
            retSize = 1;//retSize = _rRDataBuf[2];
            pVailableData = &_rRDataBuf[3];
            wDataLen = retSize;
        }
    }
    else if(pVarPlcArg->PlcDataType == PDP_WORD)
    {
        if(getFuncode(pVarPlcArg,R_flag) == 0x03 || getFuncode(pVarPlcArg,R_flag) == 0x04)
        {
            *pVailableData=_rRDataBuf[4];
            *(pVailableData+1)=_rRDataBuf[3];
        }
        else
        {
            *pVailableData=_rRDataBuf[3];
            *(pVailableData+1)=_rRDataBuf[4];
        }

        wDataLen=2;

    }
    else if((pVarPlcArg->PlcDataType == PDP_DWORD) || (pVarPlcArg->PlcDataType == PDP_FLOAT))
    {

        //根据功能选择显示数据的高字低字
        if(getFuncode(pVarPlcArg,R_flag) == 0x03 || getFuncode(pVarPlcArg,R_flag) == 0x04)
        {

            *pVailableData=_rRDataBuf[4];
            *(pVailableData+1)=_rRDataBuf[3];
            *(pVailableData+2)=_rRDataBuf[6];
            *(pVailableData+3)=_rRDataBuf[5];

        }
        else
        {
            *pVailableData=_rRDataBuf[3];
            *(pVailableData+1)=_rRDataBuf[4];
            *(pVailableData+2)=_rRDataBuf[5];
            *(pVailableData+3)=_rRDataBuf[6];
        }

        wDataLen=4;
    }
    else if(pVarPlcArg->PlcDataType == PDP_BYTE)
    {
        retSize = _rRDataBuf[2];

        if((getFuncode(pVarPlcArg,R_flag) == 0x03 && pVarPlcArg->byDataFormat == DP_STRING))
        {
            j=retSize/2;
            for(i=0;i<j;i++)
            {
                *(pVailableData+2*i+1)=_rRDataBuf[3+2*i];
                *(pVailableData+2*i)=_rRDataBuf[3+2*i+1];
            }
        }
        else
        {
            if(getFuncode(pVarPlcArg,R_flag) == 0x01 || getFuncode(pVarPlcArg,R_flag) == 0x02)
            {
                j = retSize-1;
                for(i=0;i<retSize;i++)
                    *(pVailableData+j--)=_rRDataBuf[3+i];
            }
            else
            {
                j=retSize/2-1;
                for(i=0;i<retSize;i++,j--)
                {
                    *(pVailableData+2*j)=_rRDataBuf[3+i];
                    i++;
                    *(pVailableData+2*j+1)=_rRDataBuf[3+i];
                }
            }

        }


        wDataLen=retSize;
    }
    /* 判断是ABB变频器时，对两个时间参数做特殊处理 参数编号为40144和40403 */
    if(pVarPlcArg->plc == 102 && pVarPlcArg->cm == 4 && (pVarPlcArg->dwDataPos == 144 || pVarPlcArg->dwDataPos == 403 ) )
    {
        if(pVarPlcArg->dwDataPos == 144)
            ABB_Parameter144 = TRUE;
        if(pVarPlcArg->dwDataPos == 403)
            ABB_Parameter403 = TRUE;
    }
    memcpy(pVarPlcArg->pData, pVailableData, (size_t)wDataLen);

    pVarPlcArg->bRet = TRUE;
    return 1;
}

static void Modicon_Quantum_ProcessPlcWorkParam(PBYTE pDataValue, BOOL bBfWrite)
{
    return;
}

static int Modicon_Quantum_ProcessBitSeqData(PPubVarPlcCommArg pVarPlcArg, PBYTE pDataValue)
{
    return 0;
}



static void Modicon_Quantum_PreProcessAfterRead(PPubVarPlcCommArg pVarPlcArg)
{
    if(pVarPlcArg->byDataFormat == DP_POINT_AREA)
    {
        if((pVarPlcArg->bySubType == VST_BITSEQU) || (pVarPlcArg->bySubType == VST_XYSEQU))
        {
            RecoverSelfData(pVarPlcArg->pData, pVarPlcArg->RepeatLen);
            Modicon_Quantum_ProcessBitSeqData(pVarPlcArg, pVarPlcArg->pData);
        }
        if((pVarPlcArg->bySubType == VST_SYSDATA) && (pVarPlcArg->byBitSeqDataType == SDF_PLC_WORK))
        {
            RecoverSelfData(pVarPlcArg->pData, pVarPlcArg->RepeatLen);
            Modicon_Quantum_ProcessPlcWorkParam(pVarPlcArg->pData, FALSE);
        }
        else
        {
            if(pVarPlcArg->bySubType == VST_SYSDATA)
            {
                if(pVarPlcArg->byBitSeqDataType == SDF_COMPU_ANANY)
                    RecoverSelfData(pVarPlcArg->pData, pVarPlcArg->RepeatLen);
            }
        }
    }

}



static void Modicon_Quantum_PreProcessBeforeWrite(PPubVarPlcCommArg pVarPlcArg)
{
    if(pVarPlcArg->byDataFormat == DP_POINT_AREA)
    {
        memcpy(_CommTempBuffer, pVarPlcArg->pData, pVarPlcArg->RepeatLen);
        if((pVarPlcArg->bySubType == VST_SYSDATA) && (pVarPlcArg->byBitSeqDataType == SDF_PLC_WORK))
        {
            RecoverSelfData(_CommTempBuffer, pVarPlcArg->RepeatLen);
            Modicon_Quantum_ProcessPlcWorkParam(_CommTempBuffer, TRUE);
        }
        else
        {
            if(pVarPlcArg->bySubType == VST_SYSDATA)
            {
                if(pVarPlcArg->byBitSeqDataType == SDF_COMPU_ANANY)
                    RecoverSelfData(_CommTempBuffer, pVarPlcArg->RepeatLen);
            }
        }

        pVarPlcArg->pData = _CommTempBuffer;
    }

}


/*************************************************************/
//协议操作命令码解释执行函数
int Modicon_Quantum_ProcessVarConnPlc(int CmdCode, PPubVarPlcCommArg pVarPlcArg, PVOID pExtArg, int iExt)
{
    int iRet = 0;
    PModicon_Quantum_ExtArg _pExtArg = (PModicon_Quantum_ExtArg)pExtArg;

    switch(CmdCode)
    {
    case CMD_PVCP_ISCANWRITE:
        iRet = (int)Modicon_Quantum_IsCanWrite(pVarPlcArg);
        break;
    case CMD_PVCP_WRITEDATA:
        Modicon_Quantum_PreProcessBeforeWrite(pVarPlcArg);
        iRet = (int)Modicon_Quantum_WriteData(pVarPlcArg, _pExtArg);
        break;
    case CMD_PVCP_ISCANREAD:
        iRet = (int)Modicon_Quantum_IsCanRead(pVarPlcArg,_pExtArg);
        break;
    case CMD_PVCP_READDATA:
        iRet = (int)Modicon_Quantum_ReadData(pVarPlcArg, _pExtArg);
        if(iRet == 1)
            Modicon_Quantum_PreProcessAfterRead(pVarPlcArg);
        break;
        //case CMD_PVCP_ISREADQUE:
        //	iRet = 1;
        //	break;
        /*case CMD_PVCP_ISREADQUE_OVERFLOW:
        iRet = Modicon_Quantum_IsReadQueOverFlow((PPubVarPlcCommArg *)pVarPlcArg, iExt);
        break;
    case CMD_PVCP_READQUE_EXECUTE:
        iRet = Modicon_Quantum_ReadQueExecute((PPubVarPlcCommArg *)pVarPlcArg,
            (PModicon_Quantum_ExtArg *)_pExtArg, iExt);
        break;	*/
    case CMD_PVCP_ISREADNOGAP:
        iRet = 1;
        break;
    case CMD_PVCP_ISOFFSET_OVERFLOW:
        iRet = DWORD_IsVarOffSetOverFlow(pVarPlcArg, (long)pExtArg, (long)iExt);
        break;
    default:
        break;
    }

    return iRet;
}
/*************************************************************/

/*************************************************************/
//生成调用接口扩展参数函数
void Modicon_Quantum_MakeExtArg(PVOID pPlcArg, PVOID pVarArg, PVOID pExtArg)
{
    PModicon_Quantum_PlcArg pSelfPlcArg = (PModicon_Quantum_PlcArg)pPlcArg;
    PModicon_Quantum_ExtArg pSelfExtArg = (PModicon_Quantum_ExtArg)pExtArg;

    pSelfExtArg->VerifyMode = pSelfPlcArg->VerifyMode;
}
/*************************************************************/



#endif

