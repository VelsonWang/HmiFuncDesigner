#include "ftpcontrolconnection.h"
#include "ftplistcommand.h"
#include "ftpretrcommand.h"
#include "ftpstorcommand.h"
#include "sslserver.h"
#include "dataconnection.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDir>
#include <QStringList>
#include <QDir>
#include <QEventLoop>
#include <QDebug>
#include <QTimer>
#include <QSslSocket>

FtpControlConnection::FtpControlConnection(QObject *parent, QSslSocket *socket, const QString &rootPath, const QString &userName, const QString &password, bool readOnly) :
    QObject(parent)
{
    this->socket = socket;
    this->userName = userName;
    this->password = password;
    this->rootPath = rootPath;
    this->readOnly = readOnly;
    isLoggedIn = false;
    encryptDataConnection = false;
    socket->setParent(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(acceptNewData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    currentDirectory = "/";
    dataConnection = new DataConnection(this);
    reply("220 Welcome to QFtpServer.");
}

FtpControlConnection::~FtpControlConnection()
{
}

void FtpControlConnection::acceptNewData()
{
    if (!socket->canReadLine()) {
        return;
    }

    // Note how we execute only one line, and use QTimer::singleShot, instead
    // of using a for-loop until no more lines are available. This is done
    // so we don't block the event loop for a long time.
    processCommand(QString::fromUtf8(socket->readLine()).trimmed());
    QTimer::singleShot(0, this, SLOT(acceptNewData()));
}

void FtpControlConnection::disconnectFromHost()
{
    socket->disconnectFromHost();
}

bool FtpControlConnection::verifyAuthentication(const QString &command)
{
    if (isLoggedIn) {
        return true;
    }

    const char *commandsRequiringAuth[] = {
        "PWD", "CWD", "TYPE", "PORT", "PASV", "LIST", "RETR", "REST",
        "NLST", "SIZE", "SYST", "PROT", "CDUP", "OPTS", "PBSZ", "NOOP",
        "STOR", "MKD", "RMD", "DELE", "RNFR", "RNTO", "APPE"
    };

    for (size_t ii = 0; ii < sizeof(commandsRequiringAuth)/sizeof(commandsRequiringAuth[0]); ++ii) {
        if (command == commandsRequiringAuth[ii]) {
            reply("530 You must log in first.");
            return false;
        }
    }

    return true;
}

bool FtpControlConnection::verifyWritePermission(const QString &command)
{
    if (!readOnly) {
        return true;
    }

    const char *commandsRequiringWritePermission[] = {
        "STOR", "MKD", "RMD", "DELE", "RNFR", "RNTO", "APPE"
    };

    for (size_t ii = 0; ii < sizeof(commandsRequiringWritePermission)/sizeof(commandsRequiringWritePermission[0]); ++ii) {
        if (command == commandsRequiringWritePermission[ii]) {
            reply("550 Can't do that in read-only mode.");
            return false;
        }
    }

    return true;
}

QString FtpControlConnection::stripFlagL(const QString &fileName)
{
    QString a = fileName.toUpper();
    if (a == "-L") {
        return "";
    }
    if (a.startsWith("-L ")) {
        return fileName.mid(3);
    }
    return fileName;
}

void FtpControlConnection::parseCommand(const QString &entireCommand, QString *command, QString *commandParameters)
{
    // Split parameters and command.
    int pos = entireCommand.indexOf(' ');
    if (-1 != pos) {
        *command = entireCommand.left(pos).trimmed().toUpper();
        *commandParameters = entireCommand.mid(pos+1).trimmed();
    } else {
        *command = entireCommand.trimmed().toUpper();
    }
}

QString FtpControlConnection::toLocalPath(const QString &fileName) const
{
    QString localPath = fileName;

    // Some FTP clients send backslashes.
    localPath.replace('\\', '/');

    // If this is a relative path, we prepend the current directory.
    if (!localPath.startsWith('/')) {
        localPath = currentDirectory + '/' + localPath;
    }

    // Evaluate all the ".." and ".", "/path/././to/dir/../.." becomes "/path".
    // Note we do this **before** prepending the root path, in order to avoid
    // "jailbreaking" out of the "chroot".
    QStringList components;
    foreach (const QString &component, localPath.split('/', QString::SkipEmptyParts)) {
        if (component == "..") {
            if (!components.isEmpty()) {
                components.pop_back();
            }
        } else if (component != ".") {
            components += component;
        }
    }

    // Prepend the root path.
    localPath = QDir::cleanPath(rootPath + '/' + components.join("/"));

    qDebug() << "to local path" << fileName << "->" << localPath;
    return localPath;
}

void FtpControlConnection::reply(const QString &replyCode)
{
    qDebug() << "reply" << replyCode;
    socket->write((replyCode + "\r\n").toUtf8());
}

void FtpControlConnection::processCommand(const QString &entireCommand)
{
    qDebug() << "command" << entireCommand;

    QString command;
    QString commandParameters;
    parseCommand(entireCommand, &command, &commandParameters);

    if (!verifyAuthentication(command)) {
        return;
    }

    if (!verifyWritePermission(command)) {
        return;
    }

    if ("USER" == command) {
        reply("331 User name OK, need password.");
    } else if ("PASS" == command) {
        pass(commandParameters);
    } else if ("QUIT" == command) {
        quit();
    } else if ("AUTH" == command && "TLS" == commandParameters.toUpper()) {
        auth();
    } else if ("FEAT" == command) {
        feat();
    } else if ("PWD" == command) {
        reply(QString("257 \"%1\"").arg(currentDirectory));
    } else if ("CWD" == command) {
        cwd(commandParameters);
    } else if ("TYPE" == command) {
        reply("200 Command okay.");
    } else if ("PORT" == command) {
        port(commandParameters);
    } else if ("PASV" == command) {
        pasv();
    } else if ("LIST" == command) {
        list(toLocalPath(stripFlagL(commandParameters)), false);
    } else if ("RETR" == command) {
        retr(toLocalPath(commandParameters));
    } else if ("REST" == command) {
        reply("350 Requested file action pending further information.");
    } else if ("NLST" == command) {
        list(toLocalPath(stripFlagL(commandParameters)), true);
    } else if ("SIZE" == command) {
        size(toLocalPath(commandParameters));
    } else if ("SYST" == command) {
        reply("215 UNIX");
    } else if ("PROT" == command) {
        prot(commandParameters.toUpper());
    } else if ("CDUP" == command) {
        cdup();
    } else if ("OPTS" == command && "UTF8 ON" == commandParameters.toUpper()) {
        reply("200 Command okay.");
    } else if ("PBSZ" == command && "0" == commandParameters.toUpper()) {
        reply("200 Command okay.");
    } else if ("NOOP" == command) {
        reply("200 Command okay.");
    } else if ("STOR" == command) {
        stor(toLocalPath(commandParameters));
    } else if ("MKD" == command) {
        mkd(toLocalPath(commandParameters));
    } else if ("RMD" == command) {
        rmd(toLocalPath(commandParameters));
    } else if ("DELE" == command) {
        dele(toLocalPath(commandParameters));
    } else if ("RNFR" == command) {
        reply("350 Requested file action pending further information.");
    } else if ("RNTO" == command) {
        rnto(toLocalPath(commandParameters));
    } else if ("APPE" == command) {
        stor(toLocalPath(commandParameters), true);
    } else {
        reply("502 Command not implemented.");
    }

    lastProcessedCommand = entireCommand;
}

void FtpControlConnection::startOrScheduleCommand(FtpCommand *ftpCommand)
{
    connect(ftpCommand, SIGNAL(reply(QString)), this, SLOT(reply(QString)));

    if (!dataConnection->setFtpCommand(ftpCommand)) {
        delete ftpCommand;
        reply("425 Can't open data connection.");
        return;
    }
}

void FtpControlConnection::port(const QString &addressAndPort)
{
    // Example PORT command:
    // PORT h1,h2,h3,h4,p1,p2

    // Get IP and port.
    QRegExp exp("\\s*(\\d+,\\d+,\\d+,\\d+),(\\d+),(\\d+)");
    exp.indexIn(addressAndPort);
    QString hostName = exp.cap(1).replace(',', '.');
    int port = exp.cap(2).toInt() * 256 + exp.cap(3).toInt();
    dataConnection->scheduleConnectToHost(hostName, port, encryptDataConnection);
    reply("200 Command okay.");
}

void FtpControlConnection::pasv()
{
    int port = dataConnection->listen(encryptDataConnection);
    reply(QString("227 Entering Passive Mode (%1,%2,%3).").arg(socket->localAddress().toString().replace('.',',')).arg(port/256).arg(port%256));
}

void FtpControlConnection::list(const QString &dir, bool nameListOnly)
{
    startOrScheduleCommand(new FtpListCommand(this, dir, nameListOnly));
}

void FtpControlConnection::retr(const QString &fileName)
{
    startOrScheduleCommand(new FtpRetrCommand(this, fileName, seekTo()));
}

void FtpControlConnection::stor(const QString &fileName, bool appendMode)
{
    startOrScheduleCommand(new FtpStorCommand(this, fileName, appendMode, seekTo()));
}

void FtpControlConnection::cwd(const QString &dir)
{
    QFileInfo fi(toLocalPath(dir));
    if (fi.exists() && fi.isDir()) {
        QFileInfo fi(dir);
        if (fi.isAbsolute()) {
            currentDirectory = QDir::cleanPath(dir);
        } else {
            currentDirectory = QDir::cleanPath(currentDirectory + '/' + dir);
        }
        reply("250 Requested file action okay, completed.");
    } else {
        reply("550 Requested action not taken; file unavailable.");
    }
}

void FtpControlConnection::mkd(const QString &dir)
{
    if (QDir().mkdir(dir)) {
        reply(QString("257 \"%1\" created.").arg(dir));
    } else {
        reply("550 Requested action not taken; file unavailable.");
    }
}

void FtpControlConnection::rmd(const QString &dir)
{
    if (QDir().rmdir(dir)) {
        reply("250 Requested file action okay, completed.");
    } else {
        reply("550 Requested action not taken; file unavailable.");
    }
}

void FtpControlConnection::dele(const QString &fileName)
{
    if (QDir().remove(fileName)) {
        reply("250 Requested file action okay, completed.");
    } else {
        reply("550 Requested action not taken; file unavailable.");
    }
}

void FtpControlConnection::rnto(const QString &fileName)
{
    QString command;
    QString commandParameters;
    parseCommand(lastProcessedCommand, &command, &commandParameters);
    if ("RNFR" == command && QDir().rename(toLocalPath(commandParameters), fileName)) {
        reply("250 Requested file action okay, completed.");
    } else {
        reply("550 Requested action not taken; file unavailable.");
    }
}

void FtpControlConnection::quit()
{
    reply("221 Quitting...");
    // If we have a running download or upload, we will wait until it's
    // finished before closing the control connection.
    if (dataConnection->ftpCommand()) {
        connect(dataConnection->ftpCommand(), SIGNAL(destroyed()), this, SLOT(disconnectFromHost()));
    } else {
        disconnectFromHost();
    }
}

void FtpControlConnection::size(const QString &fileName)
{
    QFileInfo fi(fileName);
    if (!fi.exists() || fi.isDir()) {
        reply("550 Requested action not taken; file unavailable.");
    } else {
        reply(QString("213 %1").arg(fi.size()));
    }
}

void FtpControlConnection::pass(const QString &password)
{
    QString command;
    QString commandParameters;
    parseCommand(lastProcessedCommand, &command, &commandParameters);
    if (this->password.isEmpty() || ("USER" == command && this->userName == commandParameters && this->password == password)) {
        reply("230 You are logged in.");
        isLoggedIn = true;
    } else {
        reply("530 User name or password was incorrect.");
    }
}

void FtpControlConnection::auth()
{
    reply("234 Initializing SSL connection.");
    SslServer::setLocalCertificateAndPrivateKey(socket);
    socket->startServerEncryption();
}

void FtpControlConnection::prot(const QString &protectionLevel)
{
    if ("C" == protectionLevel) {
        encryptDataConnection = false;
    } else if ("P" == protectionLevel) {
        encryptDataConnection = true;
    } else {
        reply("502 Command not implemented.");
        return;
    }
    reply("200 Command okay.");
}

void FtpControlConnection::cdup()
{
    if ("/" == currentDirectory) {
        reply("250 Requested file action okay, completed.");
    } else {
        cwd("..");
    }
}

void FtpControlConnection::feat()
{
    // We only report that we support UTF8 file names, this is needed because
    // some clients will assume that we use ASCII otherwise, and will not
    // encode the filenames properly.
    socket->write(
        "211-Features:\r\n"
        " UTF8\r\n"
        "211 End\r\n"
    );
}

qint64 FtpControlConnection::seekTo()
{
    qint64 seekTo = 0;
    QString command;
    QString commandParameters;
    parseCommand(lastProcessedCommand, &command, &commandParameters);
    if ("REST" == command) {
        QTextStream(commandParameters.toUtf8()) >> seekTo;
    }
    return seekTo;
}
