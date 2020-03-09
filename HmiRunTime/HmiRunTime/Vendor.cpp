#include "Vendor.h"
#include <cstdlib>
#include <cstring>
#include "ProjectData.h"
#include "DBTagObject.h"
#include <QFile>
#include "Public/PublicFunction.h"
#include "Log/Log.h"
#include <QDateTime>
#include <QDebug>

//-----------------------------------------------------------------------------

#define RETRY_TIMES    (3)  // 通讯失败尝试次数
#define RECOVERY_INTERVAL    (10000) // 断线之后尝试恢复重新连接时间间隔N毫秒

//-----------------------------------------------------------------------------


VendorPrivate::VendorPrivate()
{

}


/*
* 从文件读取配置数据
*/
bool ComDevicePrivate::LoadData(const QString &devName)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByName(devName);

    m_sDeviceName = pObj->szDeviceName_;
    m_iFrameLen = pObj->iFrameLen_;
    m_sProtocol = pObj->szProtocol_;
    m_sLink = pObj->szLink_;
    m_iStateVar = pObj->iStateVar_;
    m_iFrameTimePeriod = pObj->iFrameTimePeriod_;
    m_iCtrlVar = pObj->iCtrlVar_;
    m_bDynamicOptimization = pObj->bDynamicOptimization_;
    m_iRemotePort = pObj->iRemotePort_;
    m_sDeviceType = "COM";

    ComDevice comDev;
    comDev.fromString(pObj->szPortParameters_);

    m_sPortNumber = comDev.szPortNumber_;
    m_iBaudrate = comDev.iBaudrate_;
    m_iDatabit = comDev.iDatabit_;
    m_fStopbit = comDev.fStopbit_;
    m_sVerifybit = comDev.szVerifybit_;
    m_iTimeout = comDev.iTimeout_;

    return true;
}



/*
* 从文件读取配置数据
*/
bool NetDevicePrivate::LoadData(const QString &devName)
{
    DeviceInfo &deviceInfo = ProjectData::getInstance()->deviceInfo_;
    deviceInfo.load(ProjectData::getInstance()->dbData_);
    DeviceInfoObject *pObj = deviceInfo.getDeviceInfoObjectByName(devName);

    m_sDeviceName = pObj->szDeviceName_;
    m_iFrameLen = pObj->iFrameLen_;
    m_sProtocol = pObj->szProtocol_;
    m_sLink = pObj->szLink_;
    m_iStateVar = pObj->iStateVar_;
    m_iFrameTimePeriod = pObj->iFrameTimePeriod_;
    m_iCtrlVar = pObj->iCtrlVar_;
    m_bDynamicOptimization = pObj->bDynamicOptimization_;
    m_iRemotePort = pObj->iRemotePort_;
    m_sDeviceType = "NET";

    NetDevice netDev;
    netDev.fromString(pObj->szPortParameters_);
    m_sIpAddress = netDev.szIpAddress_;
    m_iPort = netDev.iPort_;
    m_sIpAddress1 = netDev.szIpAddress1_;
    m_iPort1 = netDev.iPort1_;

    return true;
}

//-----------------------------------------------------------------------------


Vendor::Vendor()
{
    m_pPortObj = Q_NULLPTR;
    m_readList.clear();
    m_writeQueue.clear();
    m_bIsRunning = false;
}


Vendor::~Vendor()
{
    stop();

    qDeleteAll(m_readList);
    m_readList.clear();

    if(!m_writeQueue.isEmpty())
    {
        qDeleteAll(m_writeQueue);
        m_writeQueue.clear();
    }

    if(m_pPortObj != Q_NULLPTR) {
        delete m_pPortObj;
        m_pPortObj = Q_NULLPTR;
    }
    if(m_pVendorPrivateObj != Q_NULLPTR) {
        delete m_pVendorPrivateObj;
        m_pVendorPrivateObj = Q_NULLPTR;
    }
}

