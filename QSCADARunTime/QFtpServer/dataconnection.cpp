#include "dataconnection.h"
#include "sslserver.h"
#include "ftpcommand.h"
#include <QSslSocket>

DataConnection::DataConnection(QObject *parent) :
    QObject(parent)
{
    server = new SslServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    socket = 0;
    isSocketReady = false;
    isWaitingForFtpCommand = false;
}

void DataConnection::scheduleConnectToHost(const QString &hostName, int port, bool encrypt)
{
    this->encrypt = encrypt;
    delete socket;
    this->hostName = hostName;
    this->port = port;
    isSocketReady = false;
    isWaitingForFtpCommand = true;
    isActiveConnection = true;
}

int DataConnection::listen(bool encrypt)
{
    this->encrypt = encrypt;
    delete socket;
    socket = 0;
    delete command;
    command = 0;
    isSocketReady = false;
    isWaitingForFtpCommand = true;
    isActiveConnection = false;
    server->close();
    server->listen();
    return server->serverPort();
}

bool DataConnection::setFtpCommand(FtpCommand *command)
{
    if (!isWaitingForFtpCommand) {
        return false;
    }
    isWaitingForFtpCommand = false;
    this->command = command;
    command->setParent(this);

    if (isActiveConnection) {
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(connected()), SLOT(connected()));
        socket->connectToHost(hostName, port);
    } else {
        startFtpCommand();
    }
    return true;
}

FtpCommand *DataConnection::ftpCommand()
{
    if (isSocketReady) {
        return command;
    }
    return 0;
}

void DataConnection::newConnection()
{
    socket = (QSslSocket *) server->nextPendingConnection();
    server->close();
    if (encrypt) {
        connect(socket, SIGNAL(encrypted()), this, SLOT(encrypted()));
        SslServer::setLocalCertificateAndPrivateKey(socket);
        socket->startServerEncryption();
    } else {
        encrypted();
    }
}

void DataConnection::encrypted()
{
    isSocketReady = true;
    startFtpCommand();
}

void DataConnection::connected()
{
    if (encrypt) {
        connect(socket, SIGNAL(encrypted()), this, SLOT(encrypted()));
        SslServer::setLocalCertificateAndPrivateKey(socket);
        socket->startServerEncryption();
    } else {
        encrypted();
    }
}

void DataConnection::startFtpCommand()
{
    if (command && isSocketReady) {
        command->start(socket);
        socket = 0;
    }
}
