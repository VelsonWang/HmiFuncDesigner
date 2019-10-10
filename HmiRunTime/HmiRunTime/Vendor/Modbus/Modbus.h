#ifndef MODBUS_H
#define MODBUS_H

#include "IPort.h"
#include "DataPack.h"
#include "IOTag.h"
#include <QObject>

typedef enum _TModbusVerifyMode
{
    VerifyMode_CRC = 0,
    VerifyMode_LRC = 1
}TModbusVerifyMode;

typedef enum _TModbus_CPUMEM
{
    CM_NON = 0,
    CM_0x = 1,
    CM_1x = 2,
    CM_3x = 3,
    CM_4x = 4
}TModbus_CPUMEM;

typedef enum _TModbus_ReadWrite
{
    FLAG_NON = 0,
    FLAG_READ = 1,
    FLAG_WRITE = 2
}TModbus_ReadWrite;

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
    quint8 getFuncode(IOTag* pTag, TModbus_ReadWrite rw_flag);
    // 获得寄存器个数
    quint16 getRegNum(IOTag* pTag);
    // 生成modbus报文
    virtual quint16 makeMessagePackage(quint8 */* pSendData */,
                                       IOTag* /* pTag */,
                                       TModbus_ReadWrite /* RW_flag */,
                                       quint16 */* retVarLen */){return 0;}

    virtual bool isCanWrite(IOTag* /* pTag */){return false;}
    virtual int writeData(IOTag* /* pTag */){return 0;}
    virtual bool isCanRead(IOTag* /* pTag */){return false;}
    virtual int readData(IOTag* /* pTag */){return 0;}

    // 加入块读变量至待读变量标签列表
    void insertBlockReadTagToReadList(QList<IOTag *> &listRead);

signals:

public slots:

public:
    quint8 readDataBuffer_[512] = {0};
    quint8 writeDataBuffer_[512] = {0};
    quint8 tempBuffer_[512] = {0};

private:
    IPort *iFacePort_;

};


#endif // MODBUS_H
