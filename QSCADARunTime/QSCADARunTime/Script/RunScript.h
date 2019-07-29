#ifndef RUNSCRIPT_H
#define RUNSCRIPT_H

#include "JavaScript.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QScriptEngine>
#include <QTimer>

class RunScriptTask : public QRunnable
{
public:
    RunScriptTask(QScriptEngine *engine, QString script);
    ~RunScriptTask();

private:
    void run();

private:
    QScriptEngine *m_pScriptEngine;
    QString m_strScript;
};

typedef struct tagPeriodScriptObject
{
    QString scriptText;
    int preValue;
    int curValue;
    tagPeriodScriptObject()
    {
        scriptText = "";
        preValue = 0;
        curValue = 0;
    }
}TPeriodScriptObject;

typedef struct tagConditionScriptObject
{
    QString scriptText;
    QString tagIdLeft;
    QString tagIdRight;
    double dRight;
    QString opt;
    tagConditionScriptObject()
    {
        scriptText = "";
        opt = "";
        tagIdLeft = "";
        tagIdRight = "";
        dRight = 0.0;
    }
}TConditionScriptObject;

class RunScript : public QObject
{
    Q_OBJECT
public:
    explicit RunScript(QString projectPath, QObject *parent = nullptr);
    ~RunScript();
    bool loadScriptObjects();
    bool runOnStartScripts();
    bool runOnPeriodScripts();
    void stopRunOnPeriodScripts();
    bool runOnConditionScripts(qint32 tagId);

private:
    void clear();

private slots:
    void timeout();

private:
    QScriptEngine *m_pScriptEngine;
    QString m_sProjectPath;
    QThreadPool m_threadPool;
    QList<QString> m_runOnStartList;
    QList<TPeriodScriptObject *> m_runOnPeriodList;
    QTimer *m_timerPeriod;
    bool m_bTimerPeriodRun;
    QList<TConditionScriptObject *> m_runOnConditionList;
};

#endif // RUNSCRIPT_H
