#ifndef JAVASCRIPT_H
#define JAVASCRIPT_H

#include "../Public/public.h"

#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptEngine>

class ScriptObject
{
public:
    ScriptObject();
    ~ScriptObject();
    void load(QJsonObject &json);
    void save(QJsonObject &json);

public:
    QString m_strName;
    bool m_bInUse;
    QString m_strDescription;
    QString m_strRunMode;
    QString m_strRunModeArgs;
};

class ScriptFileManage
{
public:
    static void AddScriptInfo(ScriptObject * obj);
    static void ModifyScriptInfo(ScriptObject * oldobj, ScriptObject * newobj);
    static void DeleteScriptInfo(ScriptObject * obj);
    static ScriptObject *GetScriptObject(const QString &name);
    static void load(const QString &filename=QString(), SaveFormat saveFormat=Json);
    static void save(const QString &filename=QString(), SaveFormat saveFormat=Json);

public:
    static QList<ScriptObject *> m_listScriptInfo;
};

extern void addFuncToScriptEngine(QScriptEngine *engine);

#endif // JAVASCRIPT_H
