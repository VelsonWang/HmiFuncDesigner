#ifndef TCPIPModbusDEVICE_H
#define TCPIPModbusDEVICE_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include "IOTag.h"
#include "IVendor.h"
#include "VendorPrivate.h"
#include "IPort.h"
#include "public.h"
#include "TCPIPModbus.h"

class TCPIPModbusDevice : public IVendor
{
public:
    explicit TCPIPModbusDevice();
    ~TCPIPModbusDevice();
    bool LoadData(SaveFormat saveFormat, QString fileName);

public:
    // 获取设备名称
    virtual QString GetDeviceName();
    // 打开设备
    virtual bool Open();
    // 设备初始化
    virtual void Initialize();
    // 添加设备变量至变量列表
    virtual void AddIOTagToDeviceTagList(IOTag* pTag);
    // 添加设备变量至变量写队列
    virtual void AddIOTagToDeviceTagWriteQueue(IOTag* pTag);
    // 查找设备变量
    virtual IOTag* FindIOTagByID(qint32 id);
    // 写变量
    virtual bool WriteIOTag(IOTag* pTag);
    // 写变量列表
    virtual bool WriteIOTags();
    // 读变量
    virtual bool ReadIOTag(IOTag* pTag);
    // 读变量列表
    virtual bool ReadIOTags();
    // 是否运行
    virtual bool IsRunning();
    // 启动
    virtual void Start();
    // 停止
    virtual void Stop();
    // 循环处理
    virtual void DoLoop();
    // 是重启运行
    virtual bool IsRestart();
    // 是否在线
    virtual bool IsOnLine();
    // 关闭设备
    virtual bool Close();
    // 获取端口名称
    virtual QString GetPortName();

private:
    IPort *iFacePort;
    NetDevicePrivate *pNetDevicePrivate;
    QQueue<IOTag *> mWriteQueue;
    QList<IOTag *> mReadList;
    bool mbIsRunning;
    TCPIPModbus mTCPIPModbus;
    qint32 miFailCnt; // 通信失败次数
    QMutex m_WriteMutex;
};

#endif // TCPIPModbusDEVICE_H
