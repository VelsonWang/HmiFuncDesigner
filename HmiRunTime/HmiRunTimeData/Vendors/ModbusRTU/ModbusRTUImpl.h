#ifndef MODBUSRTUIMPL_H
#define MODBUSRTUIMPL_H

#include <QString>
#include <QObject>

#include "../../HmiRunTimeData/Public/public.h"
#include "Modbus.h"
#include "IPort.h"


class ModbusRTUImpl : public Modbus
{
public:
    ModbusRTUImpl();
    ~ModbusRTUImpl();

public:
    quint16 crc16(quint8 *pbuf, qint32 len);
    bool messageCheck(quint8 *inBuf, qint16 bufLen);

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




#endif // MODBUSRTUIMPL_H





