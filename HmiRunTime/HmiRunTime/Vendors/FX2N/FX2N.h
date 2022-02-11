#ifndef FX2N_H
#define FX2N_H

#include <QObject>
#include "../IVendorPlugin/IVendorPlugin.h"
#include "MitsubishiImpl.h"

class FX2N : public QObject, IVendorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DevicePluginInterface_iid FILE "FX2N.json")
    Q_INTERFACES(IVendorPlugin)

public:
    FX2N();
    ~FX2N() Q_DECL_OVERRIDE;

    // 初始化设备
    bool initailizeDevice(void* pObj) Q_DECL_OVERRIDE;
    // 连接设备
    bool connectDevice(void* pObj) Q_DECL_OVERRIDE;
    // 断开设备连接
    bool disconnectDevice(void* pObj) Q_DECL_OVERRIDE;
    // 反初始化设备
    bool unInitailizeDevice(void* pObj) Q_DECL_OVERRIDE;

    // 写变量前处理
    bool beforeWriteIOTag(void* pObj, IOTag* pTag) Q_DECL_OVERRIDE;
    // 写变量
    int writeIOTag(void* pObj, IPort *pPort, IOTag* pTag) Q_DECL_OVERRIDE;
    // 写变量后处理
    bool afterWriteIOTag(void* pObj, IOTag* pTag) Q_DECL_OVERRIDE;

    // 读变量前处理
    bool beforeReadIOTag(void* pObj, IOTag* pTag) Q_DECL_OVERRIDE;
    // 读变量
    int readIOTag(void* pObj, IPort *pPort, IOTag* pTag) Q_DECL_OVERRIDE;
    // 读变量后处理
    bool afterReadIOTag(void* pObj, IOTag* pTag) Q_DECL_OVERRIDE;

    // 变量字节序转换为当前主机字节序
    bool convertIOTagBytesToNativeBytes(void* pObj, IOTag* pTag) Q_DECL_OVERRIDE;

private:
    MitsubishiImpl m_mitsubishiImplObj;
};




#endif // FX2N_H
