#ifndef ModbusASCII_H
#define ModbusASCII_H

#include "Modbus.h"
#include "../../Public/public.h"
#include "IVendor.h"
#include <QString>
#include <QObject>


class ModbusASCII : public Modbus
{
public:
    ModbusASCII();
    ~ModbusASCII();

public:
    quint8 getLRC(quint8 *auchMsg, qint16 usDataLen);
    bool messageCheck(quint8 *inBuf,qint16 bufLen);

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

#endif // ModbusASCII_H





