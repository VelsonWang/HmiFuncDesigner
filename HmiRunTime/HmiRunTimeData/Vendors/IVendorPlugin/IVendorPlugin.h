#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H

#include "../../HmiRunTimeData/Port/IPort.h"
#include "../../HmiRunTimeData/Tag/IOTag.h"
#include <QStringList>


/*
* 注意本类不要派生自QObject
*/
class IVendorPlugin
{
public:
    virtual ~IVendorPlugin() {}

    // 初始化设备
    virtual bool initailizeDevice(void* pObj) = 0;
    // 连接设备
    virtual bool connectDevice(void* pObj) = 0;
    // 断开设备连接
    virtual bool disconnectDevice(void* pObj) = 0;
    // 反初始化设备
    virtual bool unInitailizeDevice(void* pObj) = 0;

    // 写变量前处理
    virtual bool beforeWriteIOTag(void* pObj, IOTag* pTag) = 0;
    // 写变量
    virtual int writeIOTag(void* pObj, IPort *pPort, IOTag* pTag) = 0;
    // 写变量后处理
    virtual bool afterWriteIOTag(void* pObj, IOTag* pTag) = 0;

    // 读变量前处理
    virtual bool beforeReadIOTag(void* pObj, IOTag* pTag) = 0;
    // 读变量
    virtual int readIOTag(void* pObj, IPort *pPort, IOTag* pTag) = 0;
    // 读变量后处理
    virtual bool afterReadIOTag(void* pObj, IOTag* pTag) = 0;

    // 变量字节序转换为当前主机字节序
    virtual bool convertIOTagBytesToNativeBytes(void* pObj, IOTag* pTag) = 0;
};


QT_BEGIN_NAMESPACE

#define DevicePluginInterface_iid "HmiRunTime.Device.PluginInterface"
Q_DECLARE_INTERFACE(IVendorPlugin, DevicePluginInterface_iid)

QT_END_NAMESPACE

#endif // IDEVICEPLUGIN_H
