#include "HmiRunTime.h"
#include "public.h"
#include "ModbusRTUDevice.h"
#include "ModbusASCIIDevice.h"
#include "TCPIPModbusDevice.h"
#include "MitsubishiDevice.h"
#include "DBTagObject.h"
#include "RealTimeDB.h"
#include "Tag.h"
#include "IOTag.h"
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

HmiRunTime *g_pHmiRunTime = nullptr;
QString HmiRunTime::m_sProjectPath = QString("");
RunScript *HmiRunTime::m_pRunScript = nullptr;
QScriptEngine *HmiRunTime::scriptEngine_ = nullptr;


HmiRunTime::HmiRunTime(QString projectPath, QObject *parent)
    : QObject(parent)
{
    m_sProjectPath = projectPath;
    m_pRunScript = new RunScript(projectPath);
}

HmiRunTime::~HmiRunTime()
{
    if(m_pRunScript != nullptr) {
        delete m_pRunScript;
        m_pRunScript = nullptr;
    }
    if(scriptEngine_ != nullptr) {
        delete scriptEngine_;
        scriptEngine_ = nullptr;
    }
}

void HmiRunTime::AddPortName(const QString name)
{
    foreach (QString port, m_listPortName)
    {
        if(name == port)
            return;
    }
    //qDebug()<< "PortName:" << name;
    m_listPortName.append(name);
}

