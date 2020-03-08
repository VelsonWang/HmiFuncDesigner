#include "ModbusASCII.h"
#include "../../HmiRunTime/Vendor.h"


ModbusASCII::ModbusASCII()
{

}


ModbusASCII::~ModbusASCII()
{

}


/**
 * @brief ModbusASCII::initailizeDevice
 * @details 初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::initailizeDevice(void* pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    if(pVendorObj) {
        m_ModbusASCIIImplObj.insertBlockReadTagToReadList(pVendorObj->m_readList);
    }
    return true;
}


/**
 * @brief ModbusASCII::connectDevice
 * @details 连接设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::connectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusASCII::disconnectDevice
 * @details 断开设备连接
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::disconnectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusASCII::unInitailizeDevice
 * @details 反初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::unInitailizeDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusASCII::beforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::beforeWriteIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief ModbusASCII::writeIOTag
 * @details 写变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusASCII::writeIOTag(void* pObj, IPort *pPort, IOTag* pTag)
{
    m_ModbusASCIIImplObj.setPort(pPort);
    if(!m_ModbusASCIIImplObj.isCanWrite(pObj, pTag)) return 1;
    return m_ModbusASCIIImplObj.writeData(pObj, pTag);
}


/**
 * @brief ModbusASCII::afterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::afterWriteIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}



/**
 * @brief ModbusASCII::beforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::beforeReadIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief ModbusASCII::readIOTag
 * @details 读变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusASCII::readIOTag(void* pObj, IPort *pPort, IOTag* pTag)
{
    m_ModbusASCIIImplObj.setPort(pPort);
    if(!m_ModbusASCIIImplObj.isCanRead(pObj, pTag)) return 1;
    return m_ModbusASCIIImplObj.readData(pObj, pTag);
}


/**
 * @brief ModbusASCII::afterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusASCII::afterReadIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}


