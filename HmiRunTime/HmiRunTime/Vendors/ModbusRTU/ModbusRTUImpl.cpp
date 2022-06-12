#include "ModbusRTUImpl.h"
#include "../Public/DataPack.h"
#include "../../../libs/shared/publicfunction.h"
#include "../../HmiRunTime/Vendor.h"
#include <QDebug>

const quint8 auchCRCHi[] = {
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

const quint8 auchCRCLo[] = {
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

ModbusRTUImpl::ModbusRTUImpl()
{

}

ModbusRTUImpl::~ModbusRTUImpl()
{

}


quint16 ModbusRTUImpl::makeMessagePackage(quint8 *pSendData,
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

    memset(tempBuffer_, 0, sizeof(tempBuffer_) / sizeof(quint8 ));

    mesPi = 0;

    //设备地址
    tempBuffer_[mesPi++] = this->devAddr(pVendorObj);
    //功能代码
    tempBuffer_[mesPi++] = getFuncode(pObj, pTag, RW_flag);

    tmpDataPos = pTag->addrOffset;
    if(pVendorObj != NULL && !this->isStartAddrBit(pVendorObj)) {
        if(tmpDataPos > 0) {
            tmpDataPos -= 1;
        }
    }

    //开始地址
    tempBuffer_[mesPi++] = tmpDataPos >> 8;
    tempBuffer_[mesPi++] = tmpDataPos;

    //读取个数
    tmpUnit = getRegNum(pTag);

    //根据读/写方式构造报文
    if(RW_flag == FLAG_READ) {
        //计算返回报文长度
        tmpLen = pTag->bufLength + 3; // 3 = 一个设备地址 + 一个功能码 + 一个计数
        tempBuffer_[mesPi++] = tmpUnit >> 8;
        tempBuffer_[mesPi++] = tmpUnit;
    } else if(RW_flag == FLAG_WRITE) {
        tmpLen = 6; // 6 = 返回从机地址1， 功能代码1， 起始地址2以及强制线圈数2
        byteCount = pTag->bufLength;

        if(tempBuffer_[1] != 0x06 && tempBuffer_[1] != 0x05) { //功能码为10
            tempBuffer_[mesPi++] = tmpUnit >> 8;
            tempBuffer_[mesPi++] = tmpUnit;
            tempBuffer_[mesPi++] = byteCount;
        }

        if(cm == CM_3x || cm == CM_4x) {
            switch(pTag->dataType) {
                case TYPE_INT16:
                case TYPE_UINT16: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 2);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 2);
                    mesPi += 2;
                }
                break;
                case TYPE_INT32:
                case TYPE_UINT32:
                case TYPE_FLOAT32: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 4);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 4);
                    mesPi += 4;
                }
                break;
                case TYPE_FLOAT64: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 8);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 8);
                    mesPi += 8;
                }
                break;
                case TYPE_ASCII2CHAR: {
                    modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataToVendor, 2);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 2);
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
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 1);
                    mesPi += 1;
                }
                break;
                case TYPE_INT16:
                case TYPE_UINT16: {
                    RecoverSelfData(pTag->dataToVendor, 2);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 2);
                    mesPi += 2;
                }
                break;
                case TYPE_INT32:
                case TYPE_UINT32:
                case TYPE_FLOAT32: {
                    RecoverSelfData(pTag->dataToVendor, 4);
                    memcpy(&tempBuffer_[mesPi], pTag->dataToVendor.data(), 4);
                    mesPi += 4;
                }
                break;
                default: {

                } break;
            }

            if(pTag->bufLength <= 1) {
                // 增加功能码为05写BOOL的操作
                if(tempBuffer_[1] == 0x05) {
                    if(tempBuffer_[4] == 0x01) {
                        tempBuffer_[4] = 0xFF;
                    } else {
                        tempBuffer_[4] = 0x00;
                    }
                    tempBuffer_[5] = 0x00;
                    mesPi++;
                }
            }
        }
    }

    quint16 tmpCRC16 = crc16(tempBuffer_, mesPi);
    tempBuffer_[mesPi++] = tmpCRC16 >> 8;
    tempBuffer_[mesPi++] = tmpCRC16;

    memcpy(pSendData, tempBuffer_, mesPi);
    *retVarLen = tmpLen + 2;

    return mesPi;
}


