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
    ~ComPort();

public:
    bool open(QString port, QStringList args);
    int read(unsigned char *buf, int len, int ms);
    int write(unsigned char *buf, int len, int ms);
    bool close();

private:
    QextSerialPort *serialPortPtr_;
    QByteArray buf_;

};

#endif // COMPORT_H
