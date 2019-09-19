#ifndef __S7_200_H__
#define __S7_200_H__

#include <QObject>
#include "IPort.h"
#include "IOTag.h"

class S7_200 : public QObject
{
    Q_OBJECT
public:
    explicit S7_200(QObject *parent = nullptr);
    virtual ~S7_200();

public:
    void setPort(IPort *pPort);
    IPort *getPort();
    bool isCanWrite(IOTag* pTag);
    int writeData(IOTag* pTag);
    bool isCanRead(IOTag* pTag);
    int readData(IOTag* pTag);

private:
    IPort *iFacePort_;
    quint8 readDataBuffer_[512] = {0};
    quint8 writeDataBuffer_[512] = {0};
};

#endif

