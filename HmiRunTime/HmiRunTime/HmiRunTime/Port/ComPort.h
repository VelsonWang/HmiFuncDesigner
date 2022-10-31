#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QTimer>

#include "qextserial/qextserialport.h"
#include "IPort.h"


class ComPort : public QObject, public IPort
{
    Q_OBJECT

public:
    ComPort();
    ~ComPort() Q_DECL_OVERRIDE;

public:
    bool open(QString port, QStringList args) Q_DECL_OVERRIDE;
    bool reOpen() Q_DECL_OVERRIDE;
    int read(unsigned char *buf, int len, int ms) Q_DECL_OVERRIDE;
    int write(unsigned char *buf, int len, int ms) Q_DECL_OVERRIDE;
    bool close() Q_DECL_OVERRIDE;
    TPortType getPortType() Q_DECL_OVERRIDE;

private:
    QextSerialPort *serialPortPtr;
    QByteArray buf;

};

#endif // COMPORT_H