quint16 ModbusRTUImpl::crc16(quint8 *pbuf, qint32 len)
{
    quint8 uchCRCHi = 0xFF;
    quint8 uchCRCLo = 0xFF;
    qint32 uIndex = 0;

    while (len--) {
        uIndex = uchCRCHi ^ *pbuf++;
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }

    return (uchCRCHi << 8 | uchCRCLo);
}


bool ModbusRTUImpl::messageCheck(quint8 *inBuf, qint16 bufLen)
{
    quint16 revCRC16 = 0, calCRC16 = 0;

    calCRC16 = crc16(inBuf, bufLen - 2);
    memcpy(&revCRC16, inBuf + bufLen - 2, 2);
    RecoverSelfData((quint8 *)&revCRC16, 2);

    if(calCRC16 == revCRC16) {
        return true;
    }

    return false;
}



/**
 * @brief ModbusRTUImpl::isCanWrite
 * @details 判断该变量定义属性是否可以写
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可写，true-可写
 */
bool ModbusRTUImpl::isCanWrite(void* pObj, RunTimeTag* pTag)
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
 * @brief ModbusRTUImpl::writeData
 * @details 写一个变量值到plc设备
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int ModbusRTUImpl::writeData(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    quint16 msgLen = 0, revLen = 0;

    Vendor* pVendorObj = (Vendor*)(pObj);
    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf) / sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf) / sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_WRITE, &revLen);
    qDebug() << "Modbus Tx: " << hexToString((char *)pVendorObj->writeBuf, msgLen);

    if(getPort() != NULL) {
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
    }

    int resultlen = 0;
    if(getPort() != NULL) {
        resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        qDebug() << "Modbus Rx: " << hexToString((char *)pVendorObj->readBuf, revLen);
    }

    if(resultlen == revLen && messageCheck(pVendorObj->readBuf, resultlen)) {
        return 1;
    }

    return 0;
}


/**
 * @brief ModbusRTUImpl::isCanRead
 * @details 判断该变量定义属性是否可以读
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可读，true-可读
 */
bool ModbusRTUImpl::isCanRead(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    Q_UNUSED(pTag)
    return true;
}


/**
 * @brief ModbusRTUImpl::readData
 * @details 从plc设备读一个变量
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败,1-成功
 */
