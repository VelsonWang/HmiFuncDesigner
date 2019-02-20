#ifndef MODBUS_H
#define MODBUS_H

#include "iport.h"
#include "DataPack.h"
#include <QObject>

class Modbus : public QObject
{
    Q_OBJECT
public:
    explicit Modbus(QObject *parent = nullptr);
    virtual ~Modbus();

public:
    void set2BytesOrder(const QString &s);
    QString get2BytesOrder();
    void set3BytesOrder(const QString &s);
    QString get3BytesOrder();
    void set4BytesOrder(const QString &s);
    QString get4BytesOrder();
    void setFloatBytesOrder(const QString &s);
    QString getFloatBytesOrder();
    void SetPort(IPort *pPort);
    IPort *GetPort();

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
    void ClearBuffer();

signals:

public slots:

public:
    quint8 m_pReadBuf[512] = {0};
    quint8 m_pWriteBuf[512] = {0};
    quint8 m_pDataBuf[512] = {0};
    quint8 m_pTmpBuf[1024] = {0};
    int m_iCurReadAddress;

private:
    IPort *iFacePort_;
    QString str2BytesOrder_;
    QString str3BytesOrder_;
    QString str4BytesOrder_;
    QString strFloatBytesOrder_;
};

#endif // MODBUS_H
