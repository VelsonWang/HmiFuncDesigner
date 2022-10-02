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

private:
    explicit HmiRunTime(QObject *parent = NULL);
    ~HmiRunTime();

public:
    static HmiRunTime *instance() {
        static HmiRunTime obj;
        return &obj;
    }

    bool Load();
    bool Unload();
    void Start();
    void Stop();
    void AddPortName(const QString name);
    Vendor *FindVendor(const QString name);

    void setProjectCore(QProjectCore *core) {
        projCore = core;
    }

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

/**
 * @brief getProjectName
 * @details 获取工程名称
 * @param szProjectPath
 * @return
 */
inline QString getProjectName(const QString &szProjectPath)
{
    QFileInfo srcFileInfo(szProjectPath);
    QString szProjName = "";

    if(srcFileInfo.isDir()) {
        QDir sourceDir(szProjectPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        foreach(const QString &fileName, fileNames) {
            if(fileName.endsWith(".pdt")) {
                QFileInfo info(fileName);
                szProjName = info.baseName();
            }
        }
    }

    return szProjName;
}

#endif // HMIRUNTIME_H
