#include "HmiRunTime.h"
#include "public.h"
#include "realtimedb.h"
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



HmiRunTime::HmiRunTime(QObject *parent)
    : QObject(parent),
      projCore(NULL)
{

}

HmiRunTime::~HmiRunTime()
{

}

void HmiRunTime::AddPortName(const QString name)
{
    foreach(QString port, m_listPortName) {
        if(name == port) {
            return;
        }
    }
    m_listPortName.append(name);
}

RunTimeTag *HmiRunTime::createRunTimeTag(Tag *pTagObj)
{
    RunTimeTag *pRtTagObj = new RunTimeTag(NULL);
    pRtTagObj->id = pTagObj->m_iID;
    pRtTagObj->blockReadID = pTagObj->m_iBlockReadID;
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

    DeviceInfo &deviceInfo = projCore->m_deviceInfo;
    DeviceInfoObject *pDevObj = deviceInfo.getObjectByName(pTagObj->m_szDevType);
    if(pDevObj) {
        pRtTagObj->devType = pDevObj->m_name;
    }

    pRtTagObj->dataType = TYPE_VARIANT;
    pRtTagObj->bufLength = 1;

    qDebug() << "szDataType:" << szDataType;
    QStringList listBlockRead;
    if(pRtTagObj->blockReadID < 1) {
        // 块读变量 寄存器个数：数据类型
        if(szDataType.indexOf(":") > -1) {
            listBlockRead = szDataType.split(":");
            if(listBlockRead.count() == 2) {
                szDataType = listBlockRead[1];
            }
        }
    }

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

    // 块读变量
    if(listBlockRead.count() == 2) {
        pRtTagObj->dataType = TYPE_BYTES;
        int num = listBlockRead[0].toInt();
        pRtTagObj->bufLength = pRtTagObj->bufLength * num;
        pRtTagObj->isBlockRead = true;
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "num: " << num << ", bufLength: " << pRtTagObj->bufLength;
    }

    pRtTagObj->dataFromVendor = new quint8[pRtTagObj->bufLength];
    memset((void *)pRtTagObj->dataFromVendor, 0, pRtTagObj->bufLength);
    pRtTagObj->dataToVendor = new quint8[pRtTagObj->bufLength + 1]; // 最后一个字节标记需要同步数据至PLC等设备
    memset((void *)pRtTagObj->dataToVendor, 0, pRtTagObj->bufLength + 1);

    return pRtTagObj;
}

bool HmiRunTime::Load()
{
    qDebug() << "load devices!";
    // load devices
    m_vendors.clear();
    DeviceInfo &deviceInfo = projCore->m_deviceInfo;

    for(int i = 0; i < deviceInfo.m_deviceInfoObject.count(); i++) {
        DeviceInfoObject *pObj = deviceInfo.m_deviceInfoObject.at(i);
        QString szProtocol = pObj->m_protocol;
        QString szPortType = pObj->m_deviceType;
        QString szDeviceName = pObj->m_name;
        QString szVendorName = pObj->m_deviceName;
        qDebug() << "Protocol: " << szProtocol
                 << ", PortType:" << szPortType
                 << ", DeviceName:" << szDeviceName
                 << ", VendorName:" << szVendorName;
        Vendor *pVendorObj = new Vendor(projCore);
        IVendorPlugin *pVendorPluginObj = VendorPluginManager::getInstance()->getPlugin(szProtocol);

        if(pVendorPluginObj != NULL) {
            pVendorObj->m_pVendorPluginObj = pVendorPluginObj;
        }

        m_vendors.append(pVendorObj);

        if(szPortType == "COM") {
            ComPort* pComPortObj = new ComPort();
            pVendorObj->m_pPortObj = pComPortObj;
            ComDevicePrivate *pComDevicePrivateObj = new ComDevicePrivate();

            if(pComDevicePrivateObj->LoadData(szDeviceName, projCore)) {
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
        } else if(szPortType == "NET") {
            NetDevicePrivate* pNetDevicePrivateObj = new NetDevicePrivate();

            if(pNetDevicePrivateObj->LoadData(szDeviceName, projCore)) {
                QStringList netArgs;
                netArgs << pNetDevicePrivateObj->m_sIpAddress;
                netArgs << QString().number(pNetDevicePrivateObj->m_iPort);
                NetPort* pNetPortObj = new NetPort();
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
    foreach(Vendor *pVendor, m_vendors) {
        AddPortName(pVendor->getPortName());
    }

    /////////////////////////////////////////////

    // load block read tags and create rtdb
    foreach (QString dev, projCore->m_tagMgr.m_mapDevBlockReadTags.keys()) {
        Vendor *pVendor = FindVendor(dev);
        if(pVendor != NULL) {
            foreach(Tag *pTagObj, projCore->m_tagMgr.m_mapDevBlockReadTags[dev]) {
                RunTimeTag *pRtTagObj = createRunTimeTag(pTagObj);
                if(pRtTagObj) {
                    RealTimeDB::instance()->rtdb.insert(pRtTagObj->id, pRtTagObj);
                    if(pVendor->m_pVendorPluginObj) {
                        pVendor->m_pVendorPluginObj->setBlockReadTagBufferLength(pRtTagObj);
                    }
                    pVendor->addIOTagToDeviceTagList(pRtTagObj);
                }
            }
        }
    }
    foreach (QString dev, projCore->m_tagMgr.m_mapDevBlockReadTags.keys()) {
        qDeleteAll(projCore->m_tagMgr.m_mapDevBlockReadTags[dev]);
        projCore->m_tagMgr.m_mapDevBlockReadTags[dev].clear();
    }
    projCore->m_tagMgr.m_mapDevBlockReadTags.clear();

    // load tags and create rtdb
    foreach(Tag *pTagObj, projCore->m_tagMgr.m_vecTags) {
        RunTimeTag *pRtTagObj = createRunTimeTag(pTagObj);
        if(pRtTagObj) {
            RealTimeDB::instance()->rtdb.insert(pRtTagObj->id, pRtTagObj);
            Vendor *pVendor = FindVendor(pRtTagObj->devType);
            if(pVendor != NULL) {
                pVendor->addIOTagToDeviceTagList(pRtTagObj);
            }
        }
    }
    qDeleteAll(projCore->m_tagMgr.m_vecTags);
    projCore->m_tagMgr.m_vecTags.clear();

    return true;
}


bool HmiRunTime::Unload()
{
    qDeleteAll(m_vendors);
    m_vendors.clear();
    qDeleteAll(m_listPortThread);
    m_listPortThread.clear();
    RealTimeDB::instance()->releaseTags();
    return true;
}

void HmiRunTime::Start()
{
    foreach(QString name, m_listPortName) {
        PortThread *pPortThread = new PortThread(name);

        foreach(Vendor *pVendor, m_vendors) {
            if(name == pVendor->getPortName()) {
                pPortThread->AddVendor(pVendor);
            }
        }

        m_listPortThread.append(pPortThread);
    }

    foreach(PortThread *pPortThread, m_listPortThread) {
        pPortThread->Start();
    }
}

void HmiRunTime::Stop()
{
    foreach(PortThread *pPortThread, m_listPortThread) {
        pPortThread->Stop();
    }
}

Vendor *HmiRunTime::FindVendor(const QString name)
{
    for(int i = 0; i < m_vendors.size(); ++i) {
        Vendor *pObj = m_vendors.at(i);
        if(pObj->getDeviceName() == name) {
            return pObj;
        }
    }

    return NULL;
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
