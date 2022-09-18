#include "TCPIPModbus.h"
#include "../../HmiRunTime/Vendor.h"
#include "../../HmiRunTime/shared/publicfunction.h"

TCPIPModbus::TCPIPModbus()
{

}


TCPIPModbus::~TCPIPModbus()
{

}


/**
 * @brief TCPIPModbus::initailizeDevice
 * @details 初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::initailizeDevice(void* pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    if(pVendorObj) {

    }
    return true;
}


/**
 * @brief TCPIPModbus::connectDevice
 * @details 连接设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::connectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief TCPIPModbus::disconnectDevice
 * @details 断开设备连接
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::disconnectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief TCPIPModbus::unInitailizeDevice
 * @details 反初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::unInitailizeDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief TCPIPModbus::beforeWriteIOTag
 * @details 写变量前处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::beforeWriteIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


/**
 * @brief TCPIPModbus::writeIOTag
 * @details 写变量
 * @param pObj 设备描述对象
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int TCPIPModbus::writeIOTag(void* pObj, IPort *pPort, RunTimeTag* pTag)
{
    m_TCPIPModbusImplObj.setPort(pPort);
    if(!m_TCPIPModbusImplObj.isCanWrite(pObj, pTag)) {
        return 1;
    }
    return m_TCPIPModbusImplObj.writeData(pObj, pTag);
}


/**
 * @brief TCPIPModbus::afterWriteIOTag
 * @details 写变量后处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::afterWriteIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}



/**
 * @brief TCPIPModbus::beforeReadIOTag
 * @details 读变量前处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::beforeReadIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


/**
 * @brief TCPIPModbus::readIOTag
 * @details 读变量
 * @param pObj 设备描述对象
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int TCPIPModbus::readIOTag(void* pObj, IPort *pPort, RunTimeTag* pTag)
{
    m_TCPIPModbusImplObj.setPort(pPort);
    if(!m_TCPIPModbusImplObj.isCanRead(pObj, pTag)) {
        return 1;
    }
    return m_TCPIPModbusImplObj.readData(pObj, pTag);
}

// 从块读变量拷贝寄存器数据至普通变量
bool TCPIPModbus::copyTagDataFromBlockReadTag(RunTimeTag* pBlockReadTag, RunTimeTag* pTag)
{
    int iOffset = pTag->addrOffset - pBlockReadTag->addrOffset;
    quint8 *pDes = pTag->dataFromVendor;
    quint8 *pSrc = pBlockReadTag->dataFromVendor;

    if(pTag->dataType == TYPE_BOOL) {
        quint32 iByte = iOffset / 8;
        quint32 iBit = iOffset % 8;
        pDes[0] = (pSrc[iByte] >> iBit) & 0x01;
    } else {
        for(int x=0; x<pTag->bufLength; x++) {
            pDes[x] = pSrc[iOffset * 2 + x];
        }
    }
#if 0
    qDebug() << "BlockReadTag: addr " << pBlockReadTag->addrOffset << hexToString((char *)pBlockReadTag->dataFromVendor, pBlockReadTag->bufLength);
    qDebug() << "         Tag: addr " << pTag->addrOffset << hexToString((char *)pTag->dataFromVendor, pTag->bufLength);
#endif
    return true;
}

// 设置块读变量缓冲区长度
void TCPIPModbus::setBlockReadTagBufferLength(RunTimeTag* pBlockReadTag)
{
    if(pBlockReadTag && pBlockReadTag->isBlockRead) {
        if(pBlockReadTag->dataType != TYPE_BOOL) {
            pBlockReadTag->bufLength *= 2;
        }
    }
}

/**
 * @brief TCPIPModbus::afterReadIOTag
 * @details 读变量后处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::afterReadIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


///
/// \brief TCPIPModbus::convertIOTagBytesToNativeBytes
/// \details 变量字节序转换为当前主机字节序
/// \param pObj 设备描述对象
/// \param pTag 变量描述对象
/// \return true-成功, false-失败
///
bool TCPIPModbus::convertIOTagBytesToNativeBytes(void* pObj, RunTimeTag* pTag)
{
    return m_TCPIPModbusImplObj.convertIOTagBytesToNativeBytes(pObj, pTag);
}





