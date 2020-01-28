#ifndef SIEMENSS7_200IMPL_H
#define SIEMENSS7_200IMPL_H

#include <QString>
#include <QObject>

#include "../../HmiRunTime/Public/public.h"
#include "IOTag.h"
#include "IPort.h"


class SiemensS7_200Impl
{
public:
    SiemensS7_200Impl();
    ~SiemensS7_200Impl();

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




#endif // SIEMENSS7_200IMPL_H





