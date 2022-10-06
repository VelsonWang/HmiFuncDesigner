#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "filetansfer.h"
#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QDataStream>
#include <QTime>
#include <QFile>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();

signals:
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void reStartRuntime();

public slots:
    void disConTcp(int i);

protected slots:
    void slotReadData();

private:
    qintptr socketID;
    QMetaObject::Connection dis;

private:
    void saveToFile(QByteArray fileBuf);
    void transferFilePackage();

private:
    int transferState = 0;
    TDataPackage dataPackage;
    QByteArray fileBuf;
    quint32 fileSize = 0;
};

#endif // TCPSOCKET_H
