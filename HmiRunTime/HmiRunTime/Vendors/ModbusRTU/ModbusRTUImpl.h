#ifndef MODBUSRTUIMPL_H
#define MODBUSRTUIMPL_H

#include <QString>
#include <QObject>
#include "../../HmiRunTime/shared/realtimedb.h"
#include "../../HmiRunTime/shared/public.h"
#include "../Public/Modbus.h"
#include "../../HmiRunTime/Port/IPort.h"


class ModbusRTUImpl : public Modbus
{
public:
    ModbusRTUImpl();
    ~ModbusRTUImpl();

public:
    quint16 crc16(quint8 *pbuf, qint32 len);
    bool messageCheck(quint8 *inBuf, qint16 bufLen);

    bool isCanWrite(void* pObj, RunTimeTag* pTag) override;
    int writeData(void* pObj, RunTimeTag* pTag) override;
    bool isCanRead(void* pObj, RunTimeTag* pTag) override;
    int readData(void* pObj, RunTimeTag* pTag) override;

private:
    // 生成modbus报文
    quint16 makeMessagePackage(quint8 *pSendData,
                               void* pObj,
                               RunTimeTag* pTag,
                               TModbus_ReadWrite RW_flag,
                               quint16 *retVarLen) override;
};




#endif // MODBUSRTUIMPL_H





