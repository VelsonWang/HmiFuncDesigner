#include "TCPIPModbus.h"
#include "../../Public/PublicFunction.h"
#include <string.h>
#include <stdlib.h>

#include <QDebug>


TCPIPModbus::TCPIPModbus(QObject *parent)
    : Modbus(parent)
{

}

TCPIPModbus::~TCPIPModbus()
{

}


quint16 TCPIPModbus::makeMessagePackage(quint8 *pSendData,
                                        IOTag* pTag,
                                        TModbus_ReadWrite RW_flag,
                                        quint16 *retVarLen)
{
    quint16 mesPi = 0;
    quint32 tmpDataPos = 0;
    quint32 tmpUnit = 0;
    quint8 byteCount = 0, tmpLen = 0;

    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

    memset(tempBuffer_, 0, sizeof(tempBuffer_)/sizeof(quint8 ));

    mesPi=0;

    tempBuffer_[mesPi++] = 0x00; //transaction identifier
    tempBuffer_[mesPi++] = 0x00; //transaction identifier

    tempBuffer_[mesPi++] = 0x00; //protocol identifier
    tempBuffer_[mesPi++] = 0x00; //protocol identifier

    tempBuffer_[mesPi++] = 0x00; //length field (upper byte) = 0 (since all messages are smaller than 256)
    tempBuffer_[mesPi++] = 0x00; //length field (lower byte) = number of bytes following

    //设备地址
    tempBuffer_[mesPi++] = pTag->GetDeviceAddress();
    //功能代码
    tempBuffer_[mesPi++] = getFuncode(pTag, RW_flag);

    int iRegisterAddress = pTag->GetRegisterAddress();
    int iOffset = pTag->GetOffset();
    tmpDataPos = iRegisterAddress + iOffset;

    //开始地址
    tempBuffer_[mesPi++] = tmpDataPos >> 8;
    tempBuffer_[mesPi++] = tmpDataPos;

    //读取个数
    tmpUnit = getRegNum(pTag);

    //根据读/写方式构造报文
    if(RW_flag == FLAG_READ)
    {
        //计算返回报文长度
        tmpLen = pTag->GetDataTypeLength() + 3; // 3 = 一个设备地址 + 一个功能码 + 一个计数
        tempBuffer_[mesPi++] = tmpUnit >> 8;
        tempBuffer_[mesPi++] = tmpUnit;
    }
    else if(RW_flag == FLAG_WRITE)
    {
        tmpLen = 6; // 6 = 返回从机地址1， 功能代码1， 起始地址2以及强制线圈数2
        byteCount = pTag->GetDataTypeLength();

        if(tempBuffer_[6 + 1] != 0x06 && tempBuffer_[6 + 1] != 0x05) //功能码为10
        {
            tempBuffer_[mesPi++] = tmpUnit >> 8;
            tempBuffer_[mesPi++] = tmpUnit;
            tempBuffer_[mesPi++] = byteCount;
        }

        DBTagObject *pDBTagObject = pTag->GetDBTagObject();
        TTagDataType type = pTag->GetDataType();

        if(cm == CM_3x || cm == CM_4x)
        {
            switch(pTag->GetDataType())
            {
            case TYPE_INT16:
            {
                int val = pDBTagObject->GetWriteData().toInt();
                tempBuffer_[mesPi++] = val >> 8;
                tempBuffer_[mesPi++] = val;
            }break;
            case TYPE_UINT16:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val >> 8;
                tempBuffer_[mesPi++] = val;
            }break;
            case TYPE_INT32:
            {
                int val = pDBTagObject->GetWriteData().toInt();
                tempBuffer_[mesPi++] = val >> 8;
                tempBuffer_[mesPi++] = val;
                tempBuffer_[mesPi++] = val >> 24;
                tempBuffer_[mesPi++] = val >> 16;
            }break;
            case TYPE_UINT32:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val >> 8;
                tempBuffer_[mesPi++] = val;
                tempBuffer_[mesPi++] = val >> 24;
                tempBuffer_[mesPi++] = val >> 16;
            }break;
            case TYPE_FLOAT:
            {
                union unionFloat
                {
                    float ufloat;
                    unsigned char ubytes[4];
                };
                unionFloat uFloat;
                uFloat.ufloat = pDBTagObject->GetWriteData().toFloat();
                tempBuffer_[mesPi++] = uFloat.ubytes[1];
                tempBuffer_[mesPi++] = uFloat.ubytes[0];
                tempBuffer_[mesPi++] = uFloat.ubytes[3];
                tempBuffer_[mesPi++] = uFloat.ubytes[2];
            }break;
            case TYPE_DOUBLE:
            {
                double val = pDBTagObject->GetWriteData().toDouble();
                union unionDouble
                {
                    double udouble;
                    unsigned char ubytes[8];
                };
                unionDouble uDouble;
                uDouble.udouble = val;
                tempBuffer_[mesPi++] = uDouble.ubytes[1];
                tempBuffer_[mesPi++] = uDouble.ubytes[0];
                tempBuffer_[mesPi++] = uDouble.ubytes[3];
                tempBuffer_[mesPi++] = uDouble.ubytes[2];
                tempBuffer_[mesPi++] = uDouble.ubytes[5];
                tempBuffer_[mesPi++] = uDouble.ubytes[4];
                tempBuffer_[mesPi++] = uDouble.ubytes[7];
                tempBuffer_[mesPi++] = uDouble.ubytes[6];
            }break;
            case TYPE_ASCII2CHAR:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val >> 8;
                tempBuffer_[mesPi++] = val;
            }break;
            case TYPE_STRING:
            {

            }break;
            case TYPE_BCD:
            {

            }break;
            }
        } else {
            switch(pTag->GetDataType())
            {
            case TYPE_BOOL:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val;
            }break;
            case TYPE_INT8:
            {
                int val = pDBTagObject->GetWriteData().toInt();
                tempBuffer_[mesPi++] = val & 0x01;
            }break;
            case TYPE_UINT8:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val & 0x01;
            }break;
            case TYPE_INT16:
            {
                int val = pDBTagObject->GetWriteData().toInt();
                tempBuffer_[mesPi++] = val & 0x01;
                tempBuffer_[mesPi++] = (val>>8) & 0x01;
            }break;
            case TYPE_UINT16:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val & 0x01;
                tempBuffer_[mesPi++] = (val>>8) & 0x01;
            }break;
            case TYPE_INT32:
            {
                int val = pDBTagObject->GetWriteData().toInt();
                tempBuffer_[mesPi++] = val & 0x01;
                tempBuffer_[mesPi++] = (val>>8) & 0x01;
                tempBuffer_[mesPi++] = (val>>16) & 0x01;
                tempBuffer_[mesPi++] = (val>>24) & 0x01;
            }break;
            case TYPE_UINT32:
            {
                uint val = pDBTagObject->GetWriteData().toUInt();
                tempBuffer_[mesPi++] = val & 0x01;
                tempBuffer_[mesPi++] = (val>>8) & 0x01;
                tempBuffer_[mesPi++] = (val>>16) & 0x01;
                tempBuffer_[mesPi++] = (val>>24) & 0x01;
            }break;
            case TYPE_FLOAT:
            {
                union unionFloat
                {
                    float ufloat;
                    unsigned char ubytes[4];
                };
                unionFloat uFloat;
                uFloat.ufloat = pDBTagObject->GetWriteData().toFloat();
                for(int i=0; i<4; i++)
                    tempBuffer_[mesPi++] = uFloat.ubytes[i];
            }break;
            }

            if(pTag->GetDataTypeLength() <= 1)
            {
                // 增加功能码为05写BOOL的操作
                if(tempBuffer_[6 + 1] == 0x05)
                {
                    if(tempBuffer_[6 + 4] == 0x01)
                    {
                        tempBuffer_[6 + 4] = 0xFF;
                    }
                    else
                    {
                        tempBuffer_[6 + 4] = 0x00;
                    }
                    tempBuffer_[6 + 5] = 0x00;
                    mesPi++;
                }
            }
        }
    }

    tempBuffer_[5] = mesPi - 6;
    memcpy(pSendData, tempBuffer_, mesPi);
    *retVarLen = tmpLen + 6;

    return mesPi;
}

