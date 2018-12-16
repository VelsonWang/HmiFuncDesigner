#ifndef NetPort_H
#define NetPort_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "iport.h"

// 单独线程 因为QTcpSocket跨线程操作
// QSocketNotifier: Socket notifiers cannot be enabled or disabled from another thread
class NetPortThread : public QThread
{
    Q_OBJECT
public:
    NetPortThread();
    ~NetPortThread();
    bool open(QString port, QStringList args);
    int read(unsigned char *buf, int len, int ms);
    int write(unsigned char *buf, int len, int ms);
private:
    void ConnectToHost();

signals:
    int SignalSockWrite(unsigned char *buf, int len);

private slots:
    void SlotConnected();
    void SlotDisconnected();
    void DataReceived();
    int SlotSockWrite(unsigned char *buf, int len);

private:
    int m_iPort;
    QHostAddress *m_pServerIP;
    QTcpSocket *m_pTcpSocket;
    bool status;
    unsigned char* m_pReadBuf;
    unsigned char* m_pWriteBuf;
    int m_ReadIndex;
    int m_WriteIndex;
    int m_iWaitBytesNum;
    bool m_bConnected;

protected:
    void run();

};

class NetPort : public QObject, public IPort
{
    Q_OBJECT

public:
    NetPort();
    ~NetPort();

public:
    bool open(QString port, QStringList args);
    int read(unsigned char *buf, int len, int ms);
    int write(unsigned char *buf, int len, int ms);
    bool close();

private:    
    NetPortThread m_NetPortThread;

};

#endif // NetPort_H
