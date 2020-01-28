#ifndef MITSUBISHIIMPL_H
#define MITSUBISHIIMPL_H

#include <QString>
#include <QObject>

#include "../../HmiRunTime/Public/public.h"
#include "IOTag.h"
#include "IPort.h"


class MitsubishiImpl
{
public:
    MitsubishiImpl();
    ~MitsubishiImpl();

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




#endif // MITSUBISHIIMPL_H





