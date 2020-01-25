#include "TCPIPModbus.h"
#include "../../HmiRunTime/Vendor.h"


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
        m_TCPIPModbusImplObj.insertBlockReadTagToReadList(pVendorObj->m_readList);
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
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::beforeWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief TCPIPModbus::writeIOTag
 * @details 写变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int TCPIPModbus::writeIOTag(IPort *pPort, IOTag* pTag)
{
    m_TCPIPModbusImplObj.setPort(pPort);
    if(!m_TCPIPModbusImplObj.isCanWrite(pTag)) return 1;
    return m_TCPIPModbusImplObj.writeData(pTag);
}


/**
 * @brief TCPIPModbus::afterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::afterWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}



/**
 * @brief TCPIPModbus::beforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::beforeReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief TCPIPModbus::readIOTag
 * @details 读变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int TCPIPModbus::readIOTag(IPort *pPort, IOTag* pTag)
{
    m_TCPIPModbusImplObj.setPort(pPort);
    if(!m_TCPIPModbusImplObj.isCanRead(pTag)) return 1;
    return m_TCPIPModbusImplObj.readData(pTag);
}


/**
 * @brief TCPIPModbus::afterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool TCPIPModbus::afterReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}





