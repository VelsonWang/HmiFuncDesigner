#ifndef HMIRUNTIME_H
#define HMIRUNTIME_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QMutex>
#include "Vendor.h"
#include "RealTimeDB.h"
#include "PortThread.h"
//#include "RunScript.h"
#include "qprojectcore.h"

class HmiRunTime : public QObject
{
    Q_OBJECT
public:
    explicit HmiRunTime(QProjectCore *coreObj, QObject *parent = NULL);
    ~HmiRunTime();
    bool Load();
    bool Unload();
    void Start();
    void Stop();
    void AddPortName(const QString name);
    Vendor *FindVendor(const QString name);
    QJsonObject LoadJsonObjectFromFile(SaveFormat saveFormat, QString f);
    // 发送消息到消息服务
    static void doMessage(QString msg);

signals:

public slots:

protected:
    bool event(QEvent *e);

private:
    // 获取工程名称
    QString getProjectName(const QString &szProjectPath);

public:
    static QScriptEngine *scriptEngine_;
    // 执行脚本功能
    static void execScriptFunction(const QStringList &szFuncList, const QString &szMatchEvent);
    // 执行脚本文本
    static void execScriptText(const QString &szScriptText, const QString &szMatchEvent);

private:
    //static RunScript *m_pRunScript;
    QProjectCore *projCore;

private:
    QStringList m_listPortName;
    QList<Vendor *> m_vendors;
    QList<PortThread *> m_listPortThread;
};

extern HmiRunTime *g_pHmiRunTime;

#endif // HMIRUNTIME_H
