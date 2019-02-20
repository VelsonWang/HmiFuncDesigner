#ifndef IVENDOR_H
#define IVENDOR_H

#include <QString>
#include <QStringList>
#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <QQueue>
#include <QList>
#include "IOTag.h"
#include "IPort.h"

class IOTag;

class IVendor
{
public:
    IVendor()
    {
        iFacePort = nullptr;
        miFailCnt = 0;
        mReadList.clear();
        mWriteQueue.clear();
        mbIsRunning = false;
    }
    virtual ~IVendor()
    {
        Stop();

        qDeleteAll(mReadList);
        mReadList.clear();

        if(!mWriteQueue.isEmpty())
        {
            qDeleteAll(mWriteQueue);
            mWriteQueue.clear();
        }

        if(iFacePort != nullptr)
        {
            iFacePort->close();
        }
    }

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
    virtual void Stop(){ }
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

public:
    quint8 readBuf[512] = {0};
    quint8 writeBuf[512] = {0};

    void ClearReadBuffer()
    {
        for(int i=0; i<sizeof(readBuf)/sizeof(quint8); i++)
            readBuf[i] = 0;
    }

    void ClearWriteBuffer()
    {
        for(int i=0; i<sizeof(writeBuf)/sizeof(quint8); i++)
            writeBuf[i] = 0;
    }

public:
    IPort *iFacePort;
    QQueue<IOTag *> mWriteQueue;
    QList<IOTag *> mReadList;
    bool mbIsRunning;
    qint32 miFailCnt; // 通信失败次数
    QMutex m_WriteMutex;
};


#endif // IVENDOR_H
