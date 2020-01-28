#include "S7_200.h"
#include "../../HmiRunTime/Vendor.h"

S7_200::S7_200()
{

}


S7_200::~S7_200()
{

}


/**
 * @brief S7_200::initailizeDevice
 * @details 初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool S7_200::initailizeDevice(void* pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    if(pVendorObj) {

    }
    return true;
}


/**
 * @brief S7_200::connectDevice
 * @details 连接设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool S7_200::connectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief S7_200::disconnectDevice
 * @details 断开设备连接
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool S7_200::disconnectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief S7_200::unInitailizeDevice
 * @details 反初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool S7_200::unInitailizeDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief S7_200::beforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool S7_200::beforeWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief S7_200::writeIOTag
 * @details 写变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int S7_200::writeIOTag(IPort *pPort, IOTag* pTag)
{
    m_siemensS7_200ImplImplObj.setPort(pPort);
    if(!m_siemensS7_200ImplImplObj.isCanWrite(pTag)) return 1;
    return m_siemensS7_200ImplImplObj.writeData(pTag);
}


/**
 * @brief S7_200::afterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool S7_200::afterWriteIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}



/**
 * @brief S7_200::beforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool S7_200::beforeReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief S7_200::readIOTag
 * @details 读变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int S7_200::readIOTag(IPort *pPort, IOTag* pTag)
{
    m_siemensS7_200ImplImplObj.setPort(pPort);
    if(!m_siemensS7_200ImplImplObj.isCanRead(pTag)) return 1;
    return m_siemensS7_200ImplImplObj.readData(pTag);
}


/**
 * @brief S7_200::afterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool S7_200::afterReadIOTag(IOTag* pTag)
{
    (void)pTag;
    return true;
}






