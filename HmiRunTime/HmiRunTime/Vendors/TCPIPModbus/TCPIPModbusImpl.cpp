#include "TCPIPModbusImpl.h"
#include "../Public/DataPack.h"
#include "../../HmiRunTime/shared/publicfunction.h"
#include "../../HmiRunTime/Vendor.h"
#include <QDebug>

TCPIPModbusImpl::TCPIPModbusImpl()
{

}

TCPIPModbusImpl::~TCPIPModbusImpl()
{

}


quint16 TCPIPModbusImpl::makeMessagePackage(quint8 *pSendData,
                                            void* pObj,
                                            RunTimeTag* pTag,
                                            TModbus_ReadWrite RW_flag,
                                            quint16 *retVarLen)
{
    quint16 mesPi = 0;
    quint32 tmpDataPos = 0;
    quint32 tmpUnit = 0;
    quint8 byteCount = 0, tmpLen = 0;
    Vendor* pVendorObj = (Vendor*)(pObj);
    TModbus_CPUMEM cm = getCpuMem(pTag->addrType);

    memset(tempBuffer, 0, sizeof(tempBuffer) / sizeof(quint8 ));

    quint16 iTransIdentifier = 0;
    if(pVendorObj) {
        iTransIdentifier = pVendorObj->m_extraData[0] + pVendorObj->m_extraData[1] * 256;
        iTransIdentifier = (iTransIdentifier + 1) % 65536;
    }

    tempBuffer[mesPi++] = (iTransIdentifier >> 8) & 0xFF; //transaction identifier
    tempBuffer[mesPi++] = iTransIdentifier & 0xFF; //transaction identifier

    if(pVendorObj) {
        pVendorObj->m_extraData[0] =  tempBuffer[1];
        pVendorObj->m_extraData[1] =  tempBuffer[0];
    }

    tempBuffer[mesPi++] = 0x00; //protocol identifier
    tempBuffer[mesPi++] = 0x00; //protocol identifier

    tempBuffer[mesPi++] = 0x00; //length field (upper byte) = 0 (since all messages are smaller than 256)
    tempBuffer[mesPi++] = 0x00; //length field (lower byte) = number of bytes following

    //设备地址
    tempBuffer[mesPi++] = this->devAddr(pVendorObj);
    //功能代码
    tempBuffer[mesPi++] = getFuncode(pObj, pTag, RW_flag);

    tmpDataPos = pTag->addrOffset;
    if(pVendorObj != NULL && !this->isStartAddrBit(pVendorObj)) {
        if(tmpDataPos > 0) {
            tmpDataPos -= 1;
        }
    }

    //开始地址
    tempBuffer[mesPi++] = tmpDataPos >> 8;
    tempBuffer[mesPi++] = tmpDataPos;

    //读取个数
    tmpUnit = getRegNum(pTag);

    //根据读/写方式构造报文
    if(RW_flag == FLAG_READ) {
        //计算返回报文长度
        tmpLen = pTag->bufLength + 3; // 3 = 一个设备地址 + 一个功能码 + 一个计数
        tempBuffer[mesPi++] = tmpUnit >> 8;
        tempBuffer[mesPi++] = tmpUnit;
    } else if(RW_flag == FLAG_WRITE) {
        tmpLen = 6; // 6 = 返回从机地址1， 功能代码1， 起始地址2以及强制线圈数2
        byteCount = pTag->bufLength;

        if(tempBuffer[6 + 1] != 0x06 && tempBuffer[6 + 1] != 0x05) //功能码为10
        {
            tempBuffer[mesPi++] = tmpUnit >> 8;
            tempBuffer[mesPi++] = tmpUnit;
            tempBuffer[mesPi++] = byteCount;
        }

        if(cm == CM_3x || cm == CM_4x) {
            switch(pTag->dataType) {
                case TYPE_INT16:
                case TYPE_UINT16: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 2);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 2);
                    mesPi += 2;
                }
                break;
                case TYPE_INT32:
                case TYPE_UINT32:
                case TYPE_FLOAT32: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 4);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 4);
                    mesPi += 4;
                }
                break;
                case TYPE_FLOAT64: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 8);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 8);
                    mesPi += 8;
                }
                break;
                case TYPE_ASCII2CHAR: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 2);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 2);
                    mesPi += 2;
                }
                break;
                default: {

                } break;
            }
        } else {
            switch(pTag->dataType) {
                case TYPE_BOOL:
                case TYPE_INT8:
                case TYPE_UINT8: {
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 1);
                    mesPi += 1;
                }
                break;
                case TYPE_INT16:
                case TYPE_UINT16: {
                    RecoverSelfData(pTag->dataToVendor, 2);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 2);
                    mesPi += 2;
                }
                break;
                case TYPE_INT32:
                case TYPE_UINT32:
                case TYPE_FLOAT32: {
                    RecoverSelfData(pTag->dataToVendor, 4);
                    memcpy(&tempBuffer[mesPi], pTag->dataToVendor, 4);
                    mesPi += 4;
                }
                break;
                default: {

                } break;
            }

            if(pTag->bufLength <= 1) {
                // 增加功能码为05写BOOL的操作
                if(tempBuffer[6 + 1] == 0x05) {
                    if(tempBuffer[6 + 4] == 0x01) {
                        tempBuffer[6 + 4] = 0xFF;
                    } else {
                        tempBuffer[6 + 4] = 0x00;
                    }
                    tempBuffer[6 + 5] = 0x00;
                    mesPi++;
                }
            }
        }
    }

    tempBuffer[5] = mesPi - 6;
    memcpy(pSendData, tempBuffer, mesPi);
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
bool TCPIPModbusImpl::isCanWrite(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    if(getCpuMem(pTag->addrType) == CM_1x) {
        return false;
    } else if(getCpuMem(pTag->addrType) == CM_3x) {
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
int TCPIPModbusImpl::writeData(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    quint16 msgLen = 0, revLen = 0;

    Vendor* pVendorObj = (Vendor*)(pObj);
    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf) / sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf) / sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_WRITE, &revLen);
    qDebug() << pVendorObj->m_pVendorPrivateObj->m_sDeviceName << ", Modbus Tx: " << hexToString((char *)pVendorObj->writeBuf, msgLen);

    if(getPort() != NULL) {
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
    }

    int resultlen = 0;
    if(getPort() != NULL) {
        resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        qDebug() << pVendorObj->m_pVendorPrivateObj->m_sDeviceName << ", Modbus Rx: " << hexToString((char *)pVendorObj->readBuf, revLen);
    }

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
bool TCPIPModbusImpl::isCanRead(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
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
int TCPIPModbusImpl::readData(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    quint16 retSize = 0, msgLen = 0, revLen = 0;
    qint16 i = 0, j = 0;

    TModbus_CPUMEM cm = getCpuMem(pTag->addrType);

    Vendor* pVendorObj = (Vendor*)(pObj);
    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf) / sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf) / sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_READ, &revLen);

    qDebug() << pVendorObj->m_pVendorPrivateObj->m_sDeviceName << ", Modbus Tx: " << hexToString((char *)pVendorObj->writeBuf, msgLen);
    if(getPort() != NULL) {
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
    }

    int resultlen = 0;
    if(cm == CM_0x || cm == CM_1x) {
        if(getPort() != NULL) {
            resultlen = getPort()->read(pVendorObj->readBuf, 9, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        }
        if(!(resultlen == 9 &&
             pVendorObj->readBuf[0] == pVendorObj->writeBuf[0] &&
             pVendorObj->readBuf[1] == pVendorObj->writeBuf[1] &&
             pVendorObj->readBuf[7] == pVendorObj->writeBuf[7])) {
            return -2;
        }

        if(getPort() != NULL) {
            resultlen = getPort()->read(&pVendorObj->readBuf[9], pVendorObj->readBuf[8], pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
            qDebug() << pVendorObj->m_pVendorPrivateObj->m_sDeviceName << ", Modbus Rx: " << hexToString((char *)pVendorObj->readBuf, pVendorObj->readBuf[8] + 9);
        }

        if(resultlen != pVendorObj->readBuf[8])
            return -2;

        if(pTag->dataType == TYPE_BOOL) {
            if(pVendorObj->readBuf[8] > 1) {
                revLen = revLen + pVendorObj->readBuf[8] - 1;
            }
        }
    } else {
        if(getPort() != NULL) {
            resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
            qDebug() << pVendorObj->m_pVendorPrivateObj->m_sDeviceName << ", Modbus Rx: " << hexToString((char *)pVendorObj->readBuf, revLen);
        }

        if(!(resultlen == revLen &&
             pVendorObj->readBuf[0] == pVendorObj->writeBuf[0] &&
             pVendorObj->readBuf[1] == pVendorObj->writeBuf[1] &&
             pVendorObj->readBuf[7] == pVendorObj->writeBuf[7])) {
            return -2;
        }
    }

    memset(tempBuffer, 0, sizeof(tempBuffer)/sizeof(quint8 ));

    // 返回数据的处理
    if(pTag->dataType == TYPE_BOOL) {
        retSize = 1;
        uint8ToBytes(pVendorObj->readBuf[9]&0x01, pTag->dataFromVendor);
    } else if(pTag->dataType == TYPE_INT16 || pTag->dataType == TYPE_UINT16) {
        pTag->updateVendorData(&pVendorObj->readBuf[9], 2);
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 2);
        }
    } else if(pTag->dataType == TYPE_UINT32 || pTag->dataType == TYPE_INT32 ||
              pTag->dataType == TYPE_FLOAT32) {
        pTag->updateVendorData(&pVendorObj->readBuf[9], 4);
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 4);
        }
    } else if(pTag->dataType == TYPE_FLOAT64) {
        pTag->updateVendorData(&pVendorObj->readBuf[9], 8);
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 8);
        }
    } else if(pTag->dataType == TYPE_UINT8 || pTag->dataType == TYPE_INT8) {
        retSize = pVendorObj->readBuf[8];
        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = retSize - 1;
            for(i = 0; i < retSize; i++) {
                *(tempBuffer + (j--)) = pVendorObj->readBuf[9 + i];
            }
            pTag->updateVendorData(tempBuffer, retSize);
        } else {
            j = retSize / 2 - 1;
            for(i = 0; i < retSize; i++, j--) {
                // 8位逆序
                if(isAddr8(pObj)) {
                    *(tempBuffer + 2 * j) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[9 + i]);
                    i++;
                    *(tempBuffer + 2 * j + 1) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[9 + i]);
                } else {
                    *(tempBuffer + 2 * j) = pVendorObj->readBuf[9 + i];
                    i++;
                    *(tempBuffer + 2 * j + 1) = pVendorObj->readBuf[9 + i];
                }
            }
            pTag->updateVendorData(tempBuffer, retSize);
        }
    } else if(pTag->dataType == TYPE_BYTES) {
        retSize = pVendorObj->readBuf[8];

        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = 0;
            for(i = 0; i < retSize; i++) {
                *(tempBuffer + (j++)) = pVendorObj->readBuf[9 + i];
            }
        } else {
            j = 0;
            for(i = 0; i < retSize; i++, j++) {
                *(tempBuffer + 2 * j + 1) = pVendorObj->readBuf[9 + i];
                i++;
                *(tempBuffer + 2 * j) = pVendorObj->readBuf[9 + i];
            }
        }
        pTag->updateVendorData(tempBuffer, retSize);
    }
    return 1;
}


