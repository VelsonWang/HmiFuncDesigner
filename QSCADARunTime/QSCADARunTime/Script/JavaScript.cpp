#include "JavaScript.h"
#include "../DB/RealTimeDB.h"

#include <QMenu>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QDataStream>
#include <QThread>
#include <QScriptValue>
#include <QScriptContext>
#include <QScriptEngine>

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_LINUX
#include <stdlib.h>
#endif

#ifdef Q_OS_MAC
#endif



QList<ScriptObject *> ScriptFileManage::m_listScriptInfo = QList<ScriptObject *>();


ScriptObject::ScriptObject()
{

}

ScriptObject::~ScriptObject()
{

}


void ScriptObject::load(QJsonObject &json)
{
    m_strName = json["Name"].toString();
    m_bInUse = json["InUse"].toBool();
    m_strDescription = json["Description"].toString();
    m_strRunMode = json["RunMode"].toString();
    m_strRunModeArgs = json["RunModeArgs"].toString();
}

void ScriptObject::save(QJsonObject &json)
{
    json["Name"] = m_strName;
    json["InUse"] = m_bInUse;
    json["Description"] = m_strDescription;
    json["RunMode"] = m_strRunMode;
    json["RunModeArgs"] = m_strRunModeArgs;
}


void ScriptFileManage::AddScriptInfo(ScriptObject * obj)
{
    int pos = m_listScriptInfo.indexOf(obj);
    if(pos == -1)
        m_listScriptInfo.append(obj);
}


void ScriptFileManage::ModifyScriptInfo(ScriptObject * oldobj, ScriptObject * newobj)
{
    int pos = m_listScriptInfo.indexOf(oldobj);
    if(pos == -1)
        return;
    m_listScriptInfo.replace(pos, newobj);
}


void ScriptFileManage::DeleteScriptInfo(ScriptObject * obj)
{
    m_listScriptInfo.removeOne(obj);
}

ScriptObject *ScriptFileManage::GetScriptObject(const QString &name)
{
    foreach (ScriptObject *pobj, m_listScriptInfo)
    {
        if(pobj->m_strName == name)
            return pobj;
    }
    return NULL;
}

void ScriptFileManage::load(const QString &filename, SaveFormat saveFormat)
{
    QFile loadFile(filename);

    if(!loadFile.exists())
        return;

    if (!loadFile.open(QIODevice::ReadOnly))
        return;

    m_listScriptInfo.clear();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    const QJsonObject json = loadDoc.object();

    QJsonArray scriptInfoArray = json["ScriptInfos"].toArray();
    for (int i = 0; i < scriptInfoArray.size(); ++i)
    {
        QJsonObject jsonObj = scriptInfoArray[i].toObject();
        ScriptObject *pObj = new ScriptObject();
        pObj->load(jsonObj);
        m_listScriptInfo.append(pObj);
    }

    loadFile.close();
}


void ScriptFileManage::save(const QString &filename, SaveFormat saveFormat)
{
    QString strPath = filename.left(filename.lastIndexOf("/"));
    QDir dir(strPath);
    if(!dir.exists())
    {
        dir.mkpath(strPath);
    }

    QFile saveFile(filename);
    QJsonObject obj;
    QJsonArray scriptInfosArray;

    saveFile.open(QFile::WriteOnly);

    for(int i = 0; i < m_listScriptInfo.size(); i++)
    {
        QJsonObject jsonObj;
        ScriptObject *pObj = m_listScriptInfo.at(i);
        pObj->save(jsonObj);
        scriptInfosArray.append(jsonObj);
    }

    obj["ScriptInfos"] = scriptInfosArray;

    QJsonDocument saveDoc(obj);
    saveFile.write(saveFormat == Json ? saveDoc.toJson() : saveDoc.toBinaryData());
    saveFile.close();
}



/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////// 脚本函数 //////////////////////////////////

/*
* 设置变量值
*/
QScriptValue SetRealValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);

    qint32 id = RealTimeDB::getIdByTagName(sVar.toString());
    if(id != -1)
    {
        RealTimeDB::SetDataString(id, QString("%1").arg(dValue.toNumber()));
    }
    return dValue;
}
/*
* 变量值自增
*/
QScriptValue AddAIValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sVar.toString());
    if(id != -1)
    {
        double oldValue = RealTimeDB::GetDataString(id).toDouble();
        newValue = oldValue + dValue.toNumber();
        RealTimeDB::SetDataString(id, QString("%1").arg(newValue));
    }
    return newValue;
}

/*
* 变量值自减
*/
QScriptValue SubAIValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sVar.toString());
    if(id != -1)
    {
        double oldValue = RealTimeDB::GetDataString(id).toDouble();
        newValue = oldValue - dValue.toNumber();
        RealTimeDB::SetDataString(id, QString("%1").arg(newValue));
    }
    return newValue;
}


