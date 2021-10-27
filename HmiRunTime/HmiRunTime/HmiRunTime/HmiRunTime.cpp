#include "HmiRunTime.h"
#include "public.h"
#include "RealTimeDB.h"
#include "RunTimeTag.h"
#include "SysRuntimeEvent.h"
#include "VendorPluginManager.h"
#include "ComPort.h"
#include "NetPort.h"
#include <QTextStream>
#include <QTextCodec>
#include <QMutexLocker>
#include "qprojectcore.h"
#include <QMessageBox>
#include <QDebug>
#include "projdata/Tag.h"



HmiRunTime *g_pHmiRunTime = NULL;
//RunScript *HmiRunTime::m_pRunScript = NULL;
QScriptEngine *HmiRunTime::scriptEngine_ = NULL;

HmiRunTime::HmiRunTime(QProjectCore *coreObj, QObject *parent)
    : QObject(parent),
      projCore(coreObj)
{
    //m_pRunScript = new RunScript("");
}

HmiRunTime::~HmiRunTime()
{
    //    if(m_pRunScript != NULL) {
    //        delete m_pRunScript;
    //        m_pRunScript = NULL;
    //    }

    if(scriptEngine_ != NULL) {
        delete scriptEngine_;
        scriptEngine_ = NULL;
    }
}

void HmiRunTime::AddPortName(const QString name)
{
    foreach(QString port, m_listPortName) {
        if(name == port) {
            return;
        }
    }

    //qDebug()<< "PortName:" << name;
    m_listPortName.append(name);
}

