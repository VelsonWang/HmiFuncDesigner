#include "JavaScript.h"
#include "../DB/RealTimeDB.h"
#include "MainWindow.h"

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


void ScriptFileManage::ModifyScriptInfo(ScriptObject * oldobj,
                                        ScriptObject * newobj)
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
    return Q_NULLPTR;
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

/////////////////////////// 画面操作函数 //////////////////////////////////


/**
 * @brief SwitchGraphPage
 * @details 切换至画面
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue SwitchGraphPage(QScriptContext *context,
                             QScriptEngine *engine,
                             void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue page = context->argument(0);
    QString szPageName = page.toString();
    MainWindow::instance()->switchGraphPage(szPageName);
    return 1;
}


/**
 * @brief ReturnGraphPage
 * @details 返回至上一画面
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue ReturnGraphPage(QScriptContext *context,
                             QScriptEngine *engine,
                             void *pargs) {
    Q_UNUSED(context)
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    MainWindow::instance()->returnGraphPage();
    return 1;
}





/////////////////////////// 脚本函数 //////////////////////////////////

/*
* 设置变量值
*/
QScriptValue SetRealValue(QScriptContext *context,
                          QScriptEngine *engine,
                          void *pargs)
{
    Q_UNUSED(pargs)
    Q_UNUSED(engine)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sVar.toString());
    if(szTagTD != "") {
        RealTimeDB::instance()->SetDataString(szTagTD, QString("%1").arg(dValue.toNumber()));
    }
    return dValue;
}

/*
* 变量值自增
*/
QScriptValue AddAIValue(QScriptContext *context,
                        QScriptEngine *engine,
                        void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sVar.toString());
    if(szTagTD != "") {
        double oldValue = RealTimeDB::instance()->GetDataString(szTagTD).toDouble();
        newValue = oldValue + dValue.toNumber();
        RealTimeDB::instance()->SetDataString(szTagTD, QString("%1").arg(newValue));
    }
    return newValue;
}

/*
* 变量值自减
*/
QScriptValue SubAIValue(QScriptContext *context,
                        QScriptEngine *engine,
                        void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sVar.toString());
    if(szTagTD != "") {
        double oldValue = RealTimeDB::instance()->GetDataString(szTagTD).toDouble();
        newValue = oldValue - dValue.toNumber();
        RealTimeDB::instance()->SetDataString(szTagTD, QString("%1").arg(newValue));
    }
    return newValue;
}


/*
* 获取变量的值
*/
QScriptValue GetRealValue(QScriptContext *context,
                          QScriptEngine *engine,
                          void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    double dValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sVar.toString());
    if(szTagTD != "") {
        dValue = RealTimeDB::instance()->GetDataString(szTagTD).toDouble();
    }
    return dValue;
}


/*
* 变量0,1切换
*/
QScriptValue StateChange(QScriptContext *context,
                         QScriptEngine *engine,
                         void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sVar = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sVar.toString());
    if(szTagTD != "") {
        double oldValue = RealTimeDB::instance()->GetDataString(szTagTD).toDouble();
        if(oldValue != 0.0)
            newValue = 0.0;
        else
            newValue = 1.0;
        RealTimeDB::instance()->SetDataString(szTagTD, QString("%1").arg(newValue));
    }
    return newValue;
}


/*
* 设置系统变量值
*/
QScriptValue SetSysValue(QScriptContext *context,
                         QScriptEngine *engine,
                         void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sSys = context->argument(0);
    QScriptValue dValue = context->argument(1);
    double newValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sSys.toString());
    if(szTagTD != "") {
        newValue = dValue.toNumber();
        RealTimeDB::instance()->SetDataString(szTagTD, QString("%1").arg(newValue));
    }

    return newValue;
}

/*
* 获取系统变量值
*/
QScriptValue GetSysValue(QScriptContext *context,
                         QScriptEngine *engine,
                         void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue sSys = context->argument(0);
    double dValue = 0.0;

    QString szTagTD = RealTimeDB::instance()->getIdByTagName(sSys.toString());
    if(szTagTD != "") {
        dValue = RealTimeDB::instance()->GetDataString(szTagTD).toDouble();
    }
    return dValue;
}

//////////////////////////// 系统操作函数 /////////////////////////////////

/*
* 控制延时n秒
*/
QScriptValue Sleep(QScriptContext *context,
                   QScriptEngine *engine,
                   void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue nSec = context->argument(0);
    unsigned long lSec = static_cast<unsigned long>(nSec.toInteger());
    QThread::sleep(lSec);
    return 1;
}



