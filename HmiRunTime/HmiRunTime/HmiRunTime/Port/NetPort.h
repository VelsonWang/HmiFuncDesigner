#ifndef NetPort_H
#define NetPort_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "IPort.h"
//#include "xsocket.hpp"
#include <QtNetwork>

class NetPort : public QObject, public IPort
{
    Q_OBJECT

public:
    NetPort(QString sIp, int tPort);
    ~NetPort();

public:
	QTcpSocket* getSocket();
    bool open(QString port, QStringList args) Q_DECL_OVERRIDE;
    bool reOpen() Q_DECL_OVERRIDE;
    int read(unsigned char *buf, int len, int ms) Q_DECL_OVERRIDE;
    int write(unsigned char *buf, int len, int ms) Q_DECL_OVERRIDE;
    bool close() Q_DECL_OVERRIDE;
    TPortType getPortType() Q_DECL_OVERRIDE;

private:
	QTcpSocket *client;
    int port_;
    QString ip_;
};

#endif // NetPort_H