bool HmiRunTime::Load(SaveFormat saveFormat)
{
    if(m_sProjectPath == "")
        return false;

    QDir projDir(m_sProjectPath);
    if(!projDir.exists())
        return false;

    //qDebug() << "load devices!";

    // load devices
    m_VendorList.clear();

    QString szProjName = getProjectName(m_sProjectPath);

    if(szProjName == "") {
        LogError("project information file not found!");
        return false;
    }

    ProjectData::getInstance()->createOrOpenProjectData(m_sProjectPath, szProjName);
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    for(int i=0; i<deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        QString sProtocol = pObj->szProtocol_;
        QString sPortType = pObj->szDeviceType_;
        QString sDeviceName = pObj->szDeviceName_;

        qDebug()<< "Protocol is" <<sProtocol.toUpper();
        if(sProtocol.toUpper() == QString("MODBUSRTU"))
        {
            ModbusRTUDevice *pMbRTUDevice = new ModbusRTUDevice();
            pMbRTUDevice->LoadData(sDeviceName);
            m_VendorList.append(pMbRTUDevice);
        }
        else if(sProtocol.toUpper() == QString("MODBUSASCII"))
        {
            ModbusASCIIDevice *pMbAsciiDevice = new ModbusASCIIDevice();
            pMbAsciiDevice->LoadData(sDeviceName);
            m_VendorList.append(pMbAsciiDevice);           
        }
        else if(sProtocol.toUpper() == QString("TCPIPMODBUS"))
        {
            TCPIPModbusDevice *pMbTcpipDevice = new TCPIPModbusDevice();
            pMbTcpipDevice->LoadData(sDeviceName);
            m_VendorList.append(pMbTcpipDevice);
        }
        else if(sProtocol.toUpper() == QString("FXPROTOCOL"))
        {
            MitsubishiDevice *pMitsubishiDevice = new MitsubishiDevice();
            pMitsubishiDevice->LoadData(sDeviceName);
            m_VendorList.append(pMitsubishiDevice);
        }
    }

    /////////////////////////////////////////////

    // 查找已使用的端口名称并添加至列表
    foreach (IVendor *pVendor, m_VendorList)
    {
        AddPortName(pVendor->GetPortName());
    }

    /////////////////////////////////////////////

    // load tags and create rtdb
    //-----------------系统变量------------------//
    QJsonObject jsonSysTags = LoadJsonObjectFromFile(saveFormat, m_sProjectPath + "/SysVarList.odb");
    if(jsonSysTags != QJsonObject())
    {
        QJsonArray tagSysArray = jsonSysTags["SysVarArray"].toArray();
        for (int i = 0; i < tagSysArray.size(); ++i)
        {
            QJsonObject jsonObj = tagSysArray[i].toObject();
            SysDataTag *pSysTag = new SysDataTag();
            pSysTag->LoadData(jsonObj);
            DBTagObject* pDBSysTagObject = new DBTagObject(pSysTag->mId, pSysTag->mType, READ_WRIE, 0,
                                                        QVariant(), TYPE_SYSTEM, pSysTag);
            RealTimeDB::rtdb.insert(pSysTag->mId, pDBSysTagObject);
            RealTimeDB::varNameMapId.insert(("系统变量." + pSysTag->mName), pSysTag->mId);

            pDBSysTagObject->addListener(new DBTagObject_Event_Listener());
        }
    }

    //-----------------中间变量------------------//
    QJsonObject jsonTmpTags = LoadJsonObjectFromFile(saveFormat, m_sProjectPath + "/TmpVarList.odb");
    if(jsonTmpTags != QJsonObject())
    {
        QJsonArray TmpVarArray = jsonTmpTags["TmpVarArray"].toArray();
        for (int i = 0; i < TmpVarArray.size(); ++i)
        {
            QJsonObject jsonObj = TmpVarArray[i].toObject();
            TmpDataTag *pTmpTag = new TmpDataTag();
            pTmpTag->LoadData(jsonObj);
            DBTagObject* pDBTmpTagObject = new DBTagObject(pTmpTag->mId, pTmpTag->mType, READ_WRIE, 0,
                                                        QVariant(), TYPE_TMP, pTmpTag);
            RealTimeDB::rtdb.insert(pTmpTag->mId, pDBTmpTagObject);
            RealTimeDB::varNameMapId.insert(("中间变量." + pTmpTag->mName), pTmpTag->mId);

            pDBTmpTagObject->addListener(new DBTagObject_Event_Listener());
        }
    }

    //-----------------设备变量------------------//
    QJsonObject jsonDBVarList = LoadJsonObjectFromFile(saveFormat, m_sProjectPath + "/DBVarList.odb");
    if(jsonDBVarList != QJsonObject())
    {
        QJsonArray DevVarTabArray = jsonDBVarList["DevVarTabArray"].toArray();
        //qDebug() << "DevVarTabArray.size() " << DevVarTabArray.size();
        for (int i = 0; i < DevVarTabArray.size(); ++i)
        {
            QJsonObject jsonObj = DevVarTabArray[i].toObject();
            QJsonObject jsonIoTags = LoadJsonObjectFromFile(saveFormat, m_sProjectPath + "/DevVarList-" + jsonObj["name"].toString()+ ".odb");
            if(jsonIoTags == QJsonObject())
                return false;
            QString devVarTabName = jsonObj["name"].toString();
            QJsonArray IOVarArray = jsonIoTags["IOVarArray"].toArray();
            //qDebug() << "IOVarArray.size() " << IOVarArray.size();
            for (int i = 0; i < IOVarArray.size(); ++i)
            {
                QJsonObject jsonObj = IOVarArray[i].toObject();
                IoDataTag *pIoDataTag = new IoDataTag();
                pIoDataTag->LoadData(jsonObj);
                DBTagObject* pDBIoTagObject = new DBTagObject(pIoDataTag->mId, pIoDataTag->mType, READ_WRIE, pIoDataTag->mLength,
                                                                    QVariant(), TYPE_IO, pIoDataTag);
                RealTimeDB::rtdb.insert(pIoDataTag->mId, pDBIoTagObject);
                RealTimeDB::varNameMapId.insert(("设备变量." + devVarTabName + "." + pIoDataTag->mName), pIoDataTag->mId);
                IOTag *pIOTag = new IOTag();
                pIOTag->SetTagID(pIoDataTag->mId);
                pIOTag->SetDeviceName(pIoDataTag->mDeviceName);
                pIOTag->SetPermissionType(pIoDataTag->mPermissionType);
                pIOTag->SetDeviceAddress(pIoDataTag->mDeviceAddress);
                pIOTag->SetRegisterArea(pIoDataTag->mRegisterArea);
                pIOTag->SetRegisterAddress(pIoDataTag->mRegisterAddress);
                pIOTag->SetDataType(pIoDataTag->mDataType);
                pIOTag->SetOffset(pIoDataTag->mOffset);
                pIOTag->SetMaxValue(pIoDataTag->mMaxValue);
                pIOTag->SetMinValue(pIoDataTag->mMinValue);
                pIOTag->SetInitializeValue(pIoDataTag->mInitializeValue);
                pIOTag->SetScale(pIoDataTag->mScale);
                pIOTag->SetInFrameAddress(pIoDataTag->mInFrameAddress);
                int bufLen = (pIoDataTag->mLength > 8) ? pIoDataTag->mLength : 8;
                pIOTag->SetTagBufferLength(bufLen);
                pIOTag->SetDBTagObject(pDBIoTagObject);

                IVendor *pVendor = FindVendor(pIoDataTag->mDeviceName);
                if(pVendor != Q_NULLPTR)
                {
                    pVendor->AddIOTagToDeviceTagList(pIOTag);
                    pDBIoTagObject->pVendor = pVendor;
                }

                pDBIoTagObject->addListener(new DBTagObject_Event_Listener());
            }
        }
    }

    //-----------------加载JavaScript------------------//
    if(m_pRunScript == nullptr)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->loadScriptObjects();

//    RealTimeDB::debugShowNameMapId();

    return true;
}


