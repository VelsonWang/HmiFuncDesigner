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
    bool isCanWrite(IOTag* pTag);
    int writeData(IOTag* pTag);
    bool isCanRead(IOTag* pTag);
    int readData(IOTag* pTag);

private:
    // 生成modbus报文
    quint16 makeMessagePackage(quint8 *pSendData,
                               IOTag* pTag,
                               TModbus_ReadWrite RW_flag,
                               quint16 *retVarLen);
};




#endif // TCPIPMODBUSIMPL_H







