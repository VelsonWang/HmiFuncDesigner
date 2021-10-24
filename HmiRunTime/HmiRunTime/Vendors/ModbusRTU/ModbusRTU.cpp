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
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::beforeWriteIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


/**
 * @brief ModbusRTU::writeIOTag
 * @details 写变量
 * @param pObj 设备描述对象
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusRTU::writeIOTag(void* pObj, IPort *pPort, RunTimeTag* pTag)
{
    m_modbusRTUImplObj.setPort(pPort);
    if(!m_modbusRTUImplObj.isCanWrite(pObj, pTag)) {
        return 1;
    }
    return m_modbusRTUImplObj.writeData(pObj, pTag);
}


/**
 * @brief ModbusRTU::afterWriteIOTag
 * @details 写变量后处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::afterWriteIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}



/**
 * @brief ModbusRTU::beforeReadIOTag
 * @details 读变量前处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::beforeReadIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


/**
 * @brief ModbusRTU::readIOTag
 * @details 读变量
 * @param pObj 设备描述对象
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int ModbusRTU::readIOTag(void* pObj, IPort *pPort, RunTimeTag* pTag)
{
    m_modbusRTUImplObj.setPort(pPort);
    if(!m_modbusRTUImplObj.isCanRead(pObj, pTag)) {
        return 1;
    }
    return m_modbusRTUImplObj.readData(pObj, pTag);
}


/**
 * @brief ModbusRTU::afterReadIOTag
 * @details 读变量后处理
 * @param pObj 设备描述对象
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool ModbusRTU::afterReadIOTag(void* pObj, RunTimeTag* pTag)
{
    (void)pObj;
    (void)pTag;
    return true;
}


///
/// \brief ModbusRTU::convertIOTagBytesToNativeBytes
/// \details 变量字节序转换为当前主机字节序
/// \param pObj 设备描述对象
/// \param pTag 变量描述对象
/// \return true-成功, false-失败
///
bool ModbusRTU::convertIOTagBytesToNativeBytes(void* pObj, RunTimeTag* pTag)
{
    return m_modbusRTUImplObj.convertIOTagBytesToNativeBytes(pObj, pTag);
}





