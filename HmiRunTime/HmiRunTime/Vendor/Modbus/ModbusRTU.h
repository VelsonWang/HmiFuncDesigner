#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QString>
#include <QObject>

#include "../../Public/public.h"
#include "Modbus.h"
#include "IVendor.h"
#include "ComPort.h"
#include "IPort.h"


class ModbusRTU : public Modbus
{
public:
    ModbusRTU();
    ~ModbusRTU();

public:
    quint16 crc16(quint8 *pbuf, qint32 len);
    bool messageCheck(quint8 *inBuf, qint16 bufLen);

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




#endif // MODBUSRTU_H





