// Qt lib import
#include <QCoreApplication>

// gSoap lib import
#include "client.h"
#include <string.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#if 0
Send:  {"Code":1002,"Parameters":[{"io.group3.1":"1"},{"io.group3.2":"2"},{"io.group3.3":"3"},{"io.group3.4":"4"},{"io.group3.5":"5"},{"io.group3.6":"6"},{"io.group3.7":"7"},{"io.group3.8":"8"}]}
Read:  {"Code":1002,"Parameters":[]}
#endif
void writeTags()
{
    QJsonObject json;
    json["Code"] = 1002;
    QJsonArray parameters;
    for(int i=1; i<9; i++) {
        QJsonObject jsonParam;
        QString id = QString("io.group3.%1").arg(QString::number(i));
        jsonParam[id] = QString::number(i);
        parameters.append(jsonParam);
    }
    json["Parameters"] = parameters;

    QJsonDocument document;
    document.setObject(json);
    QByteArray sendDatas = document.toJson(QJsonDocument::Compact);

    qDebug() << "Send: " << QString(sendDatas).toStdString().c_str();

    std::string returnFlag;
    qDebug() << sendDatStream("127.0.0.1:60002", QString(sendDatas).toStdString().c_str(), returnFlag);
    qDebug() << "Read: " << returnFlag.c_str();

    QByteArray retDatas;
    retDatas.clear();
    retDatas.append(QString(returnFlag.c_str()));

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(retDatas, &err);
    if (err.error == QJsonParseError::NoError && doc.isObject())
    {
        const QJsonObject jsonDocObj = doc.object();
        int iCode = jsonDocObj["Code"].toInt();

        if(iCode == 1002) {

        }
    }

}

#if 0

Send:  {"Code":1001,"Parameters":[{"Number":6,"StartID":"sys.1"}]}
Read:  {"Code":1001,"Parameters":[{"sys.1":"2019"},{"sys.2":"7"},{"sys.3":"25"},{"sys.4":"11"},{"sys.5":"35"},{"sys.6":"36"}]}

#endif

void readTags()
{
    QJsonObject json;
    json["Code"] = 1001;
    QJsonArray parameters;
    QJsonObject jsonParam;
    jsonParam["StartID"] = "sys.1";
    jsonParam["Number"] = 6;
    parameters.append(jsonParam);
    json["Parameters"] = parameters;

    QJsonDocument document;
    document.setObject(json);
    QByteArray sendDatas = document.toJson(QJsonDocument::Compact);

    qDebug() << "Send: " << QString(sendDatas).toStdString().c_str();

    std::string returnFlag;
    qDebug() << sendDatStream("127.0.0.1:60002", QString(sendDatas).toStdString().c_str(), returnFlag);
    qDebug() << "Read: " << returnFlag.c_str();

    QByteArray retDatas;
    retDatas.clear();
    retDatas.append(QString(returnFlag.c_str()));

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(retDatas, &err);
    if (err.error == QJsonParseError::NoError && doc.isObject())
    {
        const QJsonObject jsonDocObj = doc.object();
        int iCode = jsonDocObj["Code"].toInt();

        if(iCode == 1001) {
            QJsonArray parameters = jsonDocObj["Parameters"].toArray();
            for (int i = 0; i < parameters.size(); ++i) {
                QJsonObject jsonObj = parameters[i].toObject();
                foreach(QString key , jsonObj.keys()) {
                    QString szVal = jsonObj[key].toString();
                    qDebug() << key << " = " << szVal;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    writeTags();
    readTags();

    return 0; // return a.exec();
}
