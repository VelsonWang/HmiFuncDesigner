#include "RunScript.h"
#include "JavaScript.h"
#include "Log.h"
#include "../DB/RealTimeDB.h"
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>

RunScriptTask::RunScriptTask(QString script) :
    m_strScript(script)
{
    this->setAutoDelete(true);
    m_pScriptEngine = new QScriptEngine();
    addFuncToScriptEngine(m_pScriptEngine);
}

RunScriptTask::~RunScriptTask()
{
    //qDebug() << "\n" << QThread::currentThread()<<m_strScript;
    if(m_pScriptEngine != nullptr) {
        delete m_pScriptEngine;
        m_pScriptEngine = nullptr;
    }
}

void RunScriptTask::run()
{
    //qDebug() << "\n" << QThread::currentThread();
    //qDebug() << "m_strScript： \n" << m_strScript << "\n";
    QScriptValue result = m_pScriptEngine->evaluate(m_strScript);
    if (result.isError()) {
        QString err = QString::fromLatin1("script syntax evaluate error: %1 %2")
                .arg(result.property("lineNumber").toInt32())
                .arg(result.toString());
        LogError(err);
        return;
    }
}

//////////////////////////////////////////////////////////////

RunScript::RunScript(QString projectPath, QObject *parent) :
    QObject(parent),
    m_sProjectPath(projectPath),
    m_bTimerPeriodRun(false)
{
    m_runOnStartList.clear();
    m_runOnPeriodList.clear();
    m_runOnConditionList.clear();
    m_threadPool.setMaxThreadCount(32);
}


RunScript::~RunScript()
{
    clear();
    if(m_bTimerPeriodRun) {
        delete m_timerPeriod;
        m_bTimerPeriodRun = false;
    }
}

void RunScript::clear()
{
    m_runOnStartList.clear();
    while (!m_runOnPeriodList.isEmpty())
        delete m_runOnPeriodList.takeFirst();
    while (!m_runOnConditionList.isEmpty())
        delete m_runOnConditionList.takeFirst();
}

bool RunScript::loadScriptObjects()
{
    clear();

    QString fileDes = m_sProjectPath + "/Scripts/Script.info";
    ScriptFileManage::load(fileDes, DATA_SAVE_FORMAT);

    for(int i=0; i<ScriptFileManage::m_listScriptInfo.count(); i++) {
        ScriptObject *pObj = ScriptFileManage::m_listScriptInfo.at(i);
        if(!pObj->m_bInUse)
            continue;

        QString scriptFileName = m_sProjectPath + "/Scripts/" + pObj->m_strName + ".js";
        QFile scriptFile(scriptFileName);
        if (!scriptFile.open(QIODevice::ReadOnly))
            return false;
        QTextStream in(&scriptFile);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        in.setCodec(codec);
        QString scriptText = in.readAll();
        //qDebug() << "\nscriptFileName:" << scriptFileName << "\nscriptText: " << scriptText;

        if(pObj->m_strRunMode == QString("RunOnStart")) {
            m_runOnStartList.append(scriptText);
        } else if(pObj->m_strRunMode == QString("RunOnPeriod")) {
            TPeriodScriptObject *pScriptObject = new TPeriodScriptObject();
            pScriptObject->scriptText = scriptText;
            pScriptObject->curValue = 0;
            if(pObj->m_strRunModeArgs == "")
                pScriptObject->preValue = 0;
            else
                pScriptObject->preValue = pObj->m_strRunModeArgs.toInt();
            m_runOnPeriodList.append(pScriptObject);
        } else if(pObj->m_strRunMode == QString("RunOnCondition")) {
            TConditionScriptObject *pScriptObject = new TConditionScriptObject();
            pScriptObject->scriptText = scriptText;
            QString condition = pObj->m_strRunModeArgs;
            m_runOnConditionList.append(pScriptObject);

            if(condition.indexOf("==") != -1) {
                pScriptObject->opt = "==";
            } else if(condition.indexOf(">=") != -1) {
                pScriptObject->opt = ">=";
            } else if(condition.indexOf(">=") == -1 && condition.indexOf(">") != -1) {
                pScriptObject->opt = ">";
            } else if(condition.indexOf("<=") != -1) {
                pScriptObject->opt = "<=";
            } else if(condition.indexOf("<=") == -1 && condition.indexOf("<") != -1) {
                pScriptObject->opt = "<";
            }

            QStringList listOpt = condition.split(pScriptObject->opt);
            if(listOpt.size() != 2)
                return false;
            QString leftOpt = listOpt.at(0);
            QString rightOpt = listOpt.at(1);
            if(rightOpt.indexOf("变量") != -1) {
                pScriptObject->tagIdLeft = RealTimeDB::instance()->getIdByTagName(leftOpt);
                pScriptObject->tagIdRight = RealTimeDB::instance()->getIdByTagName(rightOpt);
            } else {
                pScriptObject->tagIdLeft = RealTimeDB::instance()->getIdByTagName(leftOpt);
                pScriptObject->tagIdRight = "";
                if(rightOpt != "")
                    pScriptObject->dRight = rightOpt.toDouble();
            }
            //qDebug() << pScriptObject->tagIdLeft << pScriptObject->opt << pScriptObject->tagIdRight << pScriptObject->dRight;
        } else {

        }
        scriptFile.close();
    }
	return true;
}

