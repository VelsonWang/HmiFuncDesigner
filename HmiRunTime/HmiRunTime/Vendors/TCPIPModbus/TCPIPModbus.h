#ifndef TCPIPMODBUS_H
#define TCPIPMODBUS_H

#include <QObject>
#include "../IVendorPlugin/IVendorPlugin.h"
#include "TCPIPModbusImpl.h"


class TCPIPModbus : public QObject, IVendorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DevicePluginInterface_iid FILE "TCPIPModbus.json")
    Q_INTERFACES(IVendorPlugin)

public:
    TCPIPModbus();
    ~TCPIPModbus() Q_DECL_OVERRIDE;

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
    TCPIPModbusImpl m_TCPIPModbusImplObj;

};

#endif // TCPIPMODBUS_H