bool HmiRunTime::Unload()
{
    qDeleteAll(m_VendorList);
    m_VendorList.clear();

    qDeleteAll(RealTimeDB::rtdb);
    RealTimeDB::rtdb.clear();

    qDeleteAll(m_listPortThread);
    m_listPortThread.clear();

    MainWindow::instance()->unLoadGraphPages();

    return true;
}


void HmiRunTime::Start()
{ 
    foreach (QString name, m_listPortName)
    {
        PortThread *pPortThread = new PortThread(name);
        foreach (IVendor *pVendor, m_VendorList)
        {
            if(name == pVendor->GetPortName())
                pPortThread->AddVendor(pVendor);
        }
        m_listPortThread.append(pPortThread);
    }

    // 运行启动运行脚本
    if(m_pRunScript == nullptr)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->runOnStartScripts();

    foreach (PortThread *pPortThread, m_listPortThread)
    {
        pPortThread->Start();
    }

    // 运行定时运行脚本
    if(m_pRunScript == nullptr)
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

    foreach (PortThread *pPortThread, m_listPortThread)
    {
        pPortThread->Stop();
    }
}


IVendor *HmiRunTime::FindVendor(const QString name)
{
    IVendor *ret = NULL;
    for (int i = 0; i < m_VendorList.size(); ++i)
    {
        ret = m_VendorList.at(i);
        if (ret->GetDeviceName() == name)
            break;
    }
    return ret;
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

    if(event->type() == EV_StartRuntime)
    {
        Load(DATA_SAVE_FORMAT);
        Start();
        log->debug("start runtime.");
        qDebug() << "start runtime.";
        return false;
    }
    else if(event->type() == EV_StopRuntime)
    {
        log->debug("stop runtime.");
        qDebug() << "stop runtime.";
        Stop();
        Unload();
        return false;
    }
    else if(event->type() == EV_RestartRuntime)
    {
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
                                      const QString &szMatchEvent) {
    if(scriptEngine_ != nullptr) {
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
 * @brief HmiRunTime::getProjectName
 * @details 获取工程名称
 * @param szProjectPath
 * @return
 */
QString HmiRunTime::getProjectName(const QString &szProjectPath) {
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
