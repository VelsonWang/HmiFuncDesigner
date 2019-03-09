#include "ftpretrcommand.h"
#include <QFile>
#include <QSslSocket>

FtpRetrCommand::FtpRetrCommand(QObject *parent, const QString &fileName, qint64 seekTo) :
    FtpCommand(parent)
{
    this->fileName = fileName;
    this->seekTo = seekTo;
    file = 0;
}

FtpRetrCommand::~FtpRetrCommand()
{
    if (started) {
        if (file && file->isOpen() && file->atEnd()) {
            emit reply("226 Closing data connection.");
        } else {
            emit reply("550 Requested action not taken; file unavailable.");
        }
    }
}

void FtpRetrCommand::startImplementation()
{
    file = new QFile(fileName, this);
    if (!file->open(QIODevice::ReadOnly)) {
        deleteLater();
        return;
    }
    emit reply("150 File status okay; about to open data connection.");
    if (seekTo) {
        file->seek(seekTo);
    }

    // For encryted SSL sockets, we need to use the encryptedBytesWritten()
    // signal, see the QSslSocket documentation to for reasons why.
    if (socket->isEncrypted()) {
        connect(socket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(refillSocketBuffer(qint64)));
    } else {
        connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(refillSocketBuffer(qint64)));
    }

    refillSocketBuffer(128*1024);
}

void FtpRetrCommand::refillSocketBuffer(qint64 bytes)
{
    if (!file->atEnd()) {
        socket->write(file->read(bytes));
    } else {
        socket->disconnectFromHost();
    }
}
