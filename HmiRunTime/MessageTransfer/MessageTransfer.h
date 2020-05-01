
#ifndef MessageTransfer_H
#define MessageTransfer_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>


class MessageTransfer : public QObject
{
    Q_OBJECT

public:
    MessageTransfer(bool bServer = false, const QString szServerName = "HmiRunTime.MessageService");
    ~MessageTransfer();

    bool sendMessage(const QString &message, int timeout);


protected Q_SLOTS:
    void receiveConnection();

signals:
    void messageReceived(const QString &message);

private:
    void sysInit(const QString &appId = QString());

protected:
    QString socketName;
    QLocalServer* server;

private:
    static const char* ack;
};


#endif // MessageTransfer_H
