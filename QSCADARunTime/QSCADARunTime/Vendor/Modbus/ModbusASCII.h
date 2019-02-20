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
    unsigned char GetLRC(unsigned char *auchMsg, short usDataLen);
    bool MessageCheck(unsigned char *inBuf,short bufLen);

    // 0x00
    ResultType ReadCoils(int addDev, int addCol, int num, unsigned char *pbuf=0);
    ResultType WriteCoil(int addDev, int addCol, int value);
    ResultType WriteMultipleCoils(int addDev, int addCol, int num, int cntByte, unsigned char *pbuf=0);

    // 0x10001
    ResultType ReadDiscreteInputs(int addDev, int addinput, int num, unsigned char *pbuf=0);

    // 0x30001
    ResultType ReadReadInputRegister(int addDev, int addReg, int num, unsigned char *pbuf=0);

    // 0x40001
    ResultType ReadHoldingRegister(int addDev, int addReg, int num, unsigned char *pbuf=0);
    ResultType WriteHoldingRegister(int addDev, int addReg, unsigned short data);
    ResultType WriteMultipleHoldingRegister(int addDev, int addReg, int num, unsigned char *pbuf=0);
    ResultType WriteIntToHoldingRegister(int addDev, int addReg, int data);
    ResultType WriteUIntToHoldingRegister(int addDev, int addReg, unsigned int data);
    ResultType WriteFloatToHoldingRegister(int addDev, int addReg, float data);

};

#endif // ModbusASCII_H





