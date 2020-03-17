#include "MitsubishiImpl.h"
#include "DataPack.h"
#include "../../HmiRunTimeData/Public/PublicFunction.h"
#include "../../HmiRunTimeData/Vendor.h"
#include <string.h>
#include <QObject>

enum _TCPUMEM
{
    CM_NON  =   0,
    CM_X    =   1,
    CM_Y    =   2,
    CM_M    =   3,
    CM_S    =   4,
    CM_D   	=   5,
    CM_T   	=   6,
    CM_C16  =  	7,
    CM_C32  =  	8
};

typedef enum _TCPUMEM TCPUMEM;


static quint16 getBeginAddrAsCpuMem(TCPUMEM cm, TTagDataType dataType)
{
    quint16 wRet = 0;

    switch(cm)
    {
    case CM_X:
        wRet = 0x0080;
        break;
    case CM_Y:
        wRet = 0x00A0;
        break;
    case CM_M:
        wRet = 0x0100;
        break;
    case CM_S:
        wRet = 0x0000;
        break;
    case CM_D:
        wRet = 0x1000;
        break;
    case CM_T:
        if(dataType == TYPE_BOOL)
            wRet = 0x00C0;
        else
            wRet = 0x0800;
        break;
    case CM_C16:
        if(dataType == TYPE_BOOL)
            wRet = 0x01C0;
        else
            wRet = 0x0A00;
        break;
    case CM_C32:
        if(dataType == TYPE_BOOL)
            wRet = 0x01C0;
        else
            wRet = 0x0C00;
        break;

    default:
        break;
    }

    return wRet;
}

//4字节ASCII
static void makeAddress(quint16 wAddress, quint8 * pStore)
{
    SetDataAsWord(pStore, wAddress);
    RecoverSelfData(pStore, 2);
    MakeCodeToAsii(pStore, pStore, 2);
}

//2字节ASCII
static void makeDatalen(quint16 wDataLen, quint8 * pStore)
{
    quint8 byDataLen = (quint8)wDataLen;
    MakeCodeToAsii(&byDataLen, pStore, 1);
}

static quint16 getboolBlockByte(quint32 dwDataPos, size_t RepeatLen)
{
    int iTemp = (quint16)dwDataPos % 8;
    quint16 wDataLen = (iTemp > 0) ? 1 : 0;

    if(iTemp > 0)
        iTemp = 8 - iTemp;

    iTemp = RepeatLen - iTemp;
    wDataLen += iTemp / 8;

    iTemp = iTemp % 8;
    wDataLen += (iTemp > 0) ? 1 : 0;

    return wDataLen;
}

static quint16 MitsubishiImpl_GetAddressOffset(TCPUMEM cm, quint16 wDataPos)
{
    quint16 wOffSet;

    if((cm == CM_T) || (cm == CM_D) || (cm == CM_C16)) {
        wOffSet = wDataPos * 2;
    } else if(cm == CM_C32) {
        wOffSet = (wDataPos - 200) * 4;
    } else {
        wOffSet = wDataPos / 8;
    }

    return wOffSet;
}

static TCPUMEM getCpuMem(const QString &szRegisterArea)
{
    if(szRegisterArea == QObject::tr("X"))
        return CM_X;
    else if(szRegisterArea == QObject::tr("Y"))
        return CM_Y;
    else if(szRegisterArea == QObject::tr("M"))
        return CM_M;
    else if(szRegisterArea == QObject::tr("S"))
        return CM_S;
    else if(szRegisterArea == QObject::tr("D"))
        return CM_D;
    else if(szRegisterArea == QObject::tr("T"))
        return CM_T;
    else if(szRegisterArea == QObject::tr("C16"))
        return CM_C16;
    else if(szRegisterArea == QObject::tr("C32"))
        return CM_C32;
    else
        return CM_NON;
}


MitsubishiImpl::MitsubishiImpl() : iFacePort_(nullptr)
{
}


MitsubishiImpl::~MitsubishiImpl()
{
}


void MitsubishiImpl::setPort(IPort *pPort)
{
    if(pPort != nullptr)
        iFacePort_ = pPort;
}


IPort *MitsubishiImpl::getPort()
{
    return iFacePort_;
}


/**
 * @brief MitsubishiImpl::isCanWrite
 * @details 判断该变量定义属性是否可以写
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可写，true-可写
 */
