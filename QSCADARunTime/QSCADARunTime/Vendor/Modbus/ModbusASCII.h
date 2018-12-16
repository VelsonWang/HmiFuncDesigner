#ifndef ModbusASCII_H
#define ModbusASCII_H

#include <QString>
#include <QObject>

#include "../../Public/public.h"
#include "IVendor.h"
#include "ComPort.h"
#include "iport.h"



class ModbusASCII : public QObject
{
public:
    ModbusASCII();
    ~ModbusASCII();

public:
    void set2BytesOrder(QString s);
    void set3BytesOrder(QString s);
    void set4BytesOrder(QString s);
    void setFloatBytesOrder(QString s);
    void SetPort(IPort *pPort);

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

public:
    unsigned char GetBoolValue();
    unsigned char GetBoolValue(int add);
    short GetShortValue();
    short GetShortValue(int add);
    unsigned short GetUnsignedShortValue();
    unsigned short GetUnsignedShortValue(int add);
    int GetIntValue();
    int GetIntValue(int add);
    unsigned int GetUnsignedIntValue();
    unsigned int GetUnsignedIntValue(int add);
    float GetFloatValue();
    float GetFloatValue(int add);

private:
    void ClearBuffer();

private:
    IPort *iFacePort;
    unsigned char* m_pReadBuf;
    unsigned char* m_pWriteBuf;
    unsigned char* m_pDataBuf;
    unsigned char* m_pTmpBuf;
    int m_iCurReadAddress;

    QString m_str2BytesOrder;
    QString m_str3BytesOrder;
    QString m_str4BytesOrder;
    QString m_strFloatBytesOrder;
};

#endif // ModbusASCII_H