/**
 * @brief TCPIPModbus::isCanWrite
 * @details 判断该变量定义属性是否可以写
 * @param pTag 设备变量
 * @return false-不可写，true-可写
 */
bool TCPIPModbus::isCanWrite(IOTag* pTag) {
    if(getCpuMem(pTag->GetRegisterArea()) == CM_1x)
        return false;
    else if(getCpuMem(pTag->GetRegisterArea()) == CM_3x) {
        return false;
    }
    return true;
}


/**
 * @brief TCPIPModbus::writeData
 * @details 写一个变量值到plc设备
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int TCPIPModbus::writeData(IOTag* pTag) {
    quint16 msgLen = 0, revLen = 0;

    memset(writeDataBuffer_, 0, sizeof(writeDataBuffer_)/sizeof(quint8));
    memset(readDataBuffer_, 0, sizeof(readDataBuffer_)/sizeof(quint8));

    msgLen = makeMessagePackage(writeDataBuffer_, pTag, FLAG_WRITE, &revLen);

    if(getPort() != nullptr)
        getPort()->write(writeDataBuffer_, msgLen, 1000);

    int resultlen = 0;
    if(getPort() != nullptr)
        resultlen = getPort()->read(readDataBuffer_, revLen, 1000);

    if(resultlen == revLen && readDataBuffer_[0] == writeDataBuffer_[0])
        return 1;

    return 0;
}


/**
 * @brief TCPIPModbus::isCanRead
 * @details 判断该变量定义属性是否可以读
 * @param pTag 设备变量
 * @return false-不可读，true-可读
 */
