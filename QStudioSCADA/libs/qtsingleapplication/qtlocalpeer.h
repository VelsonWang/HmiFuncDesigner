
#ifndef QTLOCALPEER_H
#define QTLOCALPEER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QDir>

#include "qtlockedfile.h"

class QtLocalPeer : public QObject
{
    Q_OBJECT

public:
    QtLocalPeer(QObject *parent = 0, const QString &appId = QString());
    bool isClient();
    bool sendMessage(const QString &message, int timeout);
    QString applicationId() const
        { return id; }

signals:
    void messageReceived(const QString &message);

protected Q_SLOTS:
    void receiveConnection();

protected:
    QString id;
    QString socketName;
    QLocalServer* server;
    QtLP_Private::QtLockedFile lockFile;

private:
    static const char* ack;
};

#endif // QTLOCALPEER_H
