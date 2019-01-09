#include "httpserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

HttpServer::HttpServer(QObject *parent)
    : QObject(parent),
      network_(new QNetworkAccessManager(this)),
      server_(new QHttpServer(this))
{
    // connect
    connect(network_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_request_finished(QNetworkReply*)));
    connect(server_, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(on_new_request(QHttpRequest*, QHttpResponse*)));
}

HttpServer::~HttpServer()
{

}

void HttpServer::init(int port)
{
    // start http server
    bool ret = server_->listen(port);
    if (!ret)
    {
        qDebug() << "start local http server failed "
                 << port;
    }
}


void HttpServer::on_request_finished(QNetworkReply* rep)
{
    QString url = rep->url().url();
    QByteArray body = rep->readAll();
    qDebug() << "reply [ "
             << url
             << " ] : "
             << QString(body);

    if (rep->error() != QNetworkReply::NoError)
    {
        qDebug() << "network error: "
                 << rep->errorString();
        return;
    }

    // parse body
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(body, &err);
    if (err.error != QJsonParseError::NoError && doc.isObject())
    {
        qDebug() << "parse reply body error: "
                 << err.errorString()
                 << " "
                 << QString(body);
        return;
    }

}


void HttpServer::on_new_request(QHttpRequest * req, QHttpResponse * rep)
{
    mutex_.lock();
    request_ = req;
    response_ = rep;

    QString path = req->path();
    qDebug() << "new request: "
             << path;

    // check request method
    if (req->method() != QHttpRequest::HTTP_POST)
    {
        rep->writeHead(QHttpResponse::STATUS_METHOD_NOT_ALLOWED);
        rep->end();
        mutex_.unlock();

        qDebug() << "error request method: "
                 << req->methodString();
        return;
    }

    request_body_.clear();
    connect(req, SIGNAL(data(const QByteArray&)),
        this, SLOT(on_request_accumulate(const QByteArray&)));
    connect(req, SIGNAL(end()), this, SLOT(on_request_end()));
}

void HttpServer::on_request_accumulate(const QByteArray & data)
{
    request_body_.append(data);
}

void HttpServer::on_request_end()
{
    QString path = request_->path();
    // parse request body
    QJsonParseError err;
    QJsonObject tmp;
    QJsonDocument doc = QJsonDocument::fromJson(request_body_, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
    {
        response_->writeHead(QHttpResponse::STATUS_BAD_REQUEST);
        response_->end();
        mutex_.unlock();

        qDebug() << "parse request body failed: "
                 << err.errorString();
        return;
    }
    tmp = doc.object();

    int32_t ret = 0;
    QString msg = "";

    if (path == "/user")
    {
        QString user = tmp["User"].toString();
        QString password = tmp["Password"].toString();
        QString email = tmp["Email"].toString();

        msg += QString("User: %1 ").arg(user);
        msg += QString("Password: %1 ").arg(password);
        msg += QString("Email: %1 ").arg(email);
        qDebug() << msg;
    }

    // response
    QString rep_buf = QString("{'retCode':'%1', 'retDesc':'%2'}").arg(ret).arg(msg);
    response_->writeHead(QHttpResponse::STATUS_OK);
    response_->end(rep_buf.toUtf8());
    qDebug() << "response : "
             << rep_buf;

    mutex_.unlock();
    return;
}


