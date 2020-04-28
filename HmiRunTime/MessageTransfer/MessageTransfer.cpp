#include "MessageTransfer.h"
#include <QTimer>
#include <QThread>
#include <QDataStream>
#include <QTime>
#include <QDebug>

const char* MessageTransfer::ack = "ack";

MessageTransfer::MessageTransfer(bool bServer, const QString szServerName) {
    socketName = szServerName;
    if(bServer) {
        server = new QLocalServer(this);
        server->listen(socketName);
        QObject::connect(server, SIGNAL(newConnection()), SLOT(receiveConnection()));
        //qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__ << "socketName: " << socketName;
    }
}

MessageTransfer::~MessageTransfer() {

}



bool MessageTransfer::sendMessage(const QString &message, int timeout)
{
    //qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__ << "message: " << message;
    QLocalSocket socket;
    bool connOk = false;
    for(int i = 0; i < 2; i++) {
        // Try twice, in case the other instance is just starting up
        socket.connectToServer(socketName);
        connOk = socket.waitForConnected(timeout/2);
        if (connOk || i)
            break;
        int ms = 250;
        QThread::msleep(ms);
    }
    if (!connOk)
        return false;

    QByteArray uMsg(message.toUtf8());
    QDataStream ds(&socket);
    ds.writeBytes(uMsg.constData(), uMsg.size());
    bool res = socket.waitForBytesWritten(timeout);
    if (res) {
        res &= socket.waitForReadyRead(timeout);   // wait for ack
        if (res)
            res &= (socket.read(qstrlen(ack)) == ack);
    }
    return res;
}


void MessageTransfer::receiveConnection() {
    QLocalSocket* socket = server->nextPendingConnection();
    if (!socket)
        return;

    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead();
    QDataStream ds(socket);
    QByteArray uMsg;
    quint32 remaining;
    ds >> remaining;
    uMsg.resize(remaining);
    int got = 0;
    char* uMsgBuf = uMsg.data();
    do {
        got = ds.readRawData(uMsgBuf, remaining);
        remaining -= got;
        uMsgBuf += got;
    } while (remaining && got >= 0 && socket->waitForReadyRead(2000));
    if (got < 0) {
        qWarning("QtLocalPeer: Message reception failed %s", socket->errorString().toLatin1().constData());
        delete socket;
        return;
    }
    QString message(QString::fromUtf8(uMsg));
    socket->write(ack, qstrlen(ack));
    socket->waitForBytesWritten(1000);
    socket->waitForDisconnected(1000); // make sure client reads ack
    delete socket;
    //qDebug() <<__FILE__ << __LINE__ <<__FUNCTION__ << "messageReceived: " << message;
    emit messageReceived(message); //### (might take a long time to return)
}