int ModbusRTUImpl::readData(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    quint16 retSize = 0, msgLen = 0, revLen = 0;
    qint16 i = 0, j = 0;
    quint32 wDataLen = 0;

    TModbus_CPUMEM cm = getCpuMem(pTag->addrType);

    Vendor* pVendorObj = (Vendor*)(pObj);
    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf) / sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf) / sizeof(quint8));

    msgLen = makeMessagePackage(pVendorObj->writeBuf, pObj, pTag, FLAG_READ, &revLen);

    qDebug() << "Modbus Tx: " << hexToString((char *)pVendorObj->writeBuf, msgLen);

    if(getPort() != NULL) {
        getPort()->write(pVendorObj->writeBuf, msgLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
    }

    int resultlen = 0;

    if(cm == CM_0x || cm == CM_1x) {
        if(getPort() != NULL) {
            resultlen = getPort()->read(pVendorObj->readBuf, 3, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        }

        if(resultlen != 3) {
            return -2;
        }

        if(getPort() != NULL) {
            resultlen = getPort()->read(&pVendorObj->readBuf[3], pVendorObj->readBuf[2] + 2, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        }

        if(resultlen != pVendorObj->readBuf[2] + 2) {
            return -2;
        }

        if(pTag->dataType == TYPE_BOOL) {
            if(pVendorObj->readBuf[2] > 1) {
                revLen = revLen + pVendorObj->readBuf[2] - 1;
            }
        }
    } else {

        if(getPort() != NULL) {
            resultlen = getPort()->read(pVendorObj->readBuf, revLen, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        }

        qDebug() << "Modbus Rx: " << hexToString((char *)pVendorObj->readBuf, revLen);

        if(resultlen != revLen) {
            return -2;
        }
    }

    if(!messageCheck(pVendorObj->readBuf, revLen)) {
        return 0;
    }

    memset(tempBuffer_, 0, sizeof(tempBuffer_) / sizeof(quint8 ));

    // 返回数据的处理
    if(pTag->dataType == TYPE_BOOL) {
        retSize = 1;
        pTag->updateVendorData(uint8ToBytes(pVendorObj->readBuf[3] & 0x01));
        wDataLen = retSize;
    } else if(pTag->dataType == TYPE_INT16 || pTag->dataType == TYPE_UINT16) {
        pTag->updateVendorData(QByteArray((const char *)(const char *)&pVendorObj->readBuf[3], 2));
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 2);
        }
        wDataLen = 2;
    } else if(pTag->dataType == TYPE_UINT32 || pTag->dataType == TYPE_INT32 ||
              pTag->dataType == TYPE_FLOAT32) {
        pTag->updateVendorData(QByteArray((const char *)(const char *)&pVendorObj->readBuf[3], 4));
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 4);
        }
        wDataLen = 4;
    } else if(pTag->dataType == TYPE_FLOAT64) {
        pTag->updateVendorData(QByteArray((const char *)&pVendorObj->readBuf[3], 8));
        if(cm == CM_3x || cm == CM_4x) {
            modbusChangeData(isAddr8(pObj), !isAddr16(pObj), isAddr32(pObj), isAddr64(pObj), pTag->dataFromVendor, 8);
        }
        wDataLen = 8;
    } else if(pTag->dataType == TYPE_UINT8 || pTag->dataType == TYPE_INT8) {
        retSize = pVendorObj->readBuf[2];

        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = retSize - 1;
            for(i = 0; i < retSize; i++) {
                *(tempBuffer_ + (j--)) = pVendorObj->readBuf[3 + i];
            }
            pTag->updateVendorData(QByteArray((const char *)tempBuffer_, retSize));
        } else {
            j = retSize / 2 - 1;
            for(i = 0; i < retSize; i++, j--) {
                // 8位逆序
                if(isAddr8(pObj)) {
                    *(tempBuffer_ + 2 * j) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[3 + i]);
                    i++;
                    *(tempBuffer_ + 2 * j + 1) = byteSwitchHigh4bitLow4bit(pVendorObj->readBuf[3 + i]);
                } else {
                    *(tempBuffer_ + 2 * j) = pVendorObj->readBuf[3 + i];
                    i++;
                    *(tempBuffer_ + 2 * j + 1) = pVendorObj->readBuf[3 + i];
                }
            }
            pTag->updateVendorData(QByteArray((const char *)tempBuffer_, retSize));
        }
        wDataLen = retSize;
    } else if(pTag->dataType == TYPE_BYTES) {
        retSize = pVendorObj->readBuf[2];

        if(getFuncode(pObj, pTag, FLAG_READ) == 0x01 || getFuncode(pObj, pTag, FLAG_READ) == 0x02) {
            j = 0;
            for(i = 0; i < retSize; i++) {
                *(tempBuffer_ + (j++)) = pVendorObj->readBuf[3 + i];
            }
        } else {
            j = 0;
            for(i = 0; i < retSize; i++, j++) {
                *(tempBuffer_ + 2 * j + 1) = pVendorObj->readBuf[3 + i];
                i++;
                *(tempBuffer_ + 2 * j) = pVendorObj->readBuf[3 + i];
            }
        }
        pTag->updateVendorData(QByteArray((const char *)tempBuffer_, retSize));
        wDataLen = retSize;
    }

    return 1;
}