bool HmiRunTime::Load()
{
    qDebug() << "load devices!";
    // load devices
    m_VendorList.clear();
    DeviceInfo &deviceInfo = projCore->deviceInfo_;

    for(int i = 0; i < deviceInfo.listDeviceInfoObject_.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.listDeviceInfoObject_.at(i);
        QString sProtocol = pObj->szProtocol_;
        QString sPortType = pObj->szDeviceType_;
        QString sDeviceName = pObj->szDeviceName_;
        qDebug() << "Protocol: " << sProtocol
                 << " PortType:" << sPortType
                 << " DeviceName:" << sDeviceName;
        Vendor *pVendorObj = new Vendor(projCore);
        IVendorPlugin *pVendorPluginObj = VendorPluginManager::getInstance()->getPlugin(sProtocol);

        if(pVendorPluginObj != NULL) {
            pVendorObj->m_pVendorPluginObj = pVendorPluginObj;
        }

        m_VendorList.append(pVendorObj);

        if(sPortType == "COM") {
            ComPort* pComPortObj = new ComPort();
            pVendorObj->m_pPortObj = pComPortObj;
            ComDevicePrivate *pComDevicePrivateObj = new ComDevicePrivate();

            if(pComDevicePrivateObj->LoadData(sDeviceName, projCore)) {
                QStringList comArgs;
                comArgs << QString().number(pComDevicePrivateObj->m_iBaudrate);
                comArgs << QString().number(pComDevicePrivateObj->m_iDatabit);
                comArgs << pComDevicePrivateObj->m_sVerifybit;
                comArgs << QString().number(pComDevicePrivateObj->m_fStopbit);

                if(!pComPortObj->open(pComDevicePrivateObj->m_sPortNumber, comArgs)) {
                    qCritical() << "ComPort open fail!";
                }
            }

            pVendorObj->m_pVendorPrivateObj = pComDevicePrivateObj;
        } else if(sPortType == "NET") {
            NetDevicePrivate* pNetDevicePrivateObj = new NetDevicePrivate();

            if(pNetDevicePrivateObj->LoadData(sDeviceName, projCore)) {
                QStringList netArgs;
                netArgs << pNetDevicePrivateObj->m_sIpAddress;
                netArgs << QString().number(pNetDevicePrivateObj->m_iPort);
                NetPort* pNetPortObj = new NetPort(pNetDevicePrivateObj->m_sIpAddress, pNetDevicePrivateObj->m_iPort);
                pVendorObj->m_pPortObj = pNetPortObj;

                if(!pNetPortObj->open("Net", netArgs)) {
                    qCritical() << "NetPort open fail!";
                }
            }

            pVendorObj->m_pVendorPrivateObj = pNetDevicePrivateObj;
        }
    }

    /////////////////////////////////////////////

    // 查找已使用的端口名称并添加至列表
    foreach(Vendor *pVendor, m_VendorList) {
        AddPortName(pVendor->getPortName());
    }

    /////////////////////////////////////////////

    // load tags and create rtdb
    foreach(Tag *pTagObj, projCore->tagMgr_.m_vecTags) {
        RunTimeTag *pRtTagObj = new RunTimeTag(NULL);
        pRtTagObj->id = pTagObj->m_iID;
        pRtTagObj->name = pTagObj->m_szName;
        pRtTagObj->unit = pTagObj->m_szUnit;
        pRtTagObj->addrType = pTagObj->m_szAddrType;
        pRtTagObj->addrOffset = pTagObj->m_szAddrOffset.toInt();
        pRtTagObj->aAddrType2 = pTagObj->m_szAddrType2;
        pRtTagObj->addrOffset2 = pTagObj->m_szAddrOffset2.toInt();
        QString szDataType = pTagObj->m_szDataType;
        pRtTagObj->writeable = (TWriteAble)pTagObj->m_iWriteable;
        pRtTagObj->remark = pTagObj->m_szRemark;
        pRtTagObj->ownGroup = pTagObj->m_szOwnGroup;
        pRtTagObj->devType = pTagObj->m_szDevType;
        pRtTagObj->dataType = TYPE_VARIANT;
        if(szDataType == "bool") {
            pRtTagObj->dataType = TYPE_BOOL;
            pRtTagObj->bufLength = 1;
        } else if(szDataType == "int8") {
            pRtTagObj->dataType = TYPE_INT8;
            pRtTagObj->bufLength = 1;
        } else if(szDataType == "uint8") {
            pRtTagObj->dataType = TYPE_UINT8;
            pRtTagObj->bufLength = 1;
        } else if(szDataType == "int16") {
            pRtTagObj->dataType = TYPE_INT16;
            pRtTagObj->bufLength = 2;
        } else if(szDataType == "uint16") {
            pRtTagObj->dataType = TYPE_UINT16;
            pRtTagObj->bufLength = 2;
        } else if(szDataType == "int32") {
            pRtTagObj->dataType = TYPE_INT32;
            pRtTagObj->bufLength = 4;
        } else if(szDataType == "uint32") {
            pRtTagObj->dataType = TYPE_UINT32;
            pRtTagObj->bufLength = 4;
        } else if(szDataType == "int64") {
            pRtTagObj->dataType = TYPE_INT64;
            pRtTagObj->bufLength = 8;
        } else if(szDataType == "uint64") {
            pRtTagObj->dataType = TYPE_UINT64;
            pRtTagObj->bufLength = 8;
        } else if(szDataType == "float32") {
            pRtTagObj->dataType = TYPE_FLOAT32;
            pRtTagObj->bufLength = 4;
        } else if(szDataType == "float64") {
            pRtTagObj->dataType = TYPE_FLOAT64;
            pRtTagObj->bufLength = 8;
        } else if(szDataType == "bcd8") {
            pRtTagObj->dataType = TYPE_BCD8;
            pRtTagObj->bufLength = 1;
        } else if(szDataType == "bcd16") {
            pRtTagObj->dataType = TYPE_BCD16;
            pRtTagObj->bufLength = 2;
        } else if(szDataType == "bcd32") {
            pRtTagObj->dataType = TYPE_BCD32;
            pRtTagObj->bufLength = 3;
        } else if(szDataType == "ascii2char") {
            pRtTagObj->dataType = TYPE_ASCII2CHAR;
            pRtTagObj->bufLength = 2;
        } else if(szDataType == "string") {
            pRtTagObj->dataType = TYPE_STRING;
            pRtTagObj->bufLength = 256;
        } else if(szDataType == "bytes") {
            pRtTagObj->dataType = TYPE_BYTES;
            pRtTagObj->bufLength = 256;
        }

        RealTimeDB::instance()->rtdb.insert(pRtTagObj->id, pRtTagObj);

        Vendor *pVendor = FindVendor(pRtTagObj->devType);
        if(pVendor != NULL) {
            pVendor->addIOTagToDeviceTagList(pRtTagObj);
        }
    }

    //-----------------加载JavaScript------------------//
    //    if(m_pRunScript == NULL) {
    //        m_pRunScript = new RunScript("");
    //    }
    //    m_pRunScript->loadScriptObjects();

    return true;
}


bool HmiRunTime::Unload()
{
    qDeleteAll(m_VendorList);
    m_VendorList.clear();
    RealTimeDB::instance()->rtdb.clear();
    qDeleteAll(m_listPortThread);
    m_listPortThread.clear();
    return true;
}

