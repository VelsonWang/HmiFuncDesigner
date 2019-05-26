#ifndef TCPIPModbus_H
#define TCPIPModbus_H

#include <QString>
#include <QObject>

#include "../../Public/public.h"
#include "IVendor.h"
#include "ComPort.h"
#include "IPort.h"
#include "Modbus.h"


class TCPIPModbus : public Modbus
{
public:
    TCPIPModbus(QObject *parent = nullptr);
    ~TCPIPModbus();

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

private:
    quint8 tempBuffer_[512] = {0};
};

#endif // TCPIPModbus_H





