#include "TCPIPModbusImpl.h"
#include "DataPack.h"
#include "../../HmiRunTime/Public/PublicFunction.h"
#include "../../HmiRunTime/Vendor.h"

TCPIPModbusImpl::TCPIPModbusImpl()
{

}

TCPIPModbusImpl::~TCPIPModbusImpl()
{

}


quint16 TCPIPModbusImpl::makeMessagePackage(quint8 *pSendData,
                                            void* pObj,
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

    Vendor* pVendorObj = (Vendor*)(pObj);
    quint16 iTransIdentifier = 0;
    if(pVendorObj) {
        iTransIdentifier = pVendorObj->m_extraData[0] + pVendorObj->m_extraData[1] * 256;
        iTransIdentifier = (iTransIdentifier + 1) % 65536;
    }

    tempBuffer_[mesPi++] = (iTransIdentifier >> 8) & 0xFF; //transaction identifier
    tempBuffer_[mesPi++] = iTransIdentifier & 0xFF; //transaction identifier

    if(pVendorObj) {
        pVendorObj->m_extraData[0] =  tempBuffer_[1];
        pVendorObj->m_extraData[1] =  tempBuffer_[0];
    }

    tempBuffer_[mesPi++] = 0x00; //protocol identifier
    tempBuffer_[mesPi++] = 0x00; //protocol identifier

    tempBuffer_[mesPi++] = 0x00; //length field (upper byte) = 0 (since all messages are smaller than 256)
    tempBuffer_[mesPi++] = 0x00; //length field (lower byte) = number of bytes following

    //设备地址
    tempBuffer_[mesPi++] = pTag->GetDeviceAddress();
    //功能代码
    tempBuffer_[mesPi++] = getFuncode(pObj, pTag, RW_flag);

    int iRegisterAddress = pTag->GetRegisterAddress();
    int iOffset = pTag->GetOffset();
    tmpDataPos = iRegisterAddress + iOffset;
    if(pVendorObj != Q_NULLPTR && this->isStartAddrBit(pVendorObj) == false) tmpDataPos -= 1;

    //开始地址
    tempBuffer_[mesPi++] = tmpDataPos >> 8;
    tempBuffer_[mesPi++] = tmpDataPos;

    //读取个数
    tmpUnit = getRegNum(pTag);

    //根据读/写方式构造报文
    if(RW_flag == FLAG_READ) {
        //计算返回报文长度
        tmpLen = tmpUnit * 2 + 3; // 3 = 一个设备地址 + 一个功能码 + 一个计数
        tempBuffer_[mesPi++] = tmpUnit >> 8;
        tempBuffer_[mesPi++] = tmpUnit;
    }
    else if(RW_flag == FLAG_WRITE) {
        tmpLen = 6; // 6 = 返回从机地址1， 功能代码1， 起始地址2以及强制线圈数2
        byteCount = pTag->GetDataTypeLength();

        if(tempBuffer_[6 + 1] != 0x06 && tempBuffer_[6 + 1] != 0x05) //功能码为10
        {
            tempBuffer_[mesPi++] = tmpUnit >> 8;
            tempBuffer_[mesPi++] = tmpUnit;
            tempBuffer_[mesPi++] = byteCount;
        }

        if(cm == CM_3x || cm == CM_4x) {
            switch(pTag->GetDataType()) {
            case TYPE_INT16:
            case TYPE_UINT16: {
                modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->pWriteBuf, 2);
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 2);
                mesPi += 2;
            }break;
            case TYPE_INT32:
            case TYPE_UINT32:
            case TYPE_FLOAT: {
                modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->pWriteBuf, 4);
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 4);
                mesPi += 4;
            }break;
            case TYPE_DOUBLE: {
                modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->pWriteBuf, 8);
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 8);
                mesPi += 8;
            }break;
            case TYPE_ASCII2CHAR: {
                modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->pWriteBuf, 2);
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 2);
                mesPi += 2;
            }break;
            case TYPE_STRING: {

            }break;
            case TYPE_BCD: {

            }break;
            default:{

            }break;
            }
        } else {
            switch(pTag->GetDataType()) {
            case TYPE_BOOL:
            case TYPE_INT8:
            case TYPE_UINT8: {
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 1);
                mesPi += 1;
            }break;
            case TYPE_INT16:
            case TYPE_UINT16: {
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 2);
                RecoverSelfData(&tempBuffer_[mesPi], 2);
                mesPi += 2;
            }break;
            case TYPE_INT32:
            case TYPE_UINT32:
            case TYPE_FLOAT: {
                memcpy(&tempBuffer_[mesPi], pTag->pWriteBuf, 4);
                RecoverSelfData(&tempBuffer_[mesPi], 4);
                mesPi += 4;
            }break;
            default:{

            }break;
            }

            if(pTag->GetDataTypeLength() <= 1) {
                // 增加功能码为05写BOOL的操作
                if(tempBuffer_[6 + 1] == 0x05) {
                    if(tempBuffer_[6 + 4] == 0x01) {
                        tempBuffer_[6 + 4] = 0xFF;
                    } else {
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
 * @brief TCPIPModbusImpl::isCanWrite
 * @details 判断该变量定义属性是否可以写
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可写，true-可写
 */
bool TCPIPModbusImpl::isCanWrite(void* pObj, IOTag* pTag)
{
    Q_UNUSED(pObj)
    if(getCpuMem(pTag->GetRegisterArea()) == CM_1x)
        return false;
    else if(getCpuMem(pTag->GetRegisterArea()) == CM_3x) {
        return false;
    }
    return true;
}


/**
 * @brief TCPIPModbusImpl::writeData
 * @details 写一个变量值到plc设备
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int TCPIPModbusImpl::writeData(void* pObj, IOTag* pTag)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    quint16 msgLen = 0, revLen = 0;

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_WRITE, &revLen);

    if(getPort() != nullptr)
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    int resultlen = 0;
    if(getPort() != nullptr)
        resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    if(resultlen == revLen && pVendorObj->readBuf[0] == pVendorObj->writeBuf[0] &&
            pVendorObj->readBuf[1] == pVendorObj->writeBuf[1] && pVendorObj->readBuf[7] == pVendorObj->writeBuf[7])
        return 1;

    return 0;
}


/**
 * @brief TCPIPModbusImpl::isCanRead
 * @details 判断该变量定义属性是否可以读
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可读，true-可读
 */
bool TCPIPModbusImpl::isCanRead(void* pObj, IOTag* pTag)
{
    Q_UNUSED(pObj)
    Q_UNUSED(pTag)
    return true;
}


/**
 * @brief TCPIPModbusImpl::readData
 * @details 从plc设备读一个变量
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int TCPIPModbusImpl::readData(void* pObj, IOTag* pTag)
{
    quint16 retSize = 0, msgLen = 0, revLen = 0;
    qint16 i = 0, j = 0;
    quint32 wDataLen = 0;
    Vendor* pVendorObj = (Vendor*)(pObj);

    TModbus_CPUMEM cm = getCpuMem(pTag->GetRegisterArea());

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_READ, &revLen);

    if(getPort() != nullptr)
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    int resultlen = 0;

    if(cm == CM_0x || cm == CM_1x) {
        if(getPort() != nullptr)
            resultlen = getPort()->read(pVendorObj->readBuf, 9, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

        if(!(resultlen == 9 && pVendorObj->readBuf[0] == pVendorObj->writeBuf[0] &&
             pVendorObj->readBuf[1] == pVendorObj->writeBuf[1] && pVendorObj->readBuf[7] == pVendorObj->writeBuf[7]))
            return -2;

        if(getPort() != nullptr)
            resultlen = getPort()->read(&pVendorObj->readBuf[9], pVendorObj->readBuf[8], pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

        if(resultlen != pVendorObj->readBuf[8])
            return -2;

        if(pTag->GetDataType() == TYPE_BOOL) {
            if(pVendorObj->readBuf[8] > 1) {
                revLen = revLen + pVendorObj->readBuf[8] - 1;
            }
        }
    } else {
        if(getPort() != nullptr)
            resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

        if(!(resultlen == revLen && pVendorObj->readBuf[0] == pVendorObj->writeBuf[0] &&
             pVendorObj->readBuf[1] == pVendorObj->writeBuf[1] && pVendorObj->readBuf[7] == pVendorObj->writeBuf[7]))
            return -2;
    }

    memset(tempBuffer_, 0, sizeof(tempBuffer_)/sizeof(quint8 ));

    // 返回数据的处理
    if(pTag->GetDataType() == TYPE_BOOL) {
        retSize = 1;
        pTag->pReadBuf[0] = pVendorObj->readBuf[9] & 0x01;
        wDataLen = retSize;
    } else if(pTag->GetDataType() == TYPE_INT16 || pTag->GetDataType() == TYPE_UINT16) {
        if(cm == CM_3x || cm == CM_4x) modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), (quint8 *)&pVendorObj->readBuf[9], 2);
        memcpy((void *)pTag->pReadBuf, (void *)&pVendorObj->readBuf[9], 2);
        wDataLen=2;
    } else if(pTag->GetDataType() == TYPE_UINT32 || pTag->GetDataType() == TYPE_INT32 ||
              pTag->GetDataType() == TYPE_FLOAT) {
        if(cm == CM_3x || cm == CM_4x) modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), (quint8 *)&pVendorObj->readBuf[9], 4);
        memcpy((void *)pTag->pReadBuf, (void *)&pVendorObj->readBuf[9], 4);
        wDataLen = 4;
    } else if(pTag->GetDataType() == TYPE_DOUBLE) {
        if(cm == CM_3x || cm == CM_4x) modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), (quint8 *)&pVendorObj->readBuf[9], 8);
        memcpy((void *)pTag->pReadBuf, (void *)&pVendorObj->readBuf[9], 8);
        wDataLen = 8;
    } else if(pTag->GetDataType() == TYPE_UINT8 || pTag->GetDataType() == TYPE_INT8) {
        retSize = pVendorObj->readBuf[8];

        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = retSize-1;
            for(i=0; i<retSize; i++)
                *(pTag->pReadBuf + (j--)) = pVendorObj->readBuf[9+i];
        } else {
            j = retSize/2-1;
            for(i=0;i<retSize;i++,j--) {
                // 8位逆序
                if(isAddr8(pObj)) {
                    *(pTag->pReadBuf + 2 * j) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[9+i]);
                    i++;
                    *(pTag->pReadBuf + 2 * j + 1) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[9+i]);
                }
                else {
                    *(pTag->pReadBuf + 2 * j) = pVendorObj->readBuf[9+i];
                    i++;
                    *(pTag->pReadBuf + 2 * j + 1) = pVendorObj->readBuf[9+i];
                }
            }
        }
        wDataLen = retSize;
    } else if(pTag->GetDataType() == TYPE_BYTES) {
        retSize = pVendorObj->readBuf[8];

        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = 0;
            for(i=0; i<retSize; i++)
                *(pTag->pReadBuf + (j++)) = pVendorObj->readBuf[9+i];
        } else {
            j = 0;
            for(i=0;i<retSize;i++,j++) {
                *(pTag->pReadBuf + 2 * j + 1) = pVendorObj->readBuf[9+i];
                i++;
                *(pTag->pReadBuf + 2 * j) = pVendorObj->readBuf[9+i];
            }
        }

        wDataLen = retSize;
    }

    return 1;
}

