﻿#include "httpserver.h"
#include "realtimedb.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
    if (!ret) {
        qDebug() << "start local http server failed " << port;
    }
}


void HttpServer::on_request_finished(QNetworkReply* rep)
{
    QString url = rep->url().url();
    QByteArray body = rep->readAll();
    qDebug() << "reply [ " << url << " ] : " << QString(body);

    if (rep->error() != QNetworkReply::NoError) {
        qDebug() << "network error: " << rep->errorString();
        return;
    }

    // parse body
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(body, &err);
    if (err.error != QJsonParseError::NoError && doc.isObject()) {
        qDebug() << "parse reply body error: " << err.errorString() << " " << QString(body);
        return;
    }

}


void HttpServer::on_new_request(QHttpRequest * req, QHttpResponse * rep)
{
    mutex_.lock();
    request_ = req;
    response_ = rep;

    QString path = req->path();
    //qDebug() << "new request: " << path;

    // check request method
    if (req->method() != QHttpRequest::HTTP_POST) {
        rep->writeHead(QHttpResponse::STATUS_METHOD_NOT_ALLOWED);
        rep->end();
        mutex_.unlock();

        qDebug() << "error request method: " << req->methodString();
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
    QJsonDocument doc = QJsonDocument::fromJson(request_body_, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        response_->writeHead(QHttpResponse::STATUS_BAD_REQUEST);
        response_->end();
        mutex_.unlock();

        qDebug() << "parse request body failed: " << err.errorString();
        return;
    }

    QJsonObject retJsonObj;

    if (path == "/READ") {
        const QJsonObject jsonDocObj = doc.object();
        QString szStartID = jsonDocObj["StartID"].toString();
        int number = jsonDocObj["Number"].toInt();
        //qDebug() << QString("StartID: %1 ").arg(szStartID) << QString(" Number: %1 ").arg(number);

        retJsonObj["StartID"] = szStartID;
        retJsonObj["Number"] = number;
        retJsonObj["Status"] = "OK";

        QJsonArray tagArray;

        int iPos = -1;
        QString szTagIDTmp = "";
        QString szTmp = "0";

        szTagIDTmp = szStartID;
        iPos = szTagIDTmp.lastIndexOf(".");
        szTmp = szTagIDTmp.right(szTagIDTmp.length() - iPos - 1);
        int id = szTmp.toInt();

        for(int i = 0; i < number; i++) {
            QJsonObject jsonObj;
            //qDebug() << szTagID << RealTimeDB::GetDataString(szTagID);
            QString szTagID =  QString("%1%2").arg(szStartID.left(iPos + 1)).arg(QString::number(id));
            //jsonObj[szTagID] = RealTimeDB::instance()->GetDataString(szTagID);
            tagArray.append(jsonObj);
            id++;
        }
        retJsonObj["TagArray"] = tagArray;
    } else if (path == "/WRITE") {
        const QJsonObject jsonDocObj = doc.object();
        QString szStartID = jsonDocObj["StartID"].toString();
        int number = jsonDocObj["Number"].toInt();

        int iPos = -1;
        QString szTagIDTmp = "";
        QString szTmp = "0";

        szTagIDTmp = szStartID;
        iPos = szTagIDTmp.lastIndexOf(".");
        szTmp = szTagIDTmp.right(szTagIDTmp.length() - iPos - 1);
        int id = szTmp.toInt();

        QJsonArray tagArray = jsonDocObj["TagArray"].toArray();
        for (int i = 0; i < tagArray.size(); ++i) {
            QJsonObject jsonObj = tagArray[i].toObject();
            QString szTagID =  QString("%1%2").arg(szStartID.left(iPos + 1)).arg(QString::number(id));
            //RealTimeDB::instance()->SetDataString(szTagID, jsonObj[szTagID].toString());
            id++;
        }
        retJsonObj["StartID"] = szStartID;
        retJsonObj["Number"] = number;
        retJsonObj["Status"] = "OK";
    }

    QJsonDocument retJsonDoc;
    retJsonDoc.setObject(retJsonObj);
    response_->writeHead(QHttpResponse::STATUS_OK);
    response_->end(retJsonDoc.toJson(QJsonDocument::Compact));

    mutex_.unlock();
    return;
}


