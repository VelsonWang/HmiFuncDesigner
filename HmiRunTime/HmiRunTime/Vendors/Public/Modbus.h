#ifndef MODBUS_H
#define MODBUS_H

#include "../../HmiRunTime/Port/IPort.h"
#include "DataPack.h"
#include "../../HmiRunTime/RunTimeTag.h"
#include <QObject>
#include <cstring>
#include <cstdlib>

typedef enum _TModbusVerifyMode {
    VerifyMode_CRC = 0,
    VerifyMode_LRC = 1
} TModbusVerifyMode;

typedef enum _TModbus_CPUMEM {
    CM_NON = 0,
    CM_0x = 1,
    CM_1x = 2,
    CM_3x = 3,
    CM_4x = 4
} TModbus_CPUMEM;

typedef enum _TModbus_ReadWrite {
    FLAG_NON = 0,
    FLAG_READ = 1,
    FLAG_WRITE = 2
} TModbus_ReadWrite;

class Modbus : public QObject
{
    Q_OBJECT
public:
    explicit Modbus(QObject *parent = nullptr);
    virtual ~Modbus();

public:
    void setPort(IPort *pPort);
    IPort *getPort();

    TModbus_CPUMEM getCpuMem(const QString &szRegisterArea);
    //获取功能码
    quint8 getFuncode(void* pObj, RunTimeTag* pTag, TModbus_ReadWrite rw_flag);
    // 获得寄存器个数
    quint16 getRegNum(RunTimeTag* pTag);
    // 生成modbus报文
    virtual quint16 makeMessagePackage(quint8 */* pSendData */,
                                       RunTimeTag* /* pTag */,
                                       TModbus_ReadWrite /* RW_flag */,
                                       quint16 */* retVarLen */)
    {
        return 0;
    }

    virtual bool isCanWrite(void* /* pObj */, RunTimeTag* /* pTag */)
    {
        return false;
    }
    virtual int writeData(void* /* pObj */, RunTimeTag* /* pTag */)
    {
        return 0;
    }
    virtual bool isCanRead(void* /* pObj */, RunTimeTag* /* pTag */)
    {
        return false;
    }
    virtual int readData(void* /* pObj */, RunTimeTag* /* pTag */)
    {
        return 0;
    }

    // 加入块读变量至待读变量标签列表
    void insertBlockReadTagToReadList(QList<RunTimeTag *> &listRead);

    // 依据规则修改缓冲区内数据
    void modbusChangeData(bool bAddr8, bool bAddr16, bool bAddr32,
                          bool bAddr64, QByteArray &data, quint32 iLen);
    // 设备地址
    int devAddr(void *pObj);
    // 内存地址起始位是否为0
    bool isStartAddrBit(void *pObj);
    // 写线圈功能码是否为15
    bool isWriteCoilFn(void *pObj);
    // 写寄存器功能码为是否16
    bool isWriteRegFn(void *pObj);
    // 是否8位逆序
    bool isAddr8(void *pObj);
    // 是否16位低字节在前高字节在后
    bool isAddr16(void *pObj);
    // 是否32位低字节在前高字节在后
    bool isAddr32(void *pObj);
    // 是否64位低字节在前高字节在后
    bool isAddr64(void *pObj);

    // 变量字节序转换为当前主机字节序
    bool convertIOTagBytesToNativeBytes(void* pObj, RunTimeTag* pTag);

signals:

public slots:

public:
    quint8 tempBuffer_[512] = {0};

private:
    IPort *iFacePort_;

};


#endif // MODBUS_H
