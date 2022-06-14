#ifndef SCRIPT_H
#define SCRIPT_H

#include <QString>
#include <QStringList>
#include <QList>
#include "../xmlobject.h"
#include "../sharedlibglobal.h"

class SHAREDLIB_EXPORT ScriptObject
{
public:
    QString m_szName;
    bool m_bInUse;
    QString m_szDescription;
    QString m_szRunMode;
    QString m_szRunModeArgs;
    QString m_szScriptText;
};


class SHAREDLIB_EXPORT Script
{
public:
    Script();
    ~Script();

    void AddScriptObject(ScriptObject * obj);
    void ModifyScriptObject(ScriptObject * oldobj, ScriptObject * newobj);
    void DeleteScriptObject(ScriptObject * obj);
    ScriptObject *GetScriptObject(const QString &szName);

    bool openFromXml(XMLObject *pXmlObj);
    bool saveToXml(XMLObject *pXmlObj);

public:
    QList<ScriptObject *> m_scriptObjects;
};


#endif // SCRIPT_H
