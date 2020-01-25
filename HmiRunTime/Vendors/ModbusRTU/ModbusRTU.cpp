#include "ModbusRTU.h"
#include "../../HmiRunTime/Vendor.h"

ModbusRTU::ModbusRTU()
{

}


ModbusRTU::~ModbusRTU()
{

}


/**
 * @brief ModbusRTU::initailizeDevice
 * @details 初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::initailizeDevice(void* pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    if(pVendorObj) {
        m_modbusRTUImplObj.insertBlockReadTagToReadList(pVendorObj->m_readList);
    }
    return true;
}


/**
 * @brief ModbusRTU::connectDevice
 * @details 连接设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::connectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusRTU::disconnectDevice
 * @details 断开设备连接
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::disconnectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusRTU::unInitailizeDevice
 * @details 反初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::unInitailizeDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief ModbusRTU::beforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::beforeWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief ModbusRTU::writeIOTag
 * @details 写变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusRTU::writeIOTag(IPort *pPort, IOTag* pTag)
{
    m_modbusRTUImplObj.setPort(pPort);
    if(!m_modbusRTUImplObj.isCanWrite(pTag)) return 1;
    return m_modbusRTUImplObj.writeData(pTag);
}


/**
 * @brief ModbusRTU::afterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::afterWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}



/**
 * @brief ModbusRTU::beforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::beforeReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief ModbusRTU::readIOTag
 * @details 读变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusRTU::readIOTag(IPort *pPort, IOTag* pTag)
{
    m_modbusRTUImplObj.setPort(pPort);
    if(!m_modbusRTUImplObj.isCanRead(pTag)) return 1;
    return m_modbusRTUImplObj.readData(pTag);
}


/**
 * @brief ModbusRTU::afterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::afterReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}






