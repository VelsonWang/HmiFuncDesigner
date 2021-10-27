#include "Script.h"

Script::Script()
{
    m_listScriptObjects.clear();
}

Script::~Script()
{
    qDeleteAll(m_listScriptObjects);
    m_listScriptObjects.clear();
}


void Script::AddScriptObject(ScriptObject *obj)
{
    int pos = m_listScriptObjects.indexOf(obj);
    if (pos == -1) {
        m_listScriptObjects.append(obj);
    }
}

void Script::ModifyScriptObject(ScriptObject *oldobj, ScriptObject *newobj)
{
    int pos = m_listScriptObjects.indexOf(oldobj);
    if (pos == -1) {
        return;
    }
    m_listScriptObjects.replace(pos, newobj);
}

void Script::DeleteScriptObject(ScriptObject *obj)
{
    m_listScriptObjects.removeOne(obj);
}

ScriptObject *Script::GetScriptObject(const QString &szName)
{
    foreach (ScriptObject *pobj, m_listScriptObjects) {
        if (pobj->m_szName == szName) {
            return pobj;
        }
    }
    return NULL;
}


bool Script::openFromXml(XMLObject *pXmlObj)
{
    qDeleteAll(m_listScriptObjects);
    m_listScriptObjects.clear();
    XMLObject *pScriptsObj = pXmlObj->getCurrentChild("scripts");
    if(pScriptsObj == NULL) {
        return false;
    }
    QVector<XMLObject* > listScriptsObj = pScriptsObj->getCurrentChildren("script");
    foreach(XMLObject* pScriptObj, listScriptsObj) {
        ScriptObject *pObj = new ScriptObject();
        pObj->m_szName = pScriptObj->getProperty("name");
        pObj->m_bInUse = pScriptObj->getProperty("in_use") == "1";
        pObj->m_szDescription = pScriptObj->getProperty("desc");
        pObj->m_szRunMode = pScriptObj->getProperty("run_mode");
        pObj->m_szRunModeArgs = pScriptObj->getProperty("run_mode_args");
        pObj->m_szScriptText = pScriptObj->getText();
        m_listScriptObjects.append(pObj);
    }
    return true;
}


bool Script::saveToXml(XMLObject *pXmlObj)
{
    XMLObject *pScriptsObj = new XMLObject(pXmlObj);
    pScriptsObj->setTagName("scripts");
    for (int i = 0; i < m_listScriptObjects.size(); i++) {
        ScriptObject *pObj = m_listScriptObjects.at(i);
        XMLObject *pScriptObj = new XMLObject(pScriptsObj);
        pScriptObj->setTagName("script");
        pScriptObj->setProperty("name", pObj->m_szName);
        pScriptObj->setProperty("in_use", pObj->m_bInUse ? "1" : "0");
        pScriptObj->setProperty("desc", pObj->m_szDescription);
        pScriptObj->setProperty("run_mode", pObj->m_szRunMode);
        pScriptObj->setProperty("run_mode_args", pObj->m_szRunModeArgs);
        pScriptObj->setText(pObj->m_szScriptText);
    }
    return true;
}




