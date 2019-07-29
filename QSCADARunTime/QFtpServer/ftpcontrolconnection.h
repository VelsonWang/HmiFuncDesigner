#ifndef FTPCONTROLCONNECTION_H
#define FTPCONTROLCONNECTION_H

#include <QObject>
#include <QPointer>

class QSslSocket;
class FtpCommand;
class DataConnection;

// Implements the ftp control connection. Reads the ftp commands from the
// control connection socket, parses each line and maps it to an implemented
// command. All of the ftp commands except the ones that require a data
// connection are implemented here.

class FtpControlConnection : public QObject
{
    Q_OBJECT
public:
    explicit FtpControlConnection(QObject *parent, QSslSocket *socket, const QString &rootPath, const QString &userName = QString(), const QString &password = QString(), bool readOnly = false);
    ~FtpControlConnection();

signals:

public slots:
    // Slot used by the data connection handlers to send messages to the FTP
    // client through the control connection.
    void reply(const QString &replyCode);

private slots:
    // Handles any new data comming from the FTP client.
    void acceptNewData();
    // We need this because QSslSocket::disconnectFromHost() is not a slot.
    void disconnectFromHost();

private:
    // Verify authentication for commands that require it. Returns true
    // if user logged in or if command doesn't require it.
    bool verifyAuthentication(const QString &command);
    // Verify read-only mode. Returns true if writing is enabled, or if
    // the command doesn't require it.
    bool verifyWritePermission(const QString &command);
    // Workaround for some clients that will send "LIST -L" or "NLST -L", which
    // causes our server to take "-L" for a filename, and fail to produce a
    // listing. See http://stackoverflow.com/q/21065113/492336. This function
    // strips that flag.
    QString stripFlagL(const QString &fileName);
    // Each FTP command can have optional arguments. This function splits the
    // command name from the command arguments. This function splits the
    // command and arguments, and makes the command uppercase. For example, in
    // "cd /path/to/dir", "CD" is the command, and "/path/to/dir" are the
    // parameters.
    void parseCommand(const QString &entireCommand, QString *command, QString *commandParameters);
    // Converts a relative or absolute path given by the FTP client to a
    // local path (local for the OS on which the ftp server is running).
    QString toLocalPath(const QString &fileName) const;
    // Parses and executes a single command (for example "CD /path/to/dir").
    void processCommand(const QString &entireCommand);
    void startOrScheduleCommand(FtpCommand *ftpCommand);
    // Open a new active data connection.
    void port(const QString &addressAndPort);
    // Open a new passive data connection.
    void pasv();
    // List directory contents. Equivalent to 'ls' in UNIX, or 'dir' in DOS.
    void list(const QString &dir, bool nameListOnly);
    // Retrieve a file. FTP client uses this command to download files.
    void retr(const QString &fileName);
    // Store a file. FTP client uses this command to upload files.
    void stor(const QString &fileName, bool appendMode = false);
    // Change current directory, i.e. CD.
    void cwd(const QString &dir);
    // Create a directory.
    void mkd(const QString &dir);
    // Delete a directory (fails if directory not empty).
    void rmd(const QString &dir);
    // Delete a file.
    void dele(const QString &fileName);
    // Rename a directory or file.
    void rnto(const QString &fileName);
    // Quits the FTP session. The control connection closes.
    void quit();
    // Returns the size of the file.
    void size(const QString &fileName);
    // Enters the password.
    void pass(const QString &password);
    // The client instructs the server to switch to FTPS.
    void auth();
    // Set protection level.
    void prot(const QString &protectionLevel);
    // CD up one level - equivalent to "CD .."
    void cdup();
    // Server reports which features it supports.
    void feat();
    // If the previous command was REST, returns its value. The REST command
    // allows to the client to continue partially downloaded/uploaded files.
    qint64 seekTo();
    // The TCP (or SSL) socket of the control connection.
    QSslSocket *socket;
    // The current directory (i.e. CD) of the FTP server. Used for relative
    // paths.
    QString currentDirectory;
    // Some commands depend on the previous command (for example RNFR/RNTO,
    // REST/STOR, etc.).
    QString lastProcessedCommand;
    // Flag whether the FTP client has logged (provided a username/password) or not yet.
    bool isLoggedIn;
    // The username that the FTP server expects.
    QString userName;
    // The password that the FTP server expects.
    QString password;
    QString rootPath;
    // Flag for whether we should encrypt data connections.
    bool encryptDataConnection;
    DataConnection *dataConnection;
    // Flag whether the client is allowed only read-only access (can download,
    // but not upload/modify).
    bool readOnly;
};

#endif // FTPCONTROLCONNECTION_H