/**
 * @brief RunScript::runOnStartScripts
 * @details 启动运行脚本
 * @return
 */
bool RunScript::runOnStartScripts()
{
    //qDebug() << "run OnStartScripts.";
    for (int i = 0; i < m_runOnStartList.size(); i++) {
        m_threadPool.start(new RunScriptTask(m_runOnStartList.at(i)));
    }
	return true;
}

/**
 * @brief RunScript::runOnStartScripts
 * @details 定时运行脚本
 * @return
 */
bool RunScript::runOnPeriodScripts()
{
    if(!m_bTimerPeriodRun) {
        //qDebug() << "run PeriodScripts.";
        m_bTimerPeriodRun = true;
        m_timerPeriod = new QTimer(this);
        connect(m_timerPeriod, SIGNAL(timeout()), this, SLOT(timeout()));
        m_timerPeriod->start(1);
    }
	return true;
}

void RunScript::stopRunOnPeriodScripts()
{
    m_bTimerPeriodRun = false;
    m_timerPeriod->stop();
}

void RunScript::timeout()
{
    for (int i = 0; i < m_runOnPeriodList.size(); i++) {
        TPeriodScriptObject *pScriptObject = m_runOnPeriodList.at(i);
        if(pScriptObject->curValue++ >= pScriptObject->preValue) {
            pScriptObject->curValue = 0;
            m_threadPool.start(new RunScriptTask(pScriptObject->scriptText));
        }
    }
}

/**
 * @brief RunScript::runOnConditionScripts
 * @details 条件运行脚本
 * @return
 */
bool RunScript::runOnConditionScripts(const QString &tagId)
{
    for (int i = 0; i < m_runOnConditionList.size(); i++) {
        TConditionScriptObject *pScriptObject = m_runOnConditionList.at(i);
        //qDebug() << pScriptObject->tagIdLeft << pScriptObject->opt << pScriptObject->tagIdRight << pScriptObject->dRight;
        if(tagId == pScriptObject->tagIdLeft || tagId == pScriptObject->tagIdRight) {
            double dLeft = RealTimeDB::instance()->GetDataString(pScriptObject->tagIdLeft).toDouble();
            double dRight = 0.0;
            if(pScriptObject->tagIdRight == "") {
                dRight = pScriptObject->dRight;
            } else {
                dRight = RealTimeDB::instance()->GetDataString(pScriptObject->tagIdRight).toDouble();
            }
            bool result = false;
            if(pScriptObject->opt == "==") {
                result = (dLeft == dRight);
            } else if(pScriptObject->opt == ">=") {
                result = (dLeft >= dRight);
            } else if(pScriptObject->opt == ">") {
                result = (dLeft > dRight);
            } else if(pScriptObject->opt == "<=") {
                result = (dLeft <= dRight);
            } else if(pScriptObject->opt == "<") {
                result = (dLeft < dRight);
            }
            if(result) {
                m_threadPool.start(new RunScriptTask(pScriptObject->scriptText));
            }
        }
    }
	return true;
}
