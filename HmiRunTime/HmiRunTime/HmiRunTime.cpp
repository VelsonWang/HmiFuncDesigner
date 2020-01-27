#include "HmiRunTime.h"
#include "public.h"
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
#include "VendorPluginManager.h"
#include "ComPort.h"
#include "NetPort.h"
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

void HmiRunTime::AddPortName(const QString name)
{
    foreach (QString port, m_listPortName) {
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

        qDebug()<< "Protocol: " <<sProtocol
                << " PortType:" << sPortType
                << " DeviceName:" << sDeviceName;

        Vendor *pVendorObj = new Vendor();
        IVendorPlugin *pVendorPluginObj = VendorPluginManager::getInstance()->getPlugin(sProtocol);
        if(pVendorPluginObj != Q_NULLPTR) {
            pVendorObj->m_pVendorPluginObj = pVendorPluginObj;
        }
        m_VendorList.append(pVendorObj);

        if(sPortType == "COM") {
            ComPort* pComPortObj = new ComPort();
            pVendorObj->m_pPortObj = pComPortObj;
            ComDevicePrivate *pComDevicePrivateObj = new ComDevicePrivate();
            if (pComDevicePrivateObj->LoadData(sDeviceName)) {
                QStringList comArgs;
                comArgs << QString().number(pComDevicePrivateObj->m_iBaudrate);
                comArgs << QString().number(pComDevicePrivateObj->m_iDatabit);
                comArgs << pComDevicePrivateObj->m_sVerifybit;
                comArgs << QString().number(pComDevicePrivateObj->m_fStopbit);

                if(!pComPortObj->open(pComDevicePrivateObj->m_sPortNumber, comArgs)) {
                    LogError("ComPort open fail!");
                }
            }
            pVendorObj->m_pVendorPrivateObj = pComDevicePrivateObj;
        } else if(sPortType == "NET") {
            NetPort* pNetPortObj = new NetPort();
            pVendorObj->m_pPortObj = pNetPortObj;
            NetDevicePrivate* pNetDevicePrivateObj = new NetDevicePrivate();
            if (pNetDevicePrivateObj->LoadData(sDeviceName)) {
                QStringList netArgs;
                netArgs << pNetDevicePrivateObj->m_sIpAddress;
                netArgs << QString().number(pNetDevicePrivateObj->m_iPort);

                if(!pNetPortObj->open("Net", netArgs)) {
                    Log4Qt::Logger::logger("Run_Logger")->error("NetPort open fail!");
                    LogError("NetPort open fail!");
                }
            }
            pVendorObj->m_pVendorPrivateObj = pNetDevicePrivateObj;
        }
    }

    /////////////////////////////////////////////

    // 查找已使用的端口名称并添加至列表
    foreach (Vendor *pVendor, m_VendorList) {
        AddPortName(pVendor->getPortName());
    }

    /////////////////////////////////////////////

    // load tags and create rtdb
    //-----------------系统变量------------------//
    TagSys &tagSys = ProjectData::getInstance()->tagSys_;
    tagSys.load(ProjectData::getInstance()->dbData_);

    foreach (TagSysDBItem * itemTagSys, tagSys.listTagSysDBItem_) {
        SysDataTag *pSysTag = new SysDataTag();
        pSysTag->mId = itemTagSys->m_szTagID;
        pSysTag->mType = TYPE_STRING;
        pSysTag->mName = itemTagSys->m_szName;
        pSysTag->mDescription = itemTagSys->m_szDescription;
        pSysTag->mUnit = itemTagSys->m_szUnit;
        pSysTag->mProjectConverter = itemTagSys->m_szProjectConverter;
        pSysTag->mComments = itemTagSys->m_szComments;
        pSysTag->m_Function.LoadFuncObjects(pSysTag->mProjectConverter);

        DBTagObject* pDBSysTagObject = new DBTagObject(pSysTag->mId, pSysTag->mType, READ_WRIE, 0,
                                                       QVariant(), TYPE_SYSTEM, pSysTag);
        RealTimeDB::rtdb.insert(pSysTag->mId, pDBSysTagObject);
        RealTimeDB::varNameMapId.insert((QObject::tr("系统变量.") + pSysTag->mName + "[" + pSysTag->mId + "]"), pSysTag->mId);

        pDBSysTagObject->addListener(new DBTagObject_Event_Listener());
    }

    qDeleteAll(tagSys.listTagSysDBItem_);
    tagSys.listTagSysDBItem_.clear();

    //-----------------中间变量------------------//
    TagTmp &tagTmp = ProjectData::getInstance()->tagTmp_;
    tagTmp.load(ProjectData::getInstance()->dbData_);

    foreach (TagTmpDBItem * itemTagTmp, tagTmp.listTagTmpDBItem_) {
        TmpDataTag *pTmpTag = new TmpDataTag();
        pTmpTag->mId = itemTagTmp->m_szTagID;
        pTmpTag->mType = TYPE_STRING;
        pTmpTag->mName = itemTagTmp->m_szName;
        pTmpTag->mDescription = itemTagTmp->m_szDescription;
        pTmpTag->mUnit = "";
        pTmpTag->mProjectConverter = itemTagTmp->m_szProjectConverter;
        pTmpTag->mComments = "";
        pTmpTag->m_Function.LoadFuncObjects(pTmpTag->mProjectConverter);

        QString tmp = itemTagTmp->m_szMaxVal;
        if(tmp == "") pTmpTag->mMaxValue = 0;
        else pTmpTag->mMaxValue = tmp.toDouble();

        tmp = itemTagTmp->m_szMinVal;
        if(tmp == "") pTmpTag->mMinValue = 0;
        else pTmpTag->mMinValue = tmp.toDouble();

        tmp = itemTagTmp->m_szInitVal;
        if(tmp == "") pTmpTag->mInitializeValue = 0;
        else pTmpTag->mInitializeValue = tmp.toDouble();

        DBTagObject* pDBTmpTagObject = new DBTagObject(pTmpTag->mId, pTmpTag->mType, READ_WRIE, 0,
                                                       QVariant(), TYPE_TMP, pTmpTag);
        RealTimeDB::rtdb.insert(pTmpTag->mId, pDBTmpTagObject);
        RealTimeDB::varNameMapId.insert((QObject::tr("中间变量.") + pTmpTag->mName + "[" + pTmpTag->mId + "]"), pTmpTag->mId);

        pDBTmpTagObject->addListener(new DBTagObject_Event_Listener());
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
            if(szGroup != itemTagIO->m_szGroupName)
                continue;
            QString sTableName = itemTagIOGroup->m_szShowName;

            IoDataTag *pIoDataTag = new IoDataTag();

            pIoDataTag->mId = itemTagIO->m_szTagID;
            pIoDataTag->mName = itemTagIO->m_szName;
            pIoDataTag->mDescription = itemTagIO->m_szDescription;
            pIoDataTag->mUnit = "";
            pIoDataTag->mProjectConverter = itemTagIO->m_szProjectConverter;
            pIoDataTag->mComments ="";
            pIoDataTag->mDeviceName = itemTagIO->m_szDeviceName;

            QString tmp = itemTagIO->m_szReadWriteType;
            pIoDataTag->mPermissionType = READ_WRIE;
            if(tmp == QObject::tr("只读")) {
                pIoDataTag->mPermissionType = READ;
            } else if(tmp == QObject::tr("只写")) {
                pIoDataTag->mPermissionType = WRIE;
            } else if(tmp == QObject::tr("读写")) {
                pIoDataTag->mPermissionType = READ_WRIE;
            }

            tmp = itemTagIO->m_szDeviceAddr;
            pIoDataTag->mDeviceAddress = tmp.toInt();

            tmp = itemTagIO->m_szMaxVal;
            pIoDataTag->mMaxValue = tmp.toDouble();

            pIoDataTag->mRegisterArea = itemTagIO->m_szRegisterArea;

            tmp = itemTagIO->m_szMinVal;
            pIoDataTag->mMinValue = tmp.toDouble();

            tmp = itemTagIO->m_szRegisterAddr;
            pIoDataTag->mRegisterAddress = tmp.toInt();

            tmp = itemTagIO->m_szInitVal;
            pIoDataTag->mInitializeValue = tmp.toDouble();

            tmp = itemTagIO->m_szDataType;
            if(tmp == "Bit1开关量") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_BOOL;
                pIoDataTag->mLength = 1;
            } else if(tmp == "Char8位有符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_INT8;
                pIoDataTag->mLength = 1;
            } else if(tmp == "Byte8位无符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_UINT8;
                pIoDataTag->mLength = 1;
            } else if(tmp == "Short16位有符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_INT16;
                pIoDataTag->mLength = 2;
            } else if(tmp == "Word16位无符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_UINT16;
                pIoDataTag->mLength = 2;
            } else if(tmp == "ASCII2个字符") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_ASCII2CHAR;
                pIoDataTag->mLength = 2;
            } else if(tmp == "Long32位有符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_INT32;
                pIoDataTag->mLength = 4;
            } else if(tmp == "Dword32位无符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_UINT32;
                pIoDataTag->mLength = 4;
            } else if(tmp == "Float单精度浮点数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_FLOAT;
                pIoDataTag->mLength = 4;
            } else if(tmp == "String字符串") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_STRING;
                pIoDataTag->mLength = 256;
            } else if(tmp == "Double双精度浮点数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_DOUBLE;
                pIoDataTag->mLength = 8;
            } else if(tmp == "BCD") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_BCD;
                pIoDataTag->mLength = 8;
            } else if(tmp == "LongLong64位有符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_INT64;
                pIoDataTag->mLength = 8;
            } else if(tmp == "DwordDword64位无符号数") {
                pIoDataTag->mType = pIoDataTag->mDataType = TYPE_UINT64;
                pIoDataTag->mLength = 8;
            }

            tmp = itemTagIO->m_szScale;
            pIoDataTag->mScale = tmp.toDouble();

            tmp = itemTagIO->m_szAddrOffset;
            pIoDataTag->mOffset = tmp.toInt();

            pIoDataTag->m_Function.LoadFuncObjects(itemTagIO->m_szProjectConverter);

            DBTagObject* pDBIoTagObject = new DBTagObject(pIoDataTag->mId, pIoDataTag->mType, READ_WRIE, pIoDataTag->mLength,
                                                          QVariant(), TYPE_IO, pIoDataTag);
            RealTimeDB::rtdb.insert(pIoDataTag->mId, pDBIoTagObject);
            RealTimeDB::varNameMapId.insert((QObject::tr("设备变量.") + pIoDataTag->mName + "[" + pIoDataTag->mId + "]"), pIoDataTag->mId);
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
            int bufLen = (pIoDataTag->mLength > 8) ? pIoDataTag->mLength : 8;
            pIOTag->SetTagBufferLength(bufLen);
            pIOTag->SetDBTagObject(pDBIoTagObject);

            Vendor *pVendor = FindVendor(pIoDataTag->mDeviceName);
            if(pVendor != Q_NULLPTR) {
                pVendor->addIOTagToDeviceTagList(pIOTag);
                pDBIoTagObject->pVendor = pVendor;
            }

            pDBIoTagObject->addListener(new DBTagObject_Event_Listener());
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
        foreach (Vendor *pVendor, m_VendorList)
        {
            if(name == pVendor->getPortName())
                pPortThread->AddVendor(pVendor);
        }
        m_listPortThread.append(pPortThread);
    }

    // 运行启动运行脚本
    if(m_pRunScript == Q_NULLPTR)
        m_pRunScript = new RunScript(m_sProjectPath);
    m_pRunScript->runOnStartScripts();

    foreach (PortThread *pPortThread, m_listPortThread)
    {
        pPortThread->Start();
    }

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

    foreach (PortThread *pPortThread, m_listPortThread)
    {
        pPortThread->Stop();
    }
}


Vendor *HmiRunTime::FindVendor(const QString name)
{
    Vendor *ret = Q_NULLPTR;
    for (int i = 0; i < m_VendorList.size(); ++i)
    {
        ret = m_VendorList.at(i);
        if (ret->getDeviceName() == name)
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
                                const QString &szMatchEvent) {
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
