#include "FX2N.h"
#include "../../HmiRunTime/Vendor.h"

FX2N::FX2N()
{

}


FX2N::~FX2N()
{

}


/**
 * @brief FX2N::initailizeDevice
 * @details 初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool FX2N::initailizeDevice(void* pObj)
{
    Vendor* pVendorObj = (Vendor*)(pObj);
    if(pVendorObj) {

    }
    return true;
}


/**
 * @brief FX2N::connectDevice
 * @details 连接设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool FX2N::connectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief FX2N::disconnectDevice
 * @details 断开设备连接
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool FX2N::disconnectDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief FX2N::unInitailizeDevice
 * @details 反初始化设备
 * @param pObj 设备描述对象
 * @return false-失败, true-成功
 */
bool FX2N::unInitailizeDevice(void* pObj)
{
    (void)pObj;
    return true;
}


/**
 * @brief FX2N::beforeWriteIOTag
 * @details 写变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool FX2N::beforeWriteIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief FX2N::writeIOTag
 * @details 写变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int FX2N::writeIOTag(void* pObj, IPort *pPort, IOTag* pTag)
{
    m_mitsubishiImplObj.setPort(pPort);
    if(!m_mitsubishiImplObj.isCanWrite(pObj, pTag)) return 1;
    return m_mitsubishiImplObj.writeData(pObj, pTag);
}


/**
 * @brief FX2N::afterWriteIOTag
 * @details 写变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool FX2N::afterWriteIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}



/**
 * @brief FX2N::beforeReadIOTag
 * @details 读变量前处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool FX2N::beforeReadIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}


/**
 * @brief FX2N::readIOTag
 * @details 读变量
 * @param pPort 端口操作接口
 * @param pTag 变量描述对象
 * @return 0-失败, 1-成功
 */
int FX2N::readIOTag(void* pObj, IPort *pPort, IOTag* pTag)
{
    m_mitsubishiImplObj.setPort(pPort);
    if(!m_mitsubishiImplObj.isCanRead(pObj, pTag)) return 1;
    return m_mitsubishiImplObj.readData(pObj, pTag);
}


/**
 * @brief FX2N::afterReadIOTag
 * @details 读变量后处理
 * @param pTag 变量描述对象
 * @return false-失败, true-成功
 */
bool FX2N::afterReadIOTag(void* pObj, IOTag* pTag)
{
    (void)pTag;
    return true;
}





