#ifndef SCADARUNTIME_H
#define SCADARUNTIME_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include "IVendor.h"
#include "RealTimeDB.h"
#include "PortThread.h"
#include "RunScript.h"

class SCADARunTime : public QObject
{
    Q_OBJECT
public:
    explicit SCADARunTime(QString projectPath, QObject *parent = 0);
    ~SCADARunTime();
    bool Load(SaveFormat saveFormat);
    bool Unload();
    void Start();
    void Stop();
    void AddPortName(const QString name);
    IVendor *FindVendor(const QString name);
    QJsonObject LoadJsonObjectFromFile(SaveFormat saveFormat, QString f);
    // 发送消息到消息服务
    static void doMessage(QString msg);

signals:

public slots:

private:
    static QString m_sProjectPath;
    QStringList m_listPortName;
    QList<IVendor *> m_VendorList;
    QList<PortThread *> m_listPortThread;
    static RunScript *m_pRunScript;
};

#endif // SCADARUNTIME_H
