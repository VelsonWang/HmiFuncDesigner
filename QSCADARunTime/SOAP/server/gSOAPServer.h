#ifndef SOAP_SERVER_H
#define SOAP_SERVER_H

#include "../public/soapStub.h"
#include "../public/soapH.h"
#include <QtCore>


class SOAPServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit SOAPServerWorker();
    ~SOAPServerWorker();

public slots:
    void doService(const QString &ip, int port);
    void doExitService();

private:
    bool bRunningFlag_ = true;
};

class SOAPServer : public QObject
{
    Q_OBJECT

public:
    SOAPServer(const QString &ip, int port);
    ~SOAPServer();
    void exitService();

signals:
    void doService(const QString &ip, int port);
    void doExitService();

private:
    QThread workerThread_;
};

extern int dataStream(soap *, std::string szSendMsg, std::string &szRetMsg);

#endif // SOAP_SERVER_H
