#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>

#include "libhttp/qhttpserver.h"
#include "libhttp/qhttprequest.h"
#include "libhttp/qhttpresponse.h"

class HttpServer : public QObject
{
    Q_OBJECT

public:
    HttpServer(QObject *parent = NULL);
    ~HttpServer();
    void init(int port);

protected slots:
    void on_request_finished(QNetworkReply* rep);
    void on_new_request(QHttpRequest * req, QHttpResponse * rep);
    void on_request_accumulate(const QByteArray& data);
    void on_request_end();

private:
    QNetworkAccessManager* network_;
    QHttpServer*		server_;
    QHttpRequest*		request_;
    QHttpResponse*		response_;
    QByteArray			request_body_;
    QMutex				mutex_;
};

#endif // HTTPSERVER_H
