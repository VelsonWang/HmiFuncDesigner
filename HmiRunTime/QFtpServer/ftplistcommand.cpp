#include "ftplistcommand.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTimer>
#include <QSslSocket>

FtpListCommand::FtpListCommand(QObject *parent, const QString &fileName, bool nameListOnly) :
    FtpCommand(parent)
{
    this->listDirectory = fileName;
    this->nameListOnly = nameListOnly;
}

FtpListCommand::~FtpListCommand()
{
    if (started) {
        emit reply("226 Closing data connection.");
    }
}

void FtpListCommand::startImplementation()
{
    QFileInfo info(listDirectory);

    if (!info.isReadable()) {
        emit reply("425 File or directory is not readable or doesn't exist.");
        socket->disconnectFromHost();
        return;
    }

    emit reply("150 File status okay; about to open data connection.");

    index = 0;
    list = new QFileInfoList;
    if (!info.isDir()) {
        *list = (QFileInfoList() << info);
    } else {
        *list = QDir(listDirectory).entryInfoList();
    }

    // Start the timer.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(listNextBatch()));
    timer->start(0);
}

QString padded(QString s, int n)
{
    while (s.size() < n) {
        s = ' ' + s;
    }
    return s;
}

QString FtpListCommand::fileListingString(const QFileInfo &fi)
{
    // This is how the returned list looks. It is like what is returned by
    // 'ls -l':
    // drwxr-xr-x    9 ftp      ftp          4096 Nov 17  2009 pub

    QString line;
    if (!nameListOnly) {
        // Directory/symlink/file.
        if (fi.isSymLink()) {
            line += 'l';
        } else if (fi.isDir()) {
            line += 'd';
        } else {
            line += '-';
        }

        // Permissions.
        QFile::Permissions p = fi.permissions();
        line += (p & QFile::ReadOwner) ? 'r' : '-';
        line += (p & QFile::WriteOwner) ? 'w' : '-';
        line += (p & QFile::ExeOwner) ? 'x' : '-';
        line += (p & QFile::ReadGroup) ? 'r' : '-';
        line += (p & QFile::WriteGroup) ? 'w' : '-';
        line += (p & QFile::ExeGroup) ? 'x' : '-';
        line += (p & QFile::ReadOther) ? 'r' : '-';
        line += (p & QFile::WriteOther) ? 'w' : '-';
        line += (p & QFile::ExeOther) ? 'x' : '-';

        // Owner/group.
        QString owner = fi.owner();
        if (owner.isEmpty()) {
            owner = "unknown";
        }
        QString group = fi.group();
        if (group.isEmpty()) {
            group = "unknown";
        }
        line += ' ' + padded(owner, 10) + ' ' + padded(group, 10);

        // File size.
        line += ' ' + padded(QString::number(fi.size()), 14);

        // Last modified - note we **must** use english locale, otherwise FTP clients won't understand us.
        QLocale locale(QLocale::English);
        QDateTime lm = fi.lastModified();
        if (lm.date().year() != QDate::currentDate().year()) {
            line += ' ' + locale.toString(lm.date(), "MMM dd  yyyy");
        } else {
            line += ' ' + locale.toString(lm.date(), "MMM dd") + ' ' + locale.toString(lm.time(), "hh:mm");
        }
    }
    line += ' ' + fi.fileName();
    line += "\r\n";
    return line;
}

void FtpListCommand::listNextBatch()
{
    // List next 10 items.
    int stop = qMin(index + 10, list->size());
    while (index < stop) {
        QString line = fileListingString(list->at(index));
        socket->write(line.toUtf8());
        index++;
    }

    // If all files have been listed, then finish.
    if (list->size() == stop) {
        delete list;
        timer->stop();
        socket->disconnectFromHost();
    }
}
