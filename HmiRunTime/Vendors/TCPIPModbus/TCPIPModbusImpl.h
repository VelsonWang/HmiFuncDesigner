#ifndef TCPIPMODBUSIMPL_H
#define TCPIPMODBUSIMPL_H

#include <QString>
#include <QObject>

#include "../../HmiRunTime/Public/public.h"
#include "Modbus.h"
#include "IPort.h"


class TCPIPModbusImpl : public Modbus
{
public:
    TCPIPModbusImpl();
    ~TCPIPModbusImpl();

public:
    bool isCanWrite(void* pObj, IOTag* pTag);
    int writeData(void* pObj, IOTag* pTag);
    bool isCanRead(void* pObj, IOTag* pTag);
    int readData(void* pObj, IOTag* pTag);

private:
    // 生成modbus报文
    quint16 makeMessagePackage(quint8 *pSendData,
                               void* pObj,
                               IOTag* pTag,
                               TModbus_ReadWrite RW_flag,
                               quint16 *retVarLen);
};




#endif // TCPIPMODBUSIMPL_H







