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
#include <QScriptEngine>
#include <QScriptValue>
#include <QMutex>
#include "Vendor.h"
#include "RealTimeDB.h"
#include "PortThread.h"
#include "RunScript.h"


class HmiRunTime : public QObject
{
    Q_OBJECT
public:
    explicit HmiRunTime(QString projectPath, QObject *parent = Q_NULLPTR);
    ~HmiRunTime();
    bool Load(SaveFormat saveFormat);
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

public:
    static QScriptEngine *scriptEngine_;
    // 执行脚本功能
    static void execScriptFunction(const QStringList &szFuncList,
                                   const QString &szMatchEvent);
    // 执行脚本文本
    static void execScriptText(const QString &szScriptText,
                               const QString &szMatchEvent);

private:
    // 获取工程名称
    QString getProjectName(const QString &szProjectPath);

private:
    static QString m_sProjectPath;
    QStringList m_listPortName;
    QList<Vendor *> m_VendorList;
    QList<PortThread *> m_listPortThread;
    static RunScript *m_pRunScript;

};

extern HmiRunTime *g_pHmiRunTime;

#endif // HMIRUNTIME_H
