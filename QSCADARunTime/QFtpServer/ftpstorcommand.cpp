#include "ftpstorcommand.h"
#include <QFile>
#include <QSslSocket>

FtpStorCommand::FtpStorCommand(QObject *parent, const QString &fileName, bool appendMode, qint64 seekTo) :
    FtpCommand(parent)
{
    this->fileName = fileName;
    this->appendMode = appendMode;
    file = 0;
    this->seekTo = seekTo;
    success = false;
}

FtpStorCommand::~FtpStorCommand()
{
    if (started) {
        if (success) {
            emit reply("226 Closing data connection.");
        } else {
            emit reply("451 Requested action aborted: local error in processing.");
        }
    }
}

void FtpStorCommand::startImplementation()
{
    file = new QFile(fileName, this);
    if (!file->open(appendMode ? QIODevice::Append : QIODevice::WriteOnly)) {
        deleteLater();
        return;
    }
    success = true;
    emit reply("150 File status okay; about to open data connection.");
    if (seekTo) {
        file->seek(seekTo);
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(acceptNextBlock()));
}

void FtpStorCommand::acceptNextBlock()
{
    const QByteArray &bytes = socket->readAll();
    int bytesWritten = file->write(bytes);
    if (bytesWritten != bytes.size()) {
        emit reply("451 Requested action aborted. Could not write data to file.");
        deleteLater();
    }
}