/*
* 设置系统时间
*/
QScriptValue SetDateTime(QScriptContext *context,
                         QScriptEngine *engine,
                         void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    int ny = static_cast<int>(context->argument(0).toInteger());
    int nm = static_cast<int>(context->argument(1).toInteger());
    int nd = static_cast<int>(context->argument(2).toInteger());
    int nh = static_cast<int>(context->argument(3).toInteger());
    int nf = static_cast<int>(context->argument(4).toInteger());
    int ns = static_cast<int>(context->argument(5).toInteger());

#ifdef Q_OS_WIN
    SYSTEMTIME st;
    GetSystemTime(&st);
    st.wYear = static_cast<WORD>(ny);
    st.wMonth = static_cast<WORD>(nm);
    st.wDay = static_cast<WORD>(nd);
    st.wHour = static_cast<WORD>(nh);
    st.wMinute = static_cast<WORD>(nf);
    st.wSecond = static_cast<WORD>(ns);
    SetSystemTime(&st);
#endif

#ifdef Q_OS_LINUX
    QString szCmd = QString("date -s %1/%2/%3").arg(nm).arg(nd).arg(ny);
    system(szCmd.toLatin1().data());
    szCmd = QString("date -s %1:%2:%3").arg(nh).arg(nf).arg(ns);
    system(szCmd.toLatin1().data());
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
QScriptValue WaitForMillisec(QScriptContext *context,
                             QScriptEngine *engine,
                             void *pargs)
{
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue millisec = context->argument(0);
    unsigned long lMsec = static_cast<unsigned long>(millisec.toInteger());
    QThread::msleep(lMsec);
    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////// 控件操作函数 //////////////////////////////////


/**
 * @brief HideControlElement
 * @details 隐藏控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue HideControlElement(QScriptContext *context,
                                QScriptEngine *engine,
                                void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->hideControlElement(szId);
    return 1;
}

/**
 * @brief ShowControlElement
 * @details 显示控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue ShowControlElement(QScriptContext *context,
                                QScriptEngine *engine,
                                void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->showControlElement(szId);
    return 1;
}

/**
 * @brief EnableControlElement
 * @details 生效控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue EnableControlElement(QScriptContext *context,
                                  QScriptEngine *engine,
                                  void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->enableElement(szId);
    return 1;
}

/**
 * @brief DisableControlElement
 * @details 失效控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue DisableControlElement(QScriptContext *context,
                                   QScriptEngine *engine,
                                   void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->disableElement(szId);
    return 1;
}

/**
 * @brief MoveControlElement
 * @details 偏移控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue MoveControlElement(QScriptContext *context,
                                QScriptEngine *engine,
                                void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QScriptValue nx = context->argument(1);
    QScriptValue ny = context->argument(2);
    QString szId = id.toString();
    int x = static_cast<int>(nx.toInteger());
    int y = static_cast<int>(ny.toInteger());
    MainWindow::instance()->moveControlElement(szId, x, y);
    return 1;
}

/**
 * @brief BlinkControlElement
 * @details 闪烁控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue BlinkControlElement(QScriptContext *context,
                                 QScriptEngine *engine,
                                 void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->startBlinkElement(szId);
    return 1;
}

/**
 * @brief StopBlinkControlElement
 * @details 停止闪烁控件
 * @param context
 * @param engine
 * @param pargs
 * @return
 */
QScriptValue StopBlinkControlElement(QScriptContext *context,
                                     QScriptEngine *engine,
                                     void *pargs) {
    Q_UNUSED(engine)
    Q_UNUSED(pargs)
    QScriptValue id = context->argument(0);
    QString szId = id.toString();
    MainWindow::instance()->stopBlinkElement(szId);
    return 1;
}

//////////////////////////////////////////////////////////////////////////////

void addFuncToScriptEngine(QScriptEngine *engine)
{
    /////////////////////////// 画面操作函数 //////////////////////////////////
    // 切换画面
    QScriptValue funcSwitchGraphPage = engine->newFunction(SwitchGraphPage, nullptr);
    engine->globalObject().setProperty("SwitchGraphPage", funcSwitchGraphPage);

    // 返回至上一画面
    QScriptValue funcReturnGraphPage = engine->newFunction(ReturnGraphPage, nullptr);
    engine->globalObject().setProperty("ReturnGraphPage", funcReturnGraphPage);

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

    /////////////////////////// 控件操作函数 //////////////////////////////////

    // 隐藏控件
    QScriptValue funcHideControlElement = engine->newFunction(HideControlElement, nullptr);
    engine->globalObject().setProperty("HideControlElement", funcHideControlElement);

    // 显示控件
    QScriptValue funcShowControlElement = engine->newFunction(ShowControlElement, nullptr);
    engine->globalObject().setProperty("ShowControlElement", funcShowControlElement);

    // 生效控件
    QScriptValue funcEnableControlElement = engine->newFunction(EnableControlElement, nullptr);
    engine->globalObject().setProperty("EnableControlElement", funcEnableControlElement);

    // 失效控件
    QScriptValue funcDisableControlElement = engine->newFunction(DisableControlElement, nullptr);
    engine->globalObject().setProperty("DisableControlElement", funcDisableControlElement);

    // 偏移控件
    QScriptValue funcMoveControlElement = engine->newFunction(MoveControlElement, nullptr);
    engine->globalObject().setProperty("MoveControlElement", funcMoveControlElement);

    // 闪烁控件
    QScriptValue funcBlinkControlElement = engine->newFunction(BlinkControlElement, nullptr);
    engine->globalObject().setProperty("BlinkControlElement", funcBlinkControlElement);

    // 停止闪烁控件
    QScriptValue funcStopBlinkControlElement = engine->newFunction(StopBlinkControlElement, nullptr);
    engine->globalObject().setProperty("StopBlinkControlElement", funcStopBlinkControlElement);
}
