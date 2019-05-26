#ifndef MODBUSRTUDEVICE_H
#define MODBUSRTUDEVICE_H

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
#include "ComPort.h"
#include "ModbusRTU.h"

class ModbusRTUDevice : public IVendor
{
public:
    explicit ModbusRTUDevice();
    ~ModbusRTUDevice();
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
    // 写变量前处理
    virtual bool BeforeWriteIOTag(IOTag* pTag);
    // 写变量
    virtual bool WriteIOTag(IOTag* pTag);
    // 写变量后处理
    virtual bool AfterWriteIOTag(IOTag* pTag);
    // 写变量列表
    virtual bool WriteIOTags();
    // 读变量前处理
    virtual bool BeforeReadIOTag(IOTag* pTag);
    // 读变量
    virtual bool ReadIOTag(IOTag* pTag);
    // 读变量后处理
    virtual bool AfterReadIOTag(IOTag* pTag);
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
    ComPort * comPort_;
    ComDevicePrivate *pComDevicePrivate_;
    ModbusRTU modbusRTU_;

};

#endif // MODBUSRTUDEVICE_H
