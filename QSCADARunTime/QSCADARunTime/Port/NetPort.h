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
    int port_;
    QHostAddress *serverIPPtr_;
    QTcpSocket *tcpSocketPtr_;
    bool status_;
    QByteArray buf_;
    bool bConnected_;

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
    NetPortThread netPortThread_;

};

#endif // NetPort_H
