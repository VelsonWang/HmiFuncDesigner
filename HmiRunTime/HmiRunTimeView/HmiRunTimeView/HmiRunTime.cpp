#include "HmiRunTime.h"
#include "public.h"
#include "RealTimeDB.h"
#include "SysRuntimeEvent.h"
#include "log4qt/logger.h"
#include "Log.h"
#include "MainWindow.h"
#include "ProjectInfoManager.h"
#include "ProjectData.h"
#include <QTextStream>
#include <QTextCodec>
#include <QMutexLocker>
#include <QDebug>

HmiRunTime *g_pHmiRunTime = Q_NULLPTR;
QString HmiRunTime::m_sProjectPath = QString("");
RunScript *HmiRunTime::m_pRunScript = Q_NULLPTR;
QScriptEngine *HmiRunTime::scriptEngine_ = Q_NULLPTR;


HmiRunTime::HmiRunTime(QString projectPath, QObject *parent)
    : QObject(parent)
{
    m_sProjectPath = projectPath;
    m_pRunScript = new RunScript(projectPath);
}

HmiRunTime::~HmiRunTime()
{
    if(m_pRunScript != Q_NULLPTR) {
        delete m_pRunScript;
        m_pRunScript = Q_NULLPTR;
    }
    if(scriptEngine_ != Q_NULLPTR) {
        delete scriptEngine_;
        scriptEngine_ = Q_NULLPTR;
    }
}


bool HmiRunTime::Load(SaveFormat saveFormat)
{
    if(m_sProjectPath == "")
        return false;

    QDir projDir(m_sProjectPath);
    if(!projDir.exists())
        return false;

    QString szProjName = getProjectName(m_sProjectPath);

    if(szProjName == "") {
        LogError("project information file not found!");
        return false;
    }

    ProjectData::getInstance()->createOrOpenProjectData(m_sProjectPath, szProjName);

    // load tags and create rtdb
    //-----------------系统变量------------------//
    TagSys &tagSys = ProjectData::getInstance()->tagSys_;
    tagSys.load(ProjectData::getInstance()->dbData_);

    foreach (TagSysDBItem * itemTagSys, tagSys.listTagSysDBItem_) {
        RealTimeDB::instance()->varNameMapId.insert((QObject::tr("系统变量.") + itemTagSys->m_szName + "[" + itemTagSys->m_szTagID + "]"), itemTagSys->m_szTagID);
    }

    qDeleteAll(tagSys.listTagSysDBItem_);
    tagSys.listTagSysDBItem_.clear();

    //-----------------中间变量------------------//
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    tagTmp.load(ProjectData::getInstance()->dbData_);

    foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
        RealTimeDB::instance()->varNameMapId.insert((QObject::tr("中间变量.") + itemTagTmp->m_szName + "[" + itemTagTmp->m_szTagID + "]"), itemTagTmp->m_szTagID);
    }

    qDeleteAll(tagTmp.listTagTmpDBItem_);
    tagTmp.listTagTmpDBItem_.clear();

    //-----------------设备变量------------------//
    TagIO &tagIO = ProjectData::getInstance()->tagIO_;
    TagIOGroup &tagIOGroup = ProjectData::getInstance()->tagIOGroup_;
    tagIOGroup.load(ProjectData::getInstance()->dbData_);
    tagIO.load(ProjectData::getInstance()->dbData_);

    foreach (TagIOGroupDBItem * itemTagIOGroup, tagIOGroup.listTagIOGroupDBItem_) {
        QString szGroup = itemTagIOGroup->m_szGroupName;
        foreach (TagIODBItem * itemTagIO, tagIO.listTagIODBItem_) {
            if(szGroup != itemTagIO->m_szGroupName) continue;
            RealTimeDB::instance()->varNameMapId.insert((QObject::tr("设备变量.") + itemTagIO->m_szName + "[" + itemTagIO->m_szTagID + "]"), itemTagIO->m_szTagID);
        }
    }

    qDeleteAll(tagIO.listTagIODBItem_);
    tagIO.listTagIODBItem_.clear();
    qDeleteAll(tagIOGroup.listTagIOGroupDBItem_);
    tagIOGroup.listTagIOGroupDBItem_.clear();

    //-----------------加载JavaScript------------------//
    if(m_pRunScript == Q_NULLPTR)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->loadScriptObjects();

    //RealTimeDB::debugShowNameMapId();

    return true;
}


bool HmiRunTime::Unload()
{
    MainWindow::instance()->unLoadGraphPages();
    return true;
}