bool TCPIPModbus::isCanRead(IOTag* pTag) {
    Q_UNUSED(pTag)
    return true;
}


/**
 * @brief TCPIPModbus::readData
 * @details 从plc设备读一个变量
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int TCPIPModbus::readData(IOTag* pTag) {
    quint16 retSize = 0, msgLen = 0, revLen = 0;
    qint16 i = 0, j = 0;
    quint32 wDataLen = 0;

    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

    memset(writeDataBuffer_, 0, sizeof(writeDataBuffer_)/sizeof(quint8));
    memset(readDataBuffer_, 0, sizeof(readDataBuffer_)/sizeof(quint8));

    msgLen = makeMessagePackage(writeDataBuffer_, pTag, FLAG_READ, &revLen);

    if(getPort() != nullptr)
        getPort()->write(writeDataBuffer_, msgLen, 1000);

    int resultlen = 0;

    if(cm == CM_0x || cm == CM_1x) {
        if(getPort() != nullptr)
            resultlen = getPort()->read(readDataBuffer_, 9, 1000);

        if(resultlen != 9)
            return -2;

        if(getPort() != nullptr)
            resultlen = getPort()->read(&readDataBuffer_[9], readDataBuffer_[8], 1000);

        if(resultlen != readDataBuffer_[8])
            return -2;

        if(pTag->GetDataType() == TYPE_BOOL) {
            if(readDataBuffer_[8] > 1) {
                revLen = revLen + readDataBuffer_[8] - 1;
            }
        }
    } else {

        if(getPort() != nullptr)
            resultlen = getPort()->read(readDataBuffer_, revLen, 1000);

        if(resultlen != revLen)
            return -2;
    }

    memset(tempBuffer_, 0, sizeof(tempBuffer_)/sizeof(quint8 ));

    // 返回数据的处理
    if(pTag->GetDataType() == TYPE_BOOL) {
        retSize = 1;
        pTag->pReadBuf[0] = readDataBuffer_[9] & 0x01;
        wDataLen = retSize;
    } else if(pTag->GetDataType() == TYPE_INT16 || pTag->GetDataType() == TYPE_UINT16) {
        if(getFuncode(pTag, FLAG_READ) == 0x03 || getFuncode(pTag, FLAG_READ) == 0x04) {
            pTag->pReadBuf[0] = readDataBuffer_[10];
            pTag->pReadBuf[1] = readDataBuffer_[9];
        } else {
            pTag->pReadBuf[0] = readDataBuffer_[9];
            pTag->pReadBuf[1] = readDataBuffer_[10];
        }

        wDataLen=2;
    } else if(pTag->GetDataType() == TYPE_UINT32 || pTag->GetDataType() == TYPE_INT32 ||
            pTag->GetDataType() == TYPE_FLOAT) {
        if(getFuncode(pTag, FLAG_READ) == 0x03 || getFuncode(pTag, FLAG_READ) == 0x04) {
            pTag->pReadBuf[0] = readDataBuffer_[10];
            pTag->pReadBuf[1] = readDataBuffer_[9];
            pTag->pReadBuf[2] = readDataBuffer_[12];
            pTag->pReadBuf[3] = readDataBuffer_[11];
        } else {
            pTag->pReadBuf[0] = readDataBuffer_[9];
            pTag->pReadBuf[1] = readDataBuffer_[10];
            pTag->pReadBuf[2] = readDataBuffer_[11];
            pTag->pReadBuf[3] = readDataBuffer_[12];
        }
        wDataLen = 4;
    } else if(pTag->GetDataType() == TYPE_UINT8 || pTag->GetDataType() == TYPE_INT8) {
        retSize = readDataBuffer_[8];

        if(getFuncode(pTag, FLAG_READ) == 0x01 || getFuncode(pTag, FLAG_READ) == 0x02) {
            j = retSize-1;
            for(i=0; i<retSize; i++)
                *(pTag->pReadBuf + (j--)) = readDataBuffer_[9+i];
        } else {
            j = retSize/2-1;
            for(i=0;i<retSize;i++,j--) {
                *(pTag->pReadBuf + 2 * j) = readDataBuffer_[9+i];
                i++;
                *(pTag->pReadBuf + 2 * j + 1) = readDataBuffer_[9+i];
            }
        }

        wDataLen = retSize;
    }

    return 1;
}


