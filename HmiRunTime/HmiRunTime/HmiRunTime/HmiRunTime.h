#ifndef HMIRUNTIME_H
#define HMIRUNTIME_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QMutex>
#include "Vendor.h"
#include "realtimedb.h"
#include "PortThread.h"
#include "qprojectcore.h"

class Tag;

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

private:
    RunTimeTag *createRunTimeTag(Tag *pTagObj);

signals:

public slots:

protected:
    bool event(QEvent *e);

private:
    QProjectCore *projCore;

private:
    QStringList m_listPortName;
    QList<Vendor *> m_vendors;
    QList<PortThread *> m_listPortThread;
};

extern HmiRunTime *g_pHmiRunTime;

#endif // HMIRUNTIME_H