void Vendor::clearReadBuffer()
{
    memset((void*)readBuf, 0, sizeof(readBuf)/sizeof(quint8));
}


void Vendor::clearWriteBuffer()
{
    memset((void*)writeBuf, 0, sizeof(writeBuf)/sizeof(quint8));
}


/**
 * @brief Vendor::getDeviceName
 * @details 获取设备名称
 * @return 设备名称
 */
QString Vendor::getDeviceName()
{
    if(m_pVendorPrivateObj != Q_NULLPTR) {
        return m_pVendorPrivateObj->m_sDeviceName;
        //ComDevicePrivate *pComDevicePrivateObj = dynamic_cast<ComDevicePrivate *>(this->m_pVendorPrivateObj);
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
    if(m_pVendorPluginObj != Q_NULLPTR) {
        m_pVendorPluginObj->initailizeDevice(this);
        m_pVendorPluginObj->connectDevice(Q_NULLPTR);
    }
}


/**
 * @brief Vendor::addIOTagToDeviceTagList
 * @details 添加设备变量至变量列表
 * @param pTag 变量描述对象
 */
void Vendor::addIOTagToDeviceTagList(IOTag* pTag)
{
    m_readList.append(pTag);
}


/**
 * @brief Vendor::addIOTagToDeviceTagWriteQueue
 * @details 添加设备变量至变量写队列
 * @param pTag 变量描述对象
 */
void Vendor::addIOTagToDeviceTagWriteQueue(IOTag* pTag)
{
    QMutexLocker locker(&m_mutexWrite);
    if(!m_writeQueue.contains(pTag)) m_writeQueue.enqueue(pTag);
}


/**
 * @brief Vendor::findIOTagByID
 * @details 查找设备变量
 * @param id 变量ID
 * @return 变量描述对象
 */
IOTag* Vendor::findIOTagByID(const QString &id)
{
    for (int i = 0; i < m_readList.size(); ++i) {
        IOTag* pTag = m_readList.at(i);
        if(pTag->GetTagID() == id) {
            return pTag;
        }
    }
    return Q_NULLPTR;
}


/**
 * @brief Vendor::writeIOTag
 * @details 写变量
 * @param pTag 变量描述对象
 * @return true-成功, false-失败
 */
bool Vendor::writeIOTag(IOTag* pTag)
{
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == WRIE) {
        clearWriteBuffer();
        memset((void*)pTag->pWriteBuf, 0, pTag->mLength);
        if(m_pVendorPluginObj != Q_NULLPTR && m_pPortObj != Q_NULLPTR) {
            DBTagObject *pDBTagObject = (DBTagObject *)pTag->GetDBTagObject();
            if(pDBTagObject != Q_NULLPTR) {
                QByteArray baWriteDat = pDBTagObject->GetWriteDataBytes();
                memcpy(pTag->pWriteBuf, baWriteDat.data(), baWriteDat.size());
            }
            m_pVendorPluginObj->beforeWriteIOTag(pTag);
            if(m_pVendorPluginObj->writeIOTag(m_pPortObj, pTag) != 1) {
                qint32 iRetryTimes = 0;
                do {
                    LogInfo(QString("device[%1] try to write tag[id: %2] again! %3/%4")
                            .arg(this->getDeviceName())
                            .arg(pTag->GetTagID())
                            .arg(QString::number(iRetryTimes+1))
                            .arg(QString::number(RETRY_TIMES)));
                    iRetryTimes++;
                    if(m_pVendorPluginObj->writeIOTag(m_pPortObj, pTag) == 1) break;
                } while(iRetryTimes < RETRY_TIMES);
                if(iRetryTimes >= RETRY_TIMES) return false;
            }
            m_pVendorPluginObj->afterWriteIOTag(pTag);
        }
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
        if((QDateTime::currentMSecsSinceEpoch() - this->m_iStartOffLineTime) > RECOVERY_INTERVAL) {
            if(this->reconnect() == false) {
                this->m_bOffLine = true;
                this->m_bOnlineStatus = false;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                return false;
            }
            else {
                this->m_bOffLine = false;
                this->m_iStartOffLineTime = 0;
            }
        }
        else {
            QThread::msleep(50);
            return false;
        }
    }

    while (!m_writeQueue.isEmpty()) {
        if(!m_bIsRunning)
            return false;
        IOTag* pTag = m_writeQueue.dequeue();
        if(writeIOTag(pTag)) {
            this->m_bOffLine = false;
            this->m_iStartOffLineTime = 0;
            this->m_bOnlineStatus = true;
        } else {
            if(this->m_bOffLine == false) {
                LogInfo(QString("device[%1] start off line! time: %2 ms").arg(this->getDeviceName()).arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
                this->m_bOffLine = true;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                this->m_bOnlineStatus = false;
                return false;
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
bool Vendor::readIOTag(IOTag* pTag)
{
    QMutexLocker locker(&m_mutexWrite);
    if(pTag->GetPermissionType() == READ_WRIE || pTag->GetPermissionType() == READ) {
        clearReadBuffer();
        memset((void*)pTag->pReadBuf, 0, pTag->mLength);
        DBTagObject *pDBTagObject = (DBTagObject *)pTag->GetDBTagObject();
        if(m_pVendorPluginObj != Q_NULLPTR && m_pPortObj != Q_NULLPTR) {
            m_pVendorPluginObj->beforeReadIOTag(pTag);

            if(pTag->getBlockReadTagId() == "block") { // 块读取变量
                if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) != 1) {
                    qint32 iRetryTimes = 0;
                    do {
                        LogInfo(QString("device[%1] try to read tag[id: %2] again! %3/%4")
                                .arg(this->getDeviceName())
                                .arg(pTag->GetTagID())
                                .arg(QString::number(iRetryTimes+1))
                                .arg(QString::number(RETRY_TIMES)));
                        iRetryTimes++;
                        if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) == 1) break;
                    } while(iRetryTimes < RETRY_TIMES);
                    if(iRetryTimes >= RETRY_TIMES) {
                        pTag->setReadBlockReadTagSuccess(false);
                        return false;
                    }
                }
                pTag->setReadBlockReadTagSuccess(true);
            } else { // 单一变量读取操作
                IOTag* pBlockReadTag = pTag->getBlockReadTag();
                if(pBlockReadTag != Q_NULLPTR) {
                    // 块读取变量读取成功, 变量直接拷贝数据, 否则直接读取单一变量
                    if(pBlockReadTag->isReadBlockReadTagSuccess()) {
                        int iBlockRegAddr = pBlockReadTag->GetRegisterAddress() + pBlockReadTag->GetOffset();
                        int iBlockDataTypeLength = pBlockReadTag->GetDataTypeLength();
                        int iRegAddr = pTag->GetRegisterAddress() + pTag->GetOffset();
                        int iDataTypeLength = pTag->GetDataTypeLength();
                        TTagDataType iDataType = pTag->GetDataType();

                        if(iDataType == TYPE_BOOL) {
                            int iByteAddr = (iRegAddr-iBlockRegAddr)/8;
                            int iBitAddr = (iRegAddr-iBlockRegAddr)%8;
                            quint8 buf[1] = {0};
                            memcpy((void *)&buf[0], (const void *)&pBlockReadTag->pReadBuf[iByteAddr], 1);
                            pTag->pReadBuf[0] = (buf[0] >> iBitAddr) & 0x01;
                        } else {
                            int iByteAddr = (iRegAddr-iBlockRegAddr)*2;
                            memcpy((void *)&pTag->pReadBuf[0], (const void *)&pBlockReadTag->pReadBuf[iByteAddr], iDataTypeLength);
                        }

                    } else {
                        if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) != 1) {
                            qint32 iRetryTimes = 0;
                            do {
                                LogInfo(QString("device[%1] try to read tag[id: %2] again! %3/%4")
                                        .arg(this->getDeviceName())
                                        .arg(pTag->GetTagID())
                                        .arg(QString::number(iRetryTimes+1))
                                        .arg(QString::number(RETRY_TIMES)));
                                iRetryTimes++;
                                if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) == 1) break;
                            } while(iRetryTimes < RETRY_TIMES);
                            if(iRetryTimes >= RETRY_TIMES) return false;
                        }
                    }
                } else {
                    if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) != 1) {
                        qint32 iRetryTimes = 0;
                        do {
                            LogInfo(QString("device[%1] try to read tag[id: %2] again! %3/%4")
                                    .arg(this->getDeviceName())
                                    .arg(pTag->GetTagID())
                                    .arg(QString::number(iRetryTimes+1))
                                    .arg(QString::number(RETRY_TIMES)));
                            iRetryTimes++;
                            if(m_pVendorPluginObj->readIOTag(m_pPortObj, pTag) == 1) break;
                        } while(iRetryTimes < RETRY_TIMES);
                        if(iRetryTimes >= RETRY_TIMES) return false;
                    }
                }
                if(pDBTagObject != Q_NULLPTR)
                    pDBTagObject->SetData(pTag->pReadBuf);
            }

            m_pVendorPluginObj->afterReadIOTag(pTag);
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
        if((QDateTime::currentMSecsSinceEpoch() - this->m_iStartOffLineTime) > RECOVERY_INTERVAL) {
            if(this->reconnect() == false) {
                this->m_bOffLine = true;
                this->m_bOnlineStatus = false;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                return false;
            }
            else {
                this->m_bOffLine = false;
                this->m_iStartOffLineTime = 0;
            }
        }
        else {
            QThread::msleep(50);
            return false;
        }
    }

    for (int i = 0; i < m_readList.size(); ++i) {
        if(!m_writeQueue.isEmpty()) break;
        if(!m_bIsRunning) return false;

        IOTag* pTag = m_readList.at(i);

        if(readIOTag(pTag)) {
            this->m_bOffLine = false;
            this->m_iStartOffLineTime = 0;
            this->m_bOnlineStatus = true;
        } else {
            if(this->m_bOffLine == false) {
                LogInfo(QString("device[%1] start off line! time: %2 ms").arg(this->getDeviceName()).arg(QString::number(QDateTime::currentMSecsSinceEpoch())));
                this->m_bOffLine = true;
                this->m_iStartOffLineTime = QDateTime::currentMSecsSinceEpoch();
                this->m_bOnlineStatus = false;
                return false;
            }
        }

        if(pTag->getBlockReadTagId() == "block") { // 块读取变量
            if(m_pVendorPrivateObj != Q_NULLPTR) {
                if(m_pVendorPrivateObj->m_iFrameTimePeriod > 0)
                    QThread::msleep(static_cast<unsigned long>(m_pVendorPrivateObj->m_iFrameTimePeriod));
            }
        } else {
            IOTag* pBlockReadTag = pTag->getBlockReadTag();
            if(pBlockReadTag != Q_NULLPTR) {
                if(!pBlockReadTag->isReadBlockReadTagSuccess()) {
                    if(m_pVendorPrivateObj != Q_NULLPTR) {
                        if(m_pVendorPrivateObj->m_iFrameTimePeriod > 0)
                            QThread::msleep(static_cast<unsigned long>(m_pVendorPrivateObj->m_iFrameTimePeriod));
                    }
                }
            }
        }
    }
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
    if(m_pVendorPluginObj != Q_NULLPTR) {
        m_pVendorPluginObj->disconnectDevice(Q_NULLPTR);
        m_pVendorPluginObj->unInitailizeDevice(Q_NULLPTR);
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
    if(m_pVendorPrivateObj != Q_NULLPTR) {
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
        LogInfo(QString("try to reconnect net device[%1]! port name: %2").arg(this->getDeviceName()).arg(this->getPortName()));
        if(!m_pPortObj->reOpen()) return false;
    }

    return true;
}


