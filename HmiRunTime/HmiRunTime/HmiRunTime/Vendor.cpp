#include "Vendor.h"
#include <cstdlib>
#include <cstring>
#include <QFile>
#include "publicfunction.h"
#include "realtimedb.h"
#include "qprojectcore.h"
#include <QDateTime>
#include <QDebug>

VendorPrivate::VendorPrivate()
{

}



///
/// \brief VendorPrivate::parsePropertiesFromString
/// \details 从字符串解析设备私有属性
/// \param szProperty 属性字符串
/// \param properties 属性
///
void VendorPrivate::parsePropertiesFromString(const QString &szProperty, QMap<QString, QVariant> &mapProperties)
{
    mapProperties.clear();
    QStringList szListProperties = szProperty.split('|');
    foreach(QString szProp, szListProperties) {
        QStringList szListKeyVal = szProp.split('=');
        if(szListKeyVal.size() == 2) {
            mapProperties.insert(szListKeyVal.at(0), szListKeyVal.at(1));
        }
    }
    //qDebug() << mapProperties;
}

/*
* 从文件读取配置数据
*/
bool ComDevicePrivate::LoadData(const QString &devName, QProjectCore *coreObj)
{
    DeviceInfoObject *pObj = coreObj->m_deviceInfo.getObjectByName(devName);

    m_sDeviceName = pObj->m_deviceName;
    m_iFrameLen = pObj->m_frameLen;
    m_sProtocol = pObj->m_protocol;
    m_sLink = pObj->m_link;
    m_iStateVar = pObj->m_stateVar;
    m_iFrameTimePeriod = pObj->m_frameTimePeriod;
    m_iCtrlVar = pObj->m_ctrlVar;
    m_bDynamicOptimization = pObj->m_dynamicOptimization;
    m_iRemotePort = pObj->m_remotePort;
    parsePropertiesFromString(pObj->m_properties, m_mapProperties);
    m_iRetryTimes = m_mapProperties.value("commFailRetryTimes").toInt();
    m_iCommTimeout = m_mapProperties.value("commTimeout").toInt() * 1000;
    m_iCommResumeTime = m_mapProperties.value("commResumeTime").toInt() * 1000;
    m_sDeviceType = "COM";

    ComDevice comDev;
    comDev.fromString(pObj->m_portParameters);
    m_sPortNumber = comDev.m_portNumber;
    m_iBaudrate = comDev.m_baudrate;
    m_iDatabit = comDev.m_databit;
    m_fStopbit = comDev.m_stopbit;
    m_sVerifybit = comDev.m_verifybit;
    m_iTimeout = comDev.m_timeout;

    return true;
}



/*
* 从文件读取配置数据
*/
bool NetDevicePrivate::LoadData(const QString &devName, QProjectCore *coreObj)
{
    DeviceInfoObject *pObj = coreObj->m_deviceInfo.getObjectByName(devName);

    m_sDeviceName = pObj->m_deviceName;
    m_iFrameLen = pObj->m_frameLen;
    m_sProtocol = pObj->m_protocol;
    m_sLink = pObj->m_link;
    m_iStateVar = pObj->m_stateVar;
    m_iFrameTimePeriod = pObj->m_frameTimePeriod;
    m_iCtrlVar = pObj->m_ctrlVar;
    m_bDynamicOptimization = pObj->m_dynamicOptimization;
    m_iRemotePort = pObj->m_remotePort;
    parsePropertiesFromString(pObj->m_properties, m_mapProperties);
    m_iRetryTimes = m_mapProperties.value("commFailRetryTimes").toInt();
    m_iCommTimeout = m_mapProperties.value("commTimeout").toInt() * 1000;
    m_iCommResumeTime = m_mapProperties.value("commResumeTime").toInt() * 1000;
    m_sDeviceType = "NET";

    NetDevice netDev;
    netDev.fromString(pObj->m_portParameters);
    m_sIpAddress = netDev.m_ipAddress;
    m_iPort = netDev.m_port;
    m_sIpAddress1 = netDev.m_ipAddress1;
    m_iPort1 = netDev.m_port1;

    return true;
}