void HmiRunTime::Start()
{
    // 运行启动运行脚本
    if(m_pRunScript == Q_NULLPTR)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->runOnStartScripts();

    // 运行定时运行脚本
    if(m_pRunScript == Q_NULLPTR)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->runOnPeriodScripts();

    //////////////////////////////////////////////////////////////////////////////
    // start graph page show

    // find project infomation file
    QString szProjName = getProjectName(m_sProjectPath);

    if(szProjName == "") {
        LogError("project information file not found!");
    } else {
        ProjectInfoManager &projInfoMgr = ProjectData::getInstance()->projInfoMgr_;
        projInfoMgr.load(ProjectData::getInstance()->dbData_);
        QString startPageFile = projInfoMgr.getStartPage();
        if(startPageFile.toLower() != "none") {
            MainWindow::projpath_ = m_sProjectPath;
            MainWindow::graphPageName_ = startPageFile;
            MainWindow::instance()->loadGraphPages(m_sProjectPath);
            MainWindow::instance()->openGraphPage(m_sProjectPath, startPageFile);
            MainWindow::instance()->show();
        }
    }
}

void HmiRunTime::Stop()
{
    m_pRunScript->stopRunOnPeriodScripts();
}


QJsonObject HmiRunTime::LoadJsonObjectFromFile(SaveFormat saveFormat, QString f)
{
    QFile loadFile(f);
    if (!loadFile.open(QIODevice::ReadOnly))
        return QJsonObject();
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    loadFile.close();
    return loadDoc.object();
}

/**
 * @brief HmiRunTime::doMessage
 * @details 处理消息
 * @param msg
 */
void HmiRunTime::doMessage(QString msg)
{
    if(msg.indexOf(QString("VALUE_CHANGE")) != -1)
    {
        QString tagId = msg.replace(QString("VALUE_CHANGE "), QString(""));
        // 运行条件运行脚本
        if(m_pRunScript == Q_NULLPTR) {
            m_pRunScript = new RunScript(m_sProjectPath);
        }
        m_pRunScript->runOnConditionScripts(tagId);
    }
}


bool HmiRunTime::event(QEvent *event)
{
    Log4Qt::Logger *log = Log4Qt::Logger::logger("HmiRunTime");

    if(event->type() == EV_StartRuntime) {
        Load(DATA_SAVE_FORMAT);
        Start();
        log->debug("start runtime.");
        qDebug() << "start runtime.";
        return false;
    } else if(event->type() == EV_StopRuntime) {
        log->debug("stop runtime.");
        qDebug() << "stop runtime.";
        Stop();
        Unload();
        return false;
    } else if(event->type() == EV_RestartRuntime) {
        log->debug("restart runtime.");
        qDebug() << "restart runtime.";
        Stop();
        Unload();
        Load(DATA_SAVE_FORMAT);
        Start();
        return false;
    }
    return QObject::event(event);
}


/**
 * @brief HmiRunTime::execScriptFunction
 * @details 执行脚本功能
 * @param szFuncList 功能函数事件字符
 * @param szMatchEvent 匹配执行的事件
 */
void HmiRunTime::execScriptFunction(const QStringList &szFuncList,
                                    const QString &szMatchEvent)
{
    if(scriptEngine_ != Q_NULLPTR && szFuncList.size() > 0 && szMatchEvent != "") {
        foreach (QString szFuncEv, szFuncList) {
            QStringList listFuncEv = szFuncEv.split(':');
            if(listFuncEv.size() == 2) {
                QString szFunc = listFuncEv.at(0);
                QString szEv = listFuncEv.at(1);
                if(szEv == szMatchEvent) {
                    QScriptValue result = scriptEngine_->evaluate(szFunc);
                    if (result.isError()) {
                        QString err = QString::fromLatin1("File:%1 Function:%2 script syntax evaluate error: %3")
                                .arg(__FILE__)
                                .arg(__FUNCTION__)
                                .arg(result.toString());
                        LogError(err);
                        return;
                    }
                }
            }
        }
    }
}



/**
 * @brief HmiRunTime::execScriptText
 * @details 执行脚本文本
 * @param szScriptText 脚本文本
 * @param szMatchEvent 匹配执行的事件
 */
void HmiRunTime::execScriptText(const QString &szScriptText,
                                const QString &szMatchEvent)
{
    if(scriptEngine_ != Q_NULLPTR && szScriptText != "" && szMatchEvent != "") {
        QStringList listFuncEv = szScriptText.split("][");
        if(listFuncEv.size() == 2) {
            QString szEv = listFuncEv.at(0);
            QString szScript = listFuncEv.at(1);
            if(szEv == szMatchEvent) {
                QScriptValue result = scriptEngine_->evaluate(szScript);
                if (result.isError()) {
                    QString err = QString::fromLatin1("File:%1 Function:%2 script syntax evaluate error: %3")
                            .arg(__FILE__)
                            .arg(__FUNCTION__)
                            .arg(result.toString());
                    LogError(err);
                    return;
                }
            }
        }
    }
}


/**
 * @brief HmiRunTime::getProjectName
 * @details 获取工程名称
 * @param szProjectPath
 * @return
 */
QString HmiRunTime::getProjectName(const QString &szProjectPath)
{
    QFileInfo srcFileInfo(szProjectPath);
    QString szProjName = "";
    if (srcFileInfo.isDir()) {
        QDir sourceDir(szProjectPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            if(fileName.endsWith("pdt")) {
                QFileInfo info(fileName);
                szProjName = info.baseName();
            }

        }
    }

    return szProjName;
}
