#ifndef IPORT
#define IPORT

#include <QString>
#include <QThread>
#include <QStringList>

class IPort
{
public:
    virtual ~IPort(){}
    virtual bool open(QString port, QStringList args) = 0;
    virtual int read(unsigned char *buf, int len, int ms) = 0;
    virtual int write(unsigned char *buf, int len, int ms) = 0;
    virtual bool close() = 0;
};

#endif // IPORT

