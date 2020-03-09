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

    bool isCanWrite(void* pObj, IOTag* pTag);
    int writeData(void* pObj, IOTag* pTag);
    bool isCanRead(void* pObj, IOTag* pTag);
    int readData(void* pObj, IOTag* pTag);

private:
    IPort *iFacePort_;
};




#endif // SIEMENSS7_200IMPL_H





