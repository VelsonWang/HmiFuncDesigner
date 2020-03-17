#ifndef IPORT
#define IPORT

#include <QString>
#include <QThread>
#include <QStringList>

// 端口类型
enum _PortType {
    PORT_NONE = 0,
    PORT_SERIAL = 1,
    PORT_NET = 2,
    PORT_RESERVE = 255 // 预留
};
typedef enum _PortType TPortType;

class IPort
{
public:
    virtual ~IPort(){}
    virtual bool open(QString port, QStringList args) = 0;
    virtual bool reOpen() = 0;
    virtual int read(unsigned char *buf, int len, int ms) = 0;
    virtual int write(unsigned char *buf, int len, int ms) = 0;
    virtual bool close() = 0;
    virtual TPortType getPortType() = 0;
};

#endif // IPORT

