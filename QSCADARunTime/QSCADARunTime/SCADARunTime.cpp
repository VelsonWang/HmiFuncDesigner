#include "SCADARunTime.h"
#include "public.h"
#include "ModbusRTUDevice.h"
#include "ModbusASCIIDevice.h"
#include "TCPIPModbusDevice.h"
#include "DBTagObject.h"
#include "RealTimeDB.h"
#include "Tag.h"
#include "IOTag.h"
#include "SysRuntimeEvent.h"
#include "log4qt/logger.h"
#include "Log.h"
#include "MainWindow.h"
#include "ProjectInfoManger.h"
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

SCADARunTime *g_SCADARunTimePtr = nullptr;
QString SCADARunTime::m_sProjectPath = QString("");
RunScript *SCADARunTime::m_pRunScript = nullptr;

SCADARunTime::SCADARunTime(QString projectPath, QObject *parent)
    : QObject(parent)
{
    m_sProjectPath = projectPath;
    m_pRunScript = new RunScript(projectPath);
}

SCADARunTime::~SCADARunTime()
{
    if(m_pRunScript != nullptr)
    {
        delete m_pRunScript;
        m_pRunScript = nullptr;
    }
}

void SCADARunTime::AddPortName(const QString name)
{
    foreach (QString port, m_listPortName)
    {
        if(name == port)
            return;
    }
    //qDebug()<< "PortName:" << name;
    m_listPortName.append(name);
}

bool SCADARunTime::Load(SaveFormat saveFormat)
{
    if(m_sProjectPath == "")
        return false;

    QDir projDir(m_sProjectPath);
    if(!projDir.exists())
        return false;

    //qDebug() << "load devices!";

    // load devices
    m_VendorList.clear();
    QJsonObject jsonDevices = LoadJsonObjectFromFile(saveFormat, m_sProjectPath + "/LinkList.odb");
    if(jsonDevices == QJsonObject())
        return false;
    QJsonArray DeviceArray = jsonDevices["DeviceArray"].toArray();
    for (int i = 0; i < DeviceArray.size(); ++i)
    {
        QJsonObject jsonObj = DeviceArray[i].toObject();
        QString sProtocol = jsonObj["prog"].toString();
        QString sPortType = jsonObj["type"].toString();
        QString sDeviceName = jsonObj["name"].toString();
        //jsonObj["bstop"] = false;
        //jsonObj["rdy"] = DevList.at(i)->m_sLink;

        qDebug()<< "Protocol is" <<sProtocol.toUpper();
        if(sProtocol.toUpper() == QString("MODBUSRTU"))
        {
            ModbusRTUDevice *pMbRTUDevice = new ModbusRTUDevice();
            QString cfgfile = m_sProjectPath + "/" + sDeviceName.trimmed() + ".odb";
            pMbRTUDevice->LoadData(DATA_SAVE_FORMAT, cfgfile);
            m_VendorList.append(pMbRTUDevice);
        }
        else if(sProtocol.toUpper() == QString("MODBUSASCII"))
        {
            ModbusASCIIDevice *pMbAsciiDevice = new ModbusASCIIDevice();
            QString cfgfile = m_sProjectPath + "/" + sDeviceName.trimmed() + ".odb";
            pMbAsciiDevice->LoadData(DATA_SAVE_FORMAT, cfgfile);
            m_VendorList.append(pMbAsciiDevice);           
        }
        else if(sProtocol.toUpper() == QString("TCPIPMODBUS"))
        {
            TCPIPModbusDevice *pMbTcpipDevice = new TCPIPModbusDevice();
            QString cfgfile = m_sProjectPath + "/" + sDeviceName.trimmed() + ".odb";
            pMbTcpipDevice->LoadData(DATA_SAVE_FORMAT, cfgfile);
            m_VendorList.append(pMbTcpipDevice);
        }
    }

    /////////////////////////////////////////////

    // 查找已使用的端口名称并添加至列表
    foreach (IVendor *pVendor, m_VendorList)
    {
        AddPortName(pVendor->GetPortName());
    }

    /////////////////////////////////////////////

    //qDebug() << "load tags and create rtdb!";

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
                pIoDataTag->LoadData(jsonObj, jsonObj["pageid"].toInt());
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
                if(pVendor != NULL)
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


bool SCADARunTime::Unload()
{
    qDeleteAll(m_VendorList);
    m_VendorList.clear();

    qDeleteAll(RealTimeDB::rtdb);
    RealTimeDB::rtdb.clear();

    qDeleteAll(m_listPortThread);
    m_listPortThread.clear();

    if(showViewWin_ != nullptr) {
        delete showViewWin_;
        showViewWin_ = nullptr;
    }

    return true;
}


void SCADARunTime::Start()
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
    QFileInfo srcFileInfo(m_sProjectPath);
    QString projInfoFile = "";
    QString projInfoFileName = "";
    if (srcFileInfo.isDir()) {
        QDir sourceDir(m_sProjectPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            if(fileName.endsWith("proj")) {
                projInfoFile = fileName;
                QFileInfo info(projInfoFile);
                projInfoFileName = info.fileName();
            }

        }
    }
    if(projInfoFile == "") {
        LogError("project information file not found!");
    } else {
        ProjectInfoManger projInfoMgr;
        projInfoMgr.loadFromFile(DATA_SAVE_FORMAT, m_sProjectPath + "/" + projInfoFile);
        QString startPageFile = projInfoMgr.getStartPage();
        if(startPageFile.toLower() != "none") {
            showViewWin_ = new MainWindow(m_sProjectPath, startPageFile);
            showViewWin_->openGraphPage(m_sProjectPath, startPageFile);
            showViewWin_->show();
        }
    }
}

void SCADARunTime::Stop()
{
    m_pRunScript->stopRunOnPeriodScripts();

    foreach (PortThread *pPortThread, m_listPortThread)
    {
        pPortThread->Stop();
    }
}


IVendor *SCADARunTime::FindVendor(const QString name)
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


QJsonObject SCADARunTime::LoadJsonObjectFromFile(SaveFormat saveFormat, QString f)
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
 * @brief SCADARunTime::doMessage
 * @details 处理消息
 * @param msg
 */
void SCADARunTime::doMessage(QString msg)
{
    if(msg.indexOf(QString("VALUE_CHANGE")) != -1)
    {
        qint32 tagId = msg.replace(QString("VALUE_CHANGE "), QString("")).toInt();
        // 运行条件运行脚本
        if(m_pRunScript == nullptr)
            m_pRunScript = new RunScript(m_sProjectPath);
        m_pRunScript->runOnConditionScripts(tagId);
    }

}


bool SCADARunTime::event(QEvent *event)
{
    Log4Qt::Logger *log = Log4Qt::Logger::logger("SCADARunTime");

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