//-----------------------------------------------------------------------------


Vendor::Vendor(QProjectCore *coreObj)
{
    m_pPortObj = NULL;
    m_readList.clear();
    m_bIsRunning = false;
    projCore = coreObj;
}


Vendor::~Vendor()
{
    stop();
    qDeleteAll(m_readList);
    m_readList.clear();
    if(m_pPortObj != NULL) {
        delete m_pPortObj;
        m_pPortObj = NULL;
    }
    if(m_pVendorPrivateObj != NULL) {
        delete m_pVendorPrivateObj;
        m_pVendorPrivateObj = NULL;
    }
}

void Vendor::clearReadBuffer()
{
    memset((void*)readBuf, 0, sizeof(readBuf) / sizeof(quint8));
}


void Vendor::clearWriteBuffer()
{
    memset((void*)writeBuf, 0, sizeof(writeBuf) / sizeof(quint8));
}


/**
 * @brief Vendor::getDeviceName
 * @details 获取设备名称
 * @return 设备名称
 */
QString Vendor::getDeviceName()
{
    if(m_pVendorPrivateObj != NULL) {
        return m_pVendorPrivateObj->m_sDeviceName;
    }
    return "";
}


/**
 * @brief Vendor::open
 * @details 打开设备
 * @return true-成功, false-失败
 */
bool Vendor::open()
{
    return true;
}


/**
 * @brief Vendor::initialize
 * @details 设备初始化
 */
void Vendor::initialize()
{
    if(m_pVendorPluginObj != NULL) {
        m_pVendorPluginObj->initailizeDevice(this);
        m_pVendorPluginObj->connectDevice(NULL);
    }
}


/**
 * @brief Vendor::addIOTagToDeviceTagList
 * @details 添加设备变量至变量列表
 * @param pTag 变量描述对象
 */
void Vendor::addIOTagToDeviceTagList(RunTimeTag* pTag)
{
    m_readList.append(pTag);
}

/**
 * @brief Vendor::findIOTagByID
 * @details 查找设备变量
 * @param id 变量ID
 * @return 变量描述对象
 */
RunTimeTag* Vendor::findIOTagByID(int id)
{
    for (int i = 0; i < m_readList.size(); ++i) {
        RunTimeTag* pTag = m_readList.at(i);
        if(pTag->id == id) {
            return pTag;
        }
    }
    return NULL;
}


/**
 * @brief Vendor::writeIOTag
 * @details 写变量
 * @param pTag 变量描述对象
 * @return true-成功, false-失败
 */
bool Vendor::writeIOTag(RunTimeTag* pTag)
{
    if(pTag && pTag->writeable == CAN_WRITE) {
        clearWriteBuffer();
        if(m_pVendorPluginObj && m_pPortObj) {
            m_pVendorPluginObj->beforeWriteIOTag(this, pTag);
            if(m_pVendorPluginObj->writeIOTag(this, m_pPortObj, pTag) != 1) {
                qint32 iRetryTimes = 0;
                do {
                    qDebug() << QString("device[%1] try to write tag[id: %2] again! %3/%4")
                                .arg(this->getDeviceName())
                                .arg(pTag->id)
                                .arg(QString::number(iRetryTimes + 1))
                                .arg(QString::number(m_pVendorPrivateObj->m_iRetryTimes));
                    iRetryTimes++;
                    if(m_pVendorPluginObj->writeIOTag(this, m_pPortObj, pTag) == 1) {
                        break;
                    }
                } while(iRetryTimes < m_pVendorPrivateObj->m_iRetryTimes);
                if(iRetryTimes >= m_pVendorPrivateObj->m_iRetryTimes) {
                    return false;
                }
            }
            m_pVendorPluginObj->afterWriteIOTag(this, pTag);
        }
        memset((void *)pTag->dataToVendor, 0, pTag->bufLength + 1);
    }
    return true;
}


/**
 * @brief Vendor::writeIOTags
 * @details 写变量列表
 * @return true-成功, false-失败
 */