/*
* 获取变量的值
*/
QScriptValue GetRealValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    double dValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sVar.toString());
    if(id != -1)
    {
        dValue = RealTimeDB::GetDataString(id).toDouble();
    }
    return dValue;
}


/*
* 变量0,1切换
*/
QScriptValue StateChange(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sVar.toString());
    if(id != -1)
    {
        double oldValue = RealTimeDB::GetDataString(id).toDouble();
        if(oldValue != 0.0)
            newValue = 0.0;
        else
            newValue = 1.0;
        RealTimeDB::SetDataString(id, QString("%1").arg(newValue));
    }
    return newValue;
}


/*
* 设置系统变量值
*/
QScriptValue SetSysValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sSys = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sSys.toString());
    if(id != -1)
    {
        newValue = dValue.toNumber();
        RealTimeDB::SetDataString(id, QString("%1").arg(newValue));
    }

    return newValue;
}

/*
* 获取系统变量值
*/
QScriptValue GetSysValue(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sSys = context->argument(0);
    double dValue = 0.0;

    qint32 id = RealTimeDB::getIdByTagName(sSys.toString());
    if(id != -1)
    {
        dValue = RealTimeDB::GetDataString(id).toDouble();
    }
    return dValue;
}

//////////////////////////// 系统操作函数 /////////////////////////////////

/*
* 控制延时n秒
*/
QScriptValue Sleep(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue nSec = context->argument(0);
    QThread::sleep(nSec.toInteger());
    return 1;
}



/*
* 设置系统时间
*/
QScriptValue SetDateTime(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    int ny = context->argument(0).toInteger();
    int nm = context->argument(0).toInteger();
    int nd = context->argument(0).toInteger();
    int nh = context->argument(0).toInteger();
    int nf = context->argument(0).toInteger();
    int ns = context->argument(0).toInteger();

#ifdef Q_OS_WIN
    SYSTEMTIME st;
    GetSystemTime(&st);
    st.wYear = ny;
    st.wMonth = nm;
    st.wDay = nd;
    st.wHour = nh;
    st.wMinute = nf;
    st.wSecond = ns;
    SetSystemTime(&st);
#endif

#ifdef Q_OS_LINUX
    QString str = "";
    str = "date -s " + nm + "/" + nd + "/" + ny;
    system(str.toLatin1().data());
    str = "date -s " + nh + ":" + nf + ":" + ns;
    system(str.toLatin1().data());
    //force write CMOS
    system("clock -w");
#endif

#ifdef Q_OS_MAC
#endif
    return 1;
}

/*
* 延时n毫秒
*/
QScriptValue WaitForMillisec(QScriptContext *context, QScriptEngine *engine, void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue millisec = context->argument(0);
    QThread::msleep(millisec.toInteger());
    return 1;
}

//////////////////////////////////////////////////////////////////////////////


void addFuncToScriptEngine(QScriptEngine *engine)
{
    /////////////////////////// 脚本函数 //////////////////////////////////
    // 设置变量值
    QScriptValue funcSetRealValue = engine->newFunction(SetRealValue, nullptr);
    engine->globalObject().setProperty("SetRealValue", funcSetRealValue);

    // 变量值自增
    QScriptValue funcAddAIValue = engine->newFunction(AddAIValue, nullptr);
    engine->globalObject().setProperty("AddAIValue", funcAddAIValue);
    // 变量值自减
    QScriptValue funcSubAIValue = engine->newFunction(SubAIValue, nullptr);
    engine->globalObject().setProperty("SubAIValue", funcSubAIValue);

    // 获取变量的值
    QScriptValue funcGetRealValue = engine->newFunction(GetRealValue, nullptr);
    engine->globalObject().setProperty("GetRealValue", funcGetRealValue);

    // 变量0,1切换
    QScriptValue funcStateChange = engine->newFunction(StateChange, nullptr);
    engine->globalObject().setProperty("StateChange", funcStateChange);

    // 设置系统变量值
    QScriptValue funcSetSysValue = engine->newFunction(SetSysValue, nullptr);
    engine->globalObject().setProperty("SetSysValue", funcSetSysValue);

    // 获取系统变量值
    QScriptValue funcGetSysValue = engine->newFunction(GetSysValue, nullptr);
    engine->globalObject().setProperty("GetSysValue", funcGetSysValue);

    //////////////////////////// 系统操作函数 /////////////////////////////////

    // 控制延时n秒
    QScriptValue funcSleep = engine->newFunction(Sleep, nullptr);
    engine->globalObject().setProperty("Sleep", funcSleep);


    // 延时n毫秒
    QScriptValue funcWaitForMillisec = engine->newFunction(WaitForMillisec, nullptr);
    engine->globalObject().setProperty("WaitForMillisec", funcWaitForMillisec);


}
