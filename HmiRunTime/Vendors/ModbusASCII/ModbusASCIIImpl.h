#ifndef MODBUSASCIIIMPL_H
#define MODBUSASCIIIMPL_H

#include <QString>
#include <QObject>

#include "../../HmiRunTime/Public/public.h"
#include "Modbus.h"
#include "IPort.h"


class ModbusASCIIImpl : public Modbus
{
public:
    ModbusASCIIImpl();
    ~ModbusASCIIImpl();

public:
    quint8 getLRC(quint8 *auchMsg, qint16 usDataLen);
    bool messageCheck(quint8 *inBuf, qint16 bufLen);

    bool isCanWrite(void* pObj, IOTag* pTag);
    int writeData(void* pObj, IOTag* pTag);
    bool isCanRead(void* pObj, IOTag* pTag);
    int readData(void* pObj, IOTag* pTag);

private:
    // 生成modbus报文
    quint16 makeMessagePackage(quint8 *pSendData,
                               IOTag* pTag,
                               TModbus_ReadWrite RW_flag,
                               quint16 *retVarLen);
};




#endif // MODBUSASCIIIMPL_H







