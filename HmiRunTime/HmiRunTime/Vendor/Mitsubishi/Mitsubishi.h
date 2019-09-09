#ifndef __MITSUBISHI_H__
#define __MITSUBISHI_H__

#include <QObject>
#include "IPort.h"
#include "IOTag.h"

///////////////////////////////////////////////////////////////////////
//Mitsubish_FS型号协议参数数据结构定义
enum _TMitsubishi_CPUTYPE
{
    CPU_FX0N 	= 0,
    CPU_FX0		= 1,
    CPU_FX0S 	= 2,
    CPU_FX1S 	= 3,
    CPU_FX1N_C 	= 4,
    CPU_FXU 	= 5,
    CPU_FX2C 	= 6,
    CPU_FX 		= 7,
    CPU_FX2 	= 8,
    CPU_FX2NC 	= 9,
    CPU_FX2N 	= 10
};
typedef enum _TMitsubishi_CPUTYPE TMitsubishi_FS_CPUTYPE;

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

