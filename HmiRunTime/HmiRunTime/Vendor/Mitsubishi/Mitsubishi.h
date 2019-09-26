#ifndef __MITSUBISHI_H__
#define __MITSUBISHI_H__

#include <QObject>
#include "IPort.h"
#include "IOTag.h"

class Mitsubishi : public QObject
{
    Q_OBJECT
public:
    explicit Mitsubishi(QObject *parent = nullptr);
    virtual ~Mitsubishi();

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

