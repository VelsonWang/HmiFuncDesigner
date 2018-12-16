#ifndef COMPORT_H
#define COMPORT_H

#include <QObject>
#include <QTimer>

#include "qextserial/qextserialport.h"
#include "iport.h"


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
    QextSerialPort *m_pSerialPort;
    unsigned char* m_pReadBuf;
    unsigned char* m_pWriteBuf;
    int m_ReadIndex;
    int m_WriteIndex;

private:
    int m_iWaitBytesNum;

};

#endif // COMPORT_H