bool Vendor::writeIOTags()
{
    if(this->m_bOffLine) {
        if((QDateTime::currentMSecsSinceEpoch() - this->m_iStartOffLineTime) > m_pVendorPrivateObj->m_iCommResumeTime) {
            if(!this->reconnect()) {
                this->m_bOffLine = true;
                this->m_bOnlineStatus = false;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                return false;
            } else {
                this->m_bOffLine = false;
                this->m_iStartOffLineTime = 0;
            }
        } else {
            QThread::msleep(50);
            return false;
        }
    }

    for (int i = 0; i < m_readList.size(); ++i) {
        if(!m_bIsRunning) {
            return false;
        }
        RunTimeTag* pTag = m_readList[i];
        if(pTag->dataToVendor[pTag->bufLength] > 0) {
            if(writeIOTag(pTag)) {
                this->m_bOffLine = false;
                this->m_iStartOffLineTime = 0;
                this->m_bOnlineStatus = true;
            } else {
                if(!this->m_bOffLine) {
                    qDebug() << QString("device[%1] start off line! time: %2 ms")
                                .arg(this->getDeviceName())
                                .arg(QString::number(QDateTime::currentMSecsSinceEpoch()));
                    this->m_bOffLine = true;
                    this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                    this->m_bOnlineStatus = false;
                    return false;
                }
            }
        }
    }

    return true;
}


/**
 * @brief Vendor::readIOTag
 * @details 读变量
 * @param pTag 变量描述对象
 * @return true-成功, false-失败
 */
bool Vendor::readIOTag(RunTimeTag* pTag)
{
    if(pTag) {
        clearReadBuffer();
        if(m_pVendorPluginObj && m_pPortObj) {
            m_pVendorPluginObj->beforeReadIOTag(this, pTag);

            if(m_pVendorPluginObj->readIOTag(this, m_pPortObj, pTag) != 1) {
                qint32 iRetryTimes = 0;
                do {
                    qDebug() << QString("device[%1] try to read tag[id: %2] again! %3/%4")
                                .arg(this->getDeviceName())
                                .arg(pTag->id)
                                .arg(QString::number(iRetryTimes + 1))
                                .arg(QString::number(m_pVendorPrivateObj->m_iRetryTimes));
                    iRetryTimes++;
                    if(m_pVendorPluginObj->readIOTag(this, m_pPortObj, pTag) == 1) {
                        break;
                    }
                } while(iRetryTimes < m_pVendorPrivateObj->m_iRetryTimes);
                if(iRetryTimes >= m_pVendorPrivateObj->m_iRetryTimes) {
                    return false;
                }
            } else {
                pTag->lastReadTime = QDateTime::currentMSecsSinceEpoch();
                if(pTag->isBlockRead) {
                    // 变量直接从块读变量拷贝数据
                    for (int i = 0; i < m_readList.size(); ++i) {
                        if(!m_bIsRunning) {
                            return false;
                        }

                        RunTimeTag* pRtTagObj = m_readList.at(i);
                        if(pTag->id == pRtTagObj->id) {
                            continue;
                        }

                        if(pTag->id == pRtTagObj->blockReadID) {
                            if(m_pVendorPluginObj->copyTagDataFromBlockReadTag(pTag, pRtTagObj)) {
                                pRtTagObj->lastReadTime = QDateTime::currentMSecsSinceEpoch();
                            } else {
                                qDebug() << "tag: " << pTag->id << " copy data from tag: "<< pRtTagObj->blockReadID <<" fail!";
                            }
                        }
                    }
                }
            }
            m_pVendorPluginObj->afterReadIOTag(this, pTag);
        }
    }
    return true;
}


/**
 * @brief Vendor::readIOTags
 * @details 读变量列表
 * @return true-成功, false-失败
 */
