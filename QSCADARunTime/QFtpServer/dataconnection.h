#ifndef PASSIVEDATACONNECTION_H
#define PASSIVEDATACONNECTION_H

class SslServer;
class QSslSocket;
class FtpCommand;

#include <QObject>
#include <QPointer>

// This class encapsulates the logic of the creation of both passive and active
// data connections. In short - there can be only one data connection per
// control connection. If a new one is requested while there is one being used
// currently, the operation using it is aborted. A new data connection starts
// when a PASV or PORT command is issued. Then the user must specify a command
// that uses the data connection, such as LIST or RETR/STOR. If we have
// encryption enabled, the data connection is also first encrypted. Once we
// have the command, and are connected to the client, and the connection has
// been encrypted, we run the command.

class DataConnection : public QObject
{
    Q_OBJECT
public:
    explicit DataConnection(QObject *parent = 0);

    // Connects to a host. Any existing data connections
    // or commands are aborted.
    void scheduleConnectToHost(const QString &hostName, int port, bool encrypt);

    // Starts listening for new data connections. Any existing data connections
    // or commands are aborted.
    int listen(bool encrypt);

    // Sets the ftp command. This function can be called only once after each
    // call of listen().
    bool setFtpCommand(FtpCommand *command);

    // Returns the currently running ftpCommand, if it is already running, but
    // not yet finished. Otherwise returns 0.
    FtpCommand *ftpCommand();

signals:

private slots:
    void newConnection();
    void encrypted();
    void connected();

private:
    void startFtpCommand();
    SslServer *server;
    QSslSocket *socket;
    QPointer<FtpCommand> command;
    bool isSocketReady;
    bool isWaitingForFtpCommand;
    bool encrypt;

    // Used for the active data connection (PORT command).
    bool isActiveConnection;
    QString hostName;
    int port;
};

#endif // PASSIVEDATACONNECTION_H
