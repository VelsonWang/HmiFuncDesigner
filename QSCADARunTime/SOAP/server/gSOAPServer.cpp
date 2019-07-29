#include "gSOAPServer.h"
#include "../public/soap.nsmap"
#include "Log.h"
#include "RealTimeDB.h"
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


SOAPServerWorker::SOAPServerWorker()
{
    bRunningFlag_ = true;
}

SOAPServerWorker::~SOAPServerWorker()
{
    bRunningFlag_ = false;
}

void SOAPServerWorker::doService(const QString &ip, int port)
{
    soap recieveSoap;
    soap_init(&recieveSoap);

    LogInfo("ip:", ip, " port:", port);
    LogInfo("Soap bind return:", soap_bind(&recieveSoap, ip.toStdString().c_str(), port, 100));

    while(bRunningFlag_)
    {
        int s = soap_accept(&recieveSoap);
        if(s < 0)
        {
            soap_print_fault(&recieveSoap, stderr);
            exit(-1);
        }
        LogInfo("Socket connection successful: client socket = ", s);

        soap_serve(&recieveSoap); //该句说明该server的服务
        soap_end(&recieveSoap);
    }
}


void SOAPServerWorker::doExitService()
{
    bRunningFlag_ = false;
}



SOAPServer::SOAPServer(const QString &ip, int port)
{
    SOAPServerWorker *worker = new SOAPServerWorker();
    worker->moveToThread(&workerThread_);
    connect(&workerThread_, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SOAPServer::doService, worker, &SOAPServerWorker::doService);
    connect(this, &SOAPServer::doExitService, worker, &SOAPServerWorker::doExitService);
    emit doService(ip, port);
    workerThread_.start();
}


SOAPServer::~SOAPServer()
{
    workerThread_.quit();
    workerThread_.wait();
}


void SOAPServer::exitService()
{
    emit doExitService();
}

/**
 * @brief readTags
 * @details 读变量
 * @param parseJson
 * @param retJson
 */
void readTags(QJsonObject parseJson, QJsonObject &retJson)
{
    QJsonArray parameters = parseJson["Parameters"].toArray();
    if(parameters.size() > 0) {
        QJsonObject jsonObj = parameters[0].toObject();
        QString szStartID = jsonObj["StartID"].toString();
        int iNumber = jsonObj["Number"].toInt();

        retJson["Code"] = 1001;
        QJsonArray retParameters;

        int iPos = -1;
        QString szTagIDTmp = "";
        QString szTmp = "0";

        szTagIDTmp = szStartID;
        iPos = szTagIDTmp.lastIndexOf(".");
        szTmp = szTagIDTmp.right(szTagIDTmp.length() - iPos - 1);
        int id = szTmp.toInt();

        for(int i=0; i<iNumber; i++) {
            QJsonObject jsonObj;
            QString szTagID =  QString("%1%2").arg(szStartID.left(iPos + 1)).arg(QString::number(id));
            jsonObj[szTagID] = RealTimeDB::GetDataString(szTagID);
            retParameters.append(jsonObj);
            id++;
        }
        retJson["Parameters"] = retParameters;
        return;
    }

    retJson = QJsonObject();
}


/**
 * @brief writeTags
 * @details 写变量
 * @param parseJson
 * @param retJson
 */
void writeTags(QJsonObject parseJson, QJsonObject &retJson)
{
    QJsonArray parameters = parseJson["Parameters"].toArray();
    for (int i = 0; i < parameters.size(); ++i) {
        QJsonObject jsonObj = parameters[i].toObject();
        foreach(QString key , jsonObj.keys()) {
            QString szVal = jsonObj[key].toString();
            //qDebug() << key << " = " << szVal;
            RealTimeDB::SetDataString(key, szVal);
        }
    }

    retJson["Code"] = 1002;
    QJsonArray retParameters;
    retJson["Parameters"] = retParameters;
}



int dataStream(soap *, std::string szSendMsg, std::string &szRetMsg)
{
    QByteArray msgDatas;
    msgDatas.clear();
    msgDatas.append(QString(szSendMsg.c_str()));

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(msgDatas, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
    {
        LogError("parse message failed: ", err.errorString());
        return -1;
    }

    QJsonObject retJsonObj;
    const QJsonObject jsonDocObj = doc.object();
    int iCode = jsonDocObj["Code"].toInt();

    switch (iCode) {
        case 1001: { // 读变量
            readTags(jsonDocObj, retJsonObj);
        }break;
        case 1002: { // 写变量
            writeTags(jsonDocObj, retJsonObj);
        }break;
    }

    QJsonDocument retJsonDoc;
    retJsonDoc.setObject(retJsonObj);
    szRetMsg = QString(retJsonDoc.toJson(QJsonDocument::Compact)).toStdString();

    return 0;
}