void HmiRunTime::Start()
{
    foreach(QString name, m_listPortName) {
        PortThread *pPortThread = new PortThread(name);

        foreach(Vendor *pVendor, m_VendorList) {
            if(name == pVendor->getPortName()) {
                pPortThread->AddVendor(pVendor);
            }
        }

        m_listPortThread.append(pPortThread);
    }

    foreach(PortThread *pPortThread, m_listPortThread) {
        pPortThread->Start();
    }

    // 运行启动运行脚本
    //    if(m_pRunScript == NULL) {
    //        m_pRunScript = new RunScript("");
    //    }

    //    m_pRunScript->runOnStartScripts();

    //    // 运行定时运行脚本
    //    if(m_pRunScript == NULL) {
    //        m_pRunScript = new RunScript("");
    //    }

    //    m_pRunScript->runOnPeriodScripts();
}

void HmiRunTime::Stop()
{
    foreach(PortThread *pPortThread, m_listPortThread) {
        pPortThread->Stop();
    }

    //m_pRunScript->stopRunOnPeriodScripts();
}

Vendor *HmiRunTime::FindVendor(const QString name)
{
    for(int i = 0; i < m_VendorList.size(); ++i) {
        Vendor *pObj = m_VendorList.at(i);

        if(pObj->getDeviceName() == name) {
            return pObj;
        }
    }

    return NULL;
}


QJsonObject HmiRunTime::LoadJsonObjectFromFile(SaveFormat saveFormat, QString f)
{
    QFile loadFile(f);

    if(!loadFile.open(QIODevice::ReadOnly)) {
        return QJsonObject();
    }

    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(saveFormat == Json ? QJsonDocument::fromJson(loadData) : QJsonDocument::fromBinaryData(loadData));
    loadFile.close();
    return loadDoc.object();
}


bool HmiRunTime::event(QEvent *event)
{
    if(event->type() == EV_StartRuntime) {
        Load();
        Start();
        qDebug() << "start runtime.";
        return false;
    } else if(event->type() == EV_StopRuntime) {
        qDebug() << "stop runtime.";
        Stop();
        Unload();
        return false;
    } else if(event->type() == EV_RestartRuntime) {
        qDebug() << "restart runtime.";
        Stop();
        Unload();
        Load();
        Start();
        return false;
    }

    return QObject::event(event);
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

    if(srcFileInfo.isDir()) {
        QDir sourceDir(szProjectPath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

        foreach(const QString &fileName, fileNames) {
            if(fileName.endsWith("pdt")) {
                QFileInfo info(fileName);
                szProjName = info.baseName();
            }
        }
    }

    return szProjName;
}

/**
 * @brief HmiRunTime::doMessage
 * @details 处理消息
 * @param msg
 */
void HmiRunTime::doMessage(QString msg)
{
    if(msg.indexOf(QString("VALUE_CHANGE")) != -1) {
        QString tagId = msg.replace(QString("VALUE_CHANGE "), QString(""));

        //        // 运行条件运行脚本
        //        if(m_pRunScript == NULL) {
        //            m_pRunScript = new RunScript("");
        //        }

        //        m_pRunScript->runOnConditionScripts(tagId);
    }
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
    if(scriptEngine_ != NULL && szFuncList.size() > 0 && szMatchEvent != "") {
        foreach(QString szFuncEv, szFuncList) {
            QStringList listFuncEv = szFuncEv.split(':');

            if(listFuncEv.size() == 2) {
                QString szFunc = listFuncEv.at(0);
                QString szEv = listFuncEv.at(1);

                if(szEv == szMatchEvent) {
                    //                    QScriptValue result = scriptEngine_->evaluate(szFunc);

                    //                    if(result.isError()) {
                    //                        QString err = QString::fromLatin1("File:%1 Function:%2 script syntax evaluate error: %3")
                    //                                      .arg(__FILE__)
                    //                                      .arg(__FUNCTION__)
                    //                                      .arg(result.toString());
                    //                        qCritical() << err;
                    //                        return;
                    //                    }
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
    if(scriptEngine_ != NULL && szScriptText != "" && szMatchEvent != "") {
        QStringList listFuncEv = szScriptText.split("][");

        if(listFuncEv.size() == 2) {
            QString szEv = listFuncEv.at(0);
            QString szScript = listFuncEv.at(1);

            if(szEv == szMatchEvent) {
                //                QScriptValue result = scriptEngine_->evaluate(szScript);

                //                if(result.isError()) {
                //                    QString err = QString::fromLatin1("File:%1 Function:%2 script syntax evaluate error: %3")
                //                                  .arg(__FILE__)
                //                                  .arg(__FUNCTION__)
                //                                  .arg(result.toString());
                //                    qCritical() << err;
                //                    return;
                //                }
            }
        }
    }
}




