#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>

class QSslSocket;

// A simple SSL server. Returns a QSslSocket instead of a QTcpSocket, but
// started in unencrypted mode, so it can be used as a regular QTcpServer, as
// well. The QSslSockets are loaded with a default certificate coming from the
// resource files.

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslServer(QObject *parent);

    // Sets the local certificate and private key for the socket, so
    // startServerEncryption() can be used. We get the local certificate and
    // private key from the application's resources.
    static void setLocalCertificateAndPrivateKey(QSslSocket *socket);

signals:

private:
    // Workaround so we support both Qt 5 and Qt 4.
    // QTcpServer::incomingConnection(int handle) has been changed to
    // QTcpServer::incomingConnection(qintptr handle) this 2nd one is sneaky as it
    // compiles properly but no connections appear to arrive since the compiler
    // doesn’t consider int and qintptr the same.
#if QT_VERSION >= 0x050000
    typedef qintptr PortableSocketDescriptorType;
#else
    typedef int PortableSocketDescriptorType;
#endif

    void incomingConnection(PortableSocketDescriptorType socketDescriptor);
};

#endif // SSLSERVER_H