bool MitsubishiImpl::isCanWrite(void* pObj, IOTag* pTag)
{
    (void)pObj;
    if(getCpuMem(pTag->GetRegisterArea()) == CM_X)
        return false;
    else if(getCpuMem(pTag->GetRegisterArea()) == CM_C32) {
        if(pTag->GetDataType() == TYPE_UINT32)
            return false;
    }

    return true;
}


/**
 * @brief MitsubishiImpl::writeData
 * @details 写一个变量值到plc设备
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败, 1-成功
 */
int MitsubishiImpl::writeData(void* pObj, IOTag* pTag)
{
    (void)pObj;
    static int iFirstFlag = 1;
    quint16 wAddress = getBeginAddrAsCpuMem(getCpuMem(pTag->GetRegisterArea()), pTag->GetDataType());
    size_t len = 0;
    int resultlen = 0;
    Vendor* pVendorObj = (Vendor*)(pObj);

    if(iFirstFlag) {
        pVendorObj->writeBuf[0] = 0x05;
        if(getPort() != nullptr)
            getPort()->write(pVendorObj->writeBuf, 1, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

        resultlen = 0;
        if(getPort() != nullptr)
            resultlen = getPort()->read(pVendorObj->readBuf, 1, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        if(!(resultlen == 1 && pVendorObj->readBuf[0] == 0x06))
            return false;
        iFirstFlag = 0;
    }

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    pVendorObj->writeBuf[len++] = 0x02;
    if(pTag->GetDataType() == TYPE_BOOL) {
        if(pTag->pWriteBuf[0])
            pVendorObj->writeBuf[len++] = 0x37;
        else
            pVendorObj->writeBuf[len++] = 0x38;
        wAddress = wAddress * 8 + pTag->GetRegisterAddress() + pTag->GetOffset();
        RecoverSelfData((quint8 *)&wAddress, 2);
        makeAddress(wAddress, pVendorObj->writeBuf + len);
        len += 4;
        pVendorObj->writeBuf[len++] = 0x03;
        *(quint8 *)(pVendorObj->writeBuf + len) = (quint8)AddCheckSum(pVendorObj->writeBuf + 1, len - 1);
        MakeCodeToAsii(pVendorObj->writeBuf + len, pVendorObj->writeBuf + len, 1);
        len += 2;
    } else {
        quint16 wDataLen;
        pVendorObj->writeBuf[1] = 0x31;
        wAddress += MitsubishiImpl_GetAddressOffset(getCpuMem(pTag->GetRegisterArea()), (quint16)(pTag->GetRegisterAddress() + pTag->GetOffset()));
        makeAddress(wAddress, pVendorObj->writeBuf + 2);
        len = 8;
        wDataLen = (quint16)pTag->GetDataTypeLength();
        memcpy(pVendorObj->writeBuf + len, pTag->pWriteBuf, wDataLen);
        MakeCodeToAsii(pVendorObj->writeBuf + len, pVendorObj->writeBuf + len, wDataLen);
        makeDatalen(wDataLen, pVendorObj->writeBuf + 6);

        len += wDataLen * 2;
        pVendorObj->writeBuf[len] = 0x03;
        len += 1;

        *(quint8 *)(pVendorObj->writeBuf + len) = (quint8)AddCheckSum(pVendorObj->writeBuf + 1, len - 1);
        MakeCodeToAsii(pVendorObj->writeBuf + len, pVendorObj->writeBuf + len, 1);
        len += 2;
    }

    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    if(getPort() != nullptr)
        getPort()->write(pVendorObj->writeBuf, len, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    resultlen = 0;
    if(getPort() != nullptr)
        resultlen = getPort()->read(pVendorObj->readBuf, 1, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    if(resultlen == 1 && pVendorObj->readBuf[0] == 0x06)
        return 1;

    return 0;
}


/**
 * @brief MitsubishiImpl::isCanRead
 * @details 判断该变量定义属性是否可以读
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return false-不可读，true-可读
 */
bool MitsubishiImpl::isCanRead(void* pObj, IOTag* pTag)
{
    (void)pObj;
    Q_UNUSED(pTag)
    return true;
}


/**
 * @brief MitsubishiImpl::readData
 * @details 从plc设备读一个变量
 * @param pObj 设备描述对象
 * @param pTag 设备变量
 * @return 0-失败, 1-成功
 */
int MitsubishiImpl::readData(void* pObj, IOTag* pTag)
{
    (void)pObj;
    static int iFirstFlag = 1;
    quint16 wAddress = getBeginAddrAsCpuMem(getCpuMem(pTag->GetRegisterArea()), pTag->GetDataType());
    size_t len = 0;
    quint16 wDataLen = 0;
    quint8 byCheckSum = 0;
    quint8 *pVailableData;
    int resultlen = 0;
    Vendor* pVendorObj = (Vendor*)(pObj);

    if(iFirstFlag) {
        pVendorObj->writeBuf[0] = 0x05;
        if(getPort() != nullptr)
            getPort()->write(pVendorObj->writeBuf, 1, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

        resultlen = 0;
        if(getPort() != nullptr)
            resultlen = getPort()->read(pVendorObj->readBuf, 1, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
        if(!(resultlen == 1 && pVendorObj->readBuf[0] == 0x06))
            return false;
        iFirstFlag = 0;
    }

    memset(pVendorObj->writeBuf, 0, sizeof(pVendorObj->writeBuf)/sizeof(quint8));
    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    pVendorObj->writeBuf[len++] = 0x02;
    pVendorObj->writeBuf[len++] = 0x30;

    if(pTag->GetDataType() == TYPE_BOOL) {
        wAddress += (quint16)(pTag->GetRegisterAddress() + pTag->GetOffset()) / 8;
        makeAddress(wAddress, pVendorObj->writeBuf + len);
        wDataLen = getboolBlockByte((quint32)(pTag->GetRegisterAddress() + pTag->GetOffset()), pTag->GetDataTypeLength());
    } else {
        wAddress += MitsubishiImpl_GetAddressOffset(getCpuMem(pTag->GetRegisterArea()), (quint16)(pTag->GetRegisterAddress() + pTag->GetOffset()));
        makeAddress(wAddress, pVendorObj->writeBuf + len);
        wDataLen = (quint16)pTag->GetDataTypeLength();
    }

    len += 4;
    makeDatalen(wDataLen, pVendorObj->writeBuf + len);
    len += 2;

    pVendorObj->writeBuf[len] = 0x03;
    len += 1;

    *(quint8 *)(pVendorObj->writeBuf + len) = (quint8)AddCheckSum(pVendorObj->writeBuf + 1, len - 1);
    MakeCodeToAsii(pVendorObj->writeBuf + len, pVendorObj->writeBuf + len, 1);
    len += 2;

    memset(pVendorObj->readBuf, 0, sizeof(pVendorObj->readBuf)/sizeof(quint8));

    if(getPort() != nullptr)
        getPort()->write(pVendorObj->writeBuf, len, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);

    resultlen = 0;
    if(getPort() != nullptr)
        resultlen = getPort()->read(pVendorObj->readBuf, 4 + wDataLen * 2, pVendorObj->m_pVendorPrivateObj->m_iCommTimeout);
    if(resultlen != (4 + wDataLen * 2))
        return -2;

    if((pVendorObj->readBuf[0] != 0x02) || (pVendorObj->readBuf[1 + wDataLen * 2] != 0x03))
        return 0;

    byCheckSum = (quint8)AddCheckSum(pVendorObj->readBuf + 1, wDataLen * 2 + 1);
    pVailableData = pVendorObj->readBuf + 2 + wDataLen * 2;
    MakeAsiiToCode(pVailableData, pVailableData, 1);
    if(byCheckSum != *pVailableData)
        return 0;

    MakeAsiiToCode(pVendorObj->readBuf + 1, pVendorObj->readBuf + 1, wDataLen);

    pVailableData = pVendorObj->readBuf + 1;

    if(pTag->GetDataType() == TYPE_BOOL) {
        int byteAddr = (quint16)(pTag->GetRegisterAddress() + pTag->GetOffset()) / 8;
        int bitAddr = (quint16)(pTag->GetRegisterAddress() + pTag->GetOffset()) % 8;
        pTag->pReadBuf[0] = (pVailableData[byteAddr] >> bitAddr) & 0x01;
    } else {
        len = pTag->GetDataTypeLength();
        memcpy(pTag->pReadBuf, pVailableData, len);
    }

    return 1;
}




///
/// \brief MitsubishiImpl::convertIOTagBytesToNativeBytes
/// \details 变量字节序转换为当前主机字节序
/// \param pObj 设备描述对象
/// \param pTag 变量描述对象
/// \return true-成功, false-失败
///
bool MitsubishiImpl::convertIOTagBytesToNativeBytes(void* pObj, IOTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}
