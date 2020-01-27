#ifndef NetPort_H
#define NetPort_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include "IPort.h"
#include "xsocket.hpp"


class NetPort : public QObject, public IPort
{
    Q_OBJECT

public:
    NetPort();
    ~NetPort();

public:
    bool open(QString port, QStringList args);
    bool reOpen();
    int read(unsigned char *buf, int len, int ms);
    int write(unsigned char *buf, int len, int ms);
    bool close();
    TPortType getPortType();

private:
    net::socket sock_;
    net::endpoint endpoint_;
    int port_;
    QString ip_;
};

#endif // NetPort_H
