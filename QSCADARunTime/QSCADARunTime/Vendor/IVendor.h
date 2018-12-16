#ifndef IVENDOR_H
#define IVENDOR_H

#include <QString>
#include <QStringList>
#include <QThread>
#include "IOTag.h"

class IOTag;

class IVendor
{
public:
    virtual ~IVendor() {}

    // 获取设备名称
    virtual QString GetDeviceName() = 0;
    // 打开设备
    virtual bool Open() = 0;
    // 设备初始化
    virtual void Initialize() = 0;
    // 添加设备变量至变量列表
    virtual void AddIOTagToDeviceTagList(IOTag* pTag) = 0;
    // 添加设备变量至变量写队列
    virtual void AddIOTagToDeviceTagWriteQueue(IOTag* pTag) = 0;
    // 查找设备变量
    virtual IOTag* FindIOTagByID(qint32 id) = 0;
    // 写变量
    virtual bool WriteIOTag(IOTag* pTag) = 0;
    // 写变量列表
    virtual bool WriteIOTags() = 0;
    // 读变量
    virtual bool ReadIOTag(IOTag* pTag) = 0;
    // 读变量列表
    virtual bool ReadIOTags() = 0;
    // 是否运行
    virtual bool IsRunning() = 0;
    // 启动
    virtual void Start() = 0;
    // 停止
    virtual void Stop() = 0;
    // 循环处理
    virtual void DoLoop() = 0;
    // 是否重启运行
    virtual bool IsRestart() = 0;
    // 是否在线
    virtual bool IsOnLine() = 0;
    // 关闭设备
    virtual bool Close() = 0;
    // 获取端口名称
    virtual QString GetPortName() = 0;

};


#endif // IVENDOR_H