bool Vendor::readIOTags()
{
    if(this->m_bOffLine) {
        if((QDateTime::currentMSecsSinceEpoch() - this->m_iStartOffLineTime) > m_pVendorPrivateObj->m_iCommResumeTime) {
            if(!this->reconnect()) {
                this->m_bOffLine = true;
                this->m_bOnlineStatus = false;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                return false;
            } else {
                this->m_bOffLine = false;
                this->m_iStartOffLineTime = 0;
            }
        } else {
            QThread::msleep(50);
            return false;
        }
    }

    for (int i = 0; i < m_readList.size(); ++i) {
        if(!m_bIsRunning) {
            return false;
        }

        RunTimeTag* pTag = m_readList.at(i);

        // 变量采集周期100ms
        if((QDateTime::currentMSecsSinceEpoch() - pTag->lastReadTime) < 100) {
            continue;
        }

        if(readIOTag(pTag)) {
            this->m_bOffLine = false;
            this->m_iStartOffLineTime = 0;
            this->m_bOnlineStatus = true;
        } else {
            if(!this->m_bOffLine) {
                qDebug() << QString("device[%1] start off line! time: %2 ms")
                            .arg(this->getDeviceName())
                            .arg(QString::number(QDateTime::currentMSecsSinceEpoch()));
                this->m_bOffLine = true;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                this->m_bOnlineStatus = false;
                return false;
            }
        }
    }

    if(m_pVendorPrivateObj != NULL) {
        if(m_pVendorPrivateObj->m_iFrameTimePeriod > 0) {
            QThread::msleep(static_cast<unsigned long>(m_pVendorPrivateObj->m_iFrameTimePeriod));
        }
    }

    //RealTimeDB::instance()->debug();
    return true;
}


/**
 * @brief Vendor::isRunning
 * @details 是否运行
 * @return true-运行, false-停止
 */
bool Vendor::isRunning()
{
    return m_bIsRunning;
}


/**
 * @brief Vendor::start
 * @details 启动
 */
void Vendor::start()
{
    m_bIsRunning = true;
}


/**
 * @brief Vendor::stop
 * @details 停止
 */
void Vendor::stop()
{
    m_bIsRunning = false;
}


/**
 * @brief Vendor::doLoop
 * @details 循环读写处理逻辑
 */
void Vendor::doLoop()
{
    writeIOTags();
    readIOTags();
}


/**
 * @brief Vendor::isRestart
 * @details 是否重启运行
 * @return
 */
bool Vendor::isRestart()
{
    return true;
}


/**
 * @brief Vendor::isOnLine
 * @details 是否在线
 * @return
 */
bool Vendor::isOnLine()
{
    return true;
}


/**
 * @brief Vendor::close
 * @details 关闭设备
 * @return
 */
bool Vendor::close()
{
    if(m_pVendorPluginObj != NULL) {
        m_pVendorPluginObj->disconnectDevice(NULL);
        m_pVendorPluginObj->unInitailizeDevice(NULL);
    }
    return true;
}


/**
 * @brief Vendor::getPortName
 * @details 获取端口名称
 * @return 端口名称
 */
QString Vendor::getPortName()
{
    if(m_pVendorPrivateObj != NULL) {
        if(m_pVendorPrivateObj->m_sDeviceType == "COM") {
            ComDevicePrivate *pComDevicePrivateObj = (ComDevicePrivate *)m_pVendorPrivateObj;
            return pComDevicePrivateObj->m_sPortNumber;
        } else if(m_pVendorPrivateObj->m_sDeviceType == "NET") {
            NetDevicePrivate* pNetDevicePrivateObj = (NetDevicePrivate*)m_pVendorPrivateObj;
            return QString("%1:%2").arg(pNetDevicePrivateObj->m_sIpAddress).arg(pNetDevicePrivateObj->m_iPort);
        }
    }
    return "";
}


/**
 * @brief Vendor::reconnect
 * \details 设备端口重新连接
 * @return true-重新连接成功, false-重新连接失败
 */
bool Vendor::reconnect()
{
    if(m_pPortObj->getPortType() == PORT_NET) {
        qDebug() << QString("try to reconnect net device[%1]! port name: %2")
                    .arg(this->getDeviceName())
                    .arg(this->getPortName());
        if(!m_pPortObj->reOpen()) {
            return false;
        }
    }

    return true;
}


