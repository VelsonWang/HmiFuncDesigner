#include "sslserver.h"

#include <QSslSocket>
#include <QSslKey>

SslServer::SslServer(QObject *parent) :
    QTcpServer(parent)
{
}

void SslServer::setLocalCertificateAndPrivateKey(QSslSocket *socket)
{
    socket->setPrivateKey(":/ssl/privkey.pem", QSsl::Rsa, QSsl::Pem, "39129380423984234012312");
    Q_ASSERT(!socket->privateKey().isNull());
    socket->setLocalCertificate(":/ssl/cacert.pem");
    Q_ASSERT(!socket->localCertificate().isNull());
}

void SslServer::incomingConnection(PortableSocketDescriptorType socketDescriptor)
{
    QSslSocket *socket = new QSslSocket(this);
    if (socket->setSocketDescriptor(socketDescriptor)) {
        addPendingConnection(socket);
    } else {
        delete socket;
    }
}
